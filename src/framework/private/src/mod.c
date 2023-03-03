/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#include "mod_imp.h"
#include "mod_ownership.h"
#include "std_lock_free_key_hash.h"
#include "std_lock_free_list.h"

static std_lock_free_key_hash_t *gp_mod_hash = NULL;

/**
 * mod_init
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_init()
{
    gp_mod_hash = std_lock_free_key_hash_create(128);
    return STD_RV_SUC;
}

/**
 * mod_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_cleanup()
{
    std_lock_free_key_hash_destroy(gp_mod_hash);
    return STD_RV_SUC;
}

/**
 * mod_register_instance_func
 * @brief   
 * @param   p_iid
 * @param   p_func_create_instance
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_register_instance_func(IN const mod_iid_t *p_iid, IN create_instance_func_t p_func_create_instance)
{
    mod_head_t *p_head = NULL;
    std_char_t key[BUF_SIZE_128] = "\0";

    STD_ASSERT_RV(p_iid != NULL, STD_RV_ERR_POINTER);
    STD_ASSERT_RV(p_func_create_instance != NULL, STD_RV_ERR_POINTER);

    snprintf(key, sizeof(key), "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
             p_iid->data1, p_iid->data2, p_iid->data3,
             p_iid->data4[0], p_iid->data4[1], p_iid->data4[2], p_iid->data4[3],
             p_iid->data4[4], p_iid->data4[5], p_iid->data4[6], p_iid->data4[7]);

    p_head = std_lock_free_key_hash_find(gp_mod_hash, key, std_safe_strlen(key, BUF_SIZE_128));
    if (p_head == NULL) {
        p_head = (mod_head_t *) CALLOC(1, sizeof(mod_head_t));
        memcpy(&p_head->iid, p_iid, sizeof(mod_iid_t));
        p_head->p_func_create_instance = p_func_create_instance;
        p_head->instance_counts = 0;

        std_lock_free_list_init(&p_head->head,
                                std_lock_free_list_head_entry_offset(mod_instance_t, list),
                                STD_BOOL_TRUE,
                                STD_BOOL_TRUE);

        return std_lock_free_key_hash_add(gp_mod_hash, key, std_safe_strlen(key, BUF_SIZE_128), p_head);
    }

    return STD_RV_ERR_FAIL;
}

/**
 * mod_unregister_instance_func
 * @brief   
 * @param   p_iid
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_unregister_instance_func(IN const mod_iid_t *p_iid)
{
    mod_head_t *p_head = NULL;
    std_char_t key[BUF_SIZE_128] = "\0";
    std_rv_t ret = STD_RV_SUC;

    STD_ASSERT_RV(p_iid != NULL, STD_RV_ERR_FAIL);

    snprintf(key, sizeof(key), "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
             p_iid->data1, p_iid->data2, p_iid->data3,
             p_iid->data4[0], p_iid->data4[1], p_iid->data4[2], p_iid->data4[3],
             p_iid->data4[4], p_iid->data4[5], p_iid->data4[6], p_iid->data4[7]);

    p_head = std_lock_free_key_hash_find(gp_mod_hash, key, std_safe_strlen(key, BUF_SIZE_128));
    STD_ASSERT_RV(std_lock_free_list_is_empty(&p_head->head) == STD_BOOL_TRUE, STD_RV_ERR_POINTER);
    STD_ASSERT_RV(p_head->instance_counts == 0, STD_RV_ERR_POINTER);

    ret = std_lock_free_key_hash_del(gp_mod_hash, key, std_safe_strlen(key, BUF_SIZE_128));
    STD_ASSERT_RV(ret == STD_RV_SUC, STD_RV_ERR_POINTER);

    std_lock_free_list_cleanup(&p_head->head);
    FREE(p_head);

    return ret;
}

/**
 * mod_create_instance_with_iid_string
 * @brief   
 * @param   p_iid_string
 * @param   p_iid_string_size
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_create_instance_with_iid_string(IN const std_char_t *p_iid_string, IN std_size_t p_iid_string_size, OUT std_void_t **pp_handle, mod_ownership_t *owner_obj)
{
    mod_head_t *p_head = NULL;
    mod_instance_t *p_inst = NULL;
    create_instance_func_t p_func = NULL;
    std_rv_t ret = STD_RV_SUC;
    mod_ownership_t *need_obj;

    p_head = std_lock_free_key_hash_find(gp_mod_hash, p_iid_string, p_iid_string_size);
    STD_ASSERT_RV(p_head != NULL, STD_RV_ERR_POINTER);

    p_func = p_head->p_func_create_instance;
    ret = (*p_func)(pp_handle);
    STD_ASSERT_RV(ret == STD_RV_SUC, STD_RV_ERR_POINTER);
    need_obj = (mod_ownership_t *) *pp_handle;

    STD_ASSERT_RV(mod_ownership_create(owner_obj, need_obj) == STD_BOOL_TRUE, STD_RV_ERR_POINTER);

    p_inst = (mod_instance_t *) CALLOC(1, sizeof(mod_instance_t));
    p_inst->p_handle = *pp_handle;

    ret = std_lock_free_list_add(&p_head->head, &p_inst->list, (std_u64_t) &p_inst->list);
    ATOMIC_INC(&p_head->instance_counts);

    return ret;
}
/**
 * mod_create_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_create_instance(IN const mod_iid_t *p_iid, OUT std_void_t **pp_handle, mod_ownership_t *owner_obj)
{
    std_char_t key[BUF_SIZE_128] = "\0";

    snprintf(key, sizeof(key), "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
             p_iid->data1, p_iid->data2, p_iid->data3,
             p_iid->data4[0], p_iid->data4[1], p_iid->data4[2], p_iid->data4[3],
             p_iid->data4[4], p_iid->data4[5], p_iid->data4[6], p_iid->data4[7]);

    return mod_create_instance_with_iid_string(key, std_safe_strlen(key, BUF_SIZE_128), pp_handle, owner_obj);
}

/**
 * mod_delete_instance_with_iid_string
 * @brief   
 * @param   p_iid_string
 * @param   p_iid_string_size
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_delete_instance_with_iid_string(IN const std_char_t *p_iid_string, IN std_size_t p_iid_string_size, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj)
{
    mod_head_t *p_head = NULL;
    mod_instance_t *p_inst = NULL;
    mod_ownership_t *need_obj;

    p_head = std_lock_free_key_hash_find(gp_mod_hash, p_iid_string, p_iid_string_size);
    STD_ASSERT_RV(p_head != NULL, STD_RV_ERR_POINTER);

    STD_ASSERT_RV(p_head != NULL, STD_RV_ERR_POINTER);
    for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(p_head->head.next); pos != NULL;) {
        p_inst = std_lock_free_list_head_entry(pos, mod_instance_t, list);
        pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);

        if (p_inst->p_handle == *pp_handle) {
            need_obj = (mod_ownership_t *) p_inst->p_handle;
            STD_ASSERT_RV(mod_ownership_verify(owner_obj, need_obj) == STD_BOOL_TRUE, STD_RV_ERR_POINTER);
            STD_ASSERT_RV(mod_ownership_clear(owner_obj, need_obj) == STD_BOOL_TRUE, STD_RV_ERR_POINTER);

            FREE(p_inst->p_handle);
            *pp_handle = NULL;
            std_lock_free_list_del(&p_head->head, (std_u64_t) &p_inst->list);
            ATOMIC_DEC(&p_head->instance_counts);
            return STD_RV_SUC;
        }
    }

    return STD_RV_ERR_FAIL;
}

/**
 * mod_delete_instance
 * @brief   
 * @param   p_iid
 * @param   p_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_delete_instance(IN const mod_iid_t *p_iid, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj)
{
    std_char_t key[BUF_SIZE_128] = "\0";

    snprintf(key, sizeof(key), "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
             p_iid->data1, p_iid->data2, p_iid->data3,
             p_iid->data4[0], p_iid->data4[1], p_iid->data4[2], p_iid->data4[3],
             p_iid->data4[4], p_iid->data4[5], p_iid->data4[6], p_iid->data4[7]);

    return mod_delete_instance_with_iid_string(key, std_safe_strlen(key, BUF_SIZE_128), pp_handle, owner_obj);
}

/**
 * mod_query_instance_with_iid_string
 * @brief   
 * @param   p_iid_string
 * @param   p_iid_string_size
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_query_instance_with_iid_string(IN const std_char_t *p_iid_string, IN std_size_t p_iid_string_size, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj)
{
    mod_head_t *p_head = NULL;
    mod_instance_t *p_inst = NULL;
    mod_ownership_t *need_obj;

    p_head = std_lock_free_key_hash_find(gp_mod_hash, p_iid_string, p_iid_string_size);
    STD_ASSERT_RV(p_head != NULL, STD_RV_ERR_POINTER);

    for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(p_head->head.next); pos != NULL;) {
        p_inst = std_lock_free_list_head_entry(pos, mod_instance_t, list);
        pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);

        if (p_inst->p_handle != NULL) {
            need_obj = (mod_ownership_t *) p_inst->p_handle;
            *pp_handle = need_obj;
            return STD_RV_SUC;
        }
    }

    return STD_RV_ERR_FAIL;
}


/**
 * mod_query_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_query_instance(IN const mod_iid_t *p_iid, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj)
{
    std_char_t key[BUF_SIZE_128] = "\0";

    snprintf(key, sizeof(key), "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
             p_iid->data1, p_iid->data2, p_iid->data3,
             p_iid->data4[0], p_iid->data4[1], p_iid->data4[2], p_iid->data4[3],
             p_iid->data4[4], p_iid->data4[5], p_iid->data4[6], p_iid->data4[7]);

    return mod_query_instance_with_iid_string(key, std_safe_strlen(key, BUF_SIZE_128), pp_handle, owner_obj);
}

/**
 * mod_get_instance_counts
 * @brief   
 * @param   p_iid
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t mod_get_instance_counts(IN const mod_iid_t *p_iid)
{
    const mod_head_t *p_head = NULL;
    std_char_t key[BUF_SIZE_128] = "\0";

    snprintf(key, sizeof(key), "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
             p_iid->data1, p_iid->data2, p_iid->data3,
             p_iid->data4[0], p_iid->data4[1], p_iid->data4[2], p_iid->data4[3],
             p_iid->data4[4], p_iid->data4[5], p_iid->data4[6], p_iid->data4[7]);

    p_head = std_lock_free_key_hash_find(gp_mod_hash, key, std_safe_strlen(key, BUF_SIZE_128));
    STD_ASSERT_RV(p_head != NULL, STD_RV_ERR_FAIL);

    return p_head->instance_counts;
}