/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_lock_free_key_hash.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#include "std_lock_free_key_hash_imp.h"
#include "xxh3.h"

/**
 * std_partial_name_hash
 * @brief   
 * @param   c
 * @param   prev_hash
 * @return  static inline std_uint_t
 */
static inline std_uint_t std_partial_name_hash(IN std_int_t c, IN std_uint_t prev_hash)
{
    return (prev_hash + (c << 4) + (c >> 4)) * 11;
}

/**
 * stl_full_name_hash
 * @brief   
 * @param   name
 * @param   len
 * @param   hash_mask
 * @return  static inline std_uint_t
 */
static inline std_uint_t std_full_name_hash(IN const std_char_t *name, IN std_size_t len, IN std_uint_t hash_mask)
{
    std_uint_t hash = 0;

    while (len--) {
        if (name != NULL) {
            hash = std_partial_name_hash(*name++, hash);
        }
    }

    return hash % hash_mask;
}

/**
 * stl_lock_free_key_hash_create
 * @brief   
 * @param   hash_buckets
 * @return  std_lock_free_key_hash_t *
 */
STD_CALL std_lock_free_key_hash_t *std_lock_free_key_hash_create(IN std_uint_t hash_buckets)
{
    std_lock_free_key_hash_t *p_hash;

    p_hash = (std_lock_free_key_hash_t *) CALLOC(sizeof(std_lock_free_key_hash_t), 1);
    STD_ASSERT_RV(p_hash != NULL, NULL);

    p_hash->hash_buckets = hash_buckets;
    p_hash->total_count = 0;
    p_hash->list_head = (std_lock_free_list_head_t *) CALLOC(sizeof(std_lock_free_list_head_t), hash_buckets);
    STD_ASSERT_RV(p_hash->list_head != NULL, NULL);

    return p_hash;
}

/**
 * stl_lock_free_key_hash_add
 * @brief   
 * @param   p_hash
 * @param   p_key
 * @param   key_len
 * @param   p_value
 * @return  std_rv_t
 */
STD_CALL std_rv_t std_lock_free_key_hash_add(IN std_lock_free_key_hash_t *p_hash, IN std_char_t *p_key, IN std_size_t key_len, IN std_void_t *p_value)
{
    std_uint_t hash;
    std_lock_free_list_head_t *head_hash;
    std_lock_free_key_hash_node_t *p_node;
    std_u64_t u64_key;
    std_hazard_ptr_t *hp0;

    STD_ASSERT_RV(p_key != NULL, STD_RV_ERR_INVALIDARG);
//    STD_ASSERT_RV(p_value != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(p_hash != NULL, STD_RV_ERR_INVALIDARG);

    hash = std_full_name_hash(p_key, key_len, p_hash->hash_buckets);
    head_hash = p_hash->list_head + hash;

    if (head_hash->hazard_head == NULL) {
        std_uint_t offset = std_lock_free_list_head_entry_offset(std_lock_free_key_hash_node_t, link_hash);
        std_lock_free_list_init(&p_hash->list_head[hash], offset, STD_BOOL_TRUE, STD_BOOL_TRUE);
    }

    u64_key = XXH64(p_key, sizeof(char) * key_len, 0);
    hp0 = std_hazard_get(head_hash->hazard_head, 0);

    if (std_lock_free_list_is_empty(head_hash)) {
        goto NEW_ENTRY;
    }

    for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(head_hash->next); pos != NULL;
         pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next)) {
        p_node = std_lock_free_list_head_entry(pos, std_lock_free_key_hash_node_t, link_hash);
        std_hazard_set_ptr(hp0, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(p_node));
        if (u64_key == p_node->u64_key) {
            p_node->value = p_value;
            std_hazard_set_ptr(hp0, NULL);
            return STD_RV_SUC;
        }
    }

NEW_ENTRY:
    p_node = (std_lock_free_key_hash_node_t *) CALLOC(sizeof(std_lock_free_key_hash_node_t), 1);
    STD_ASSERT_RV(p_node != NULL, STD_RV_ERR_OUTOFMEMORY);

    snprintf(p_node->key, sizeof(p_node->key), "%s", p_key);
    p_node->value = p_value;
    p_node->u64_key = u64_key;

    ATOMIC_INC(&p_hash->total_count);
    std_lock_free_list_add(head_hash, &p_node->link_hash, u64_key);

    std_hazard_set_ptr(hp0, NULL);
    return STD_RV_SUC;
}

/**
 * stl_lock_free_key_hash_find
 * @brief   
 * @param   p_hash
 * @param   p_key
 * @param   key_len
 * @return  std_void_t *
 */
STD_CALL std_void_t *std_lock_free_key_hash_find(IN std_lock_free_key_hash_t *p_hash, IN const std_char_t *p_key, IN std_size_t key_len)
{
    std_uint_t hash;
    std_lock_free_list_head_t *head_hash;
    std_lock_free_key_hash_node_t *p_node;
    std_u64_t u64_key;
    std_hazard_ptr_t *hp0;
    std_void_t *value;

    STD_ASSERT_RV(p_hash != NULL, NULL);
    STD_ASSERT_RV(p_key != NULL, NULL);

    hash = std_full_name_hash(p_key, key_len, p_hash->hash_buckets);
    head_hash = p_hash->list_head + hash;

    if (head_hash->hazard_head == NULL) {
        return NULL;
    }

    if (std_lock_free_list_is_empty(head_hash)) {
        return NULL;
    }

    u64_key = XXH64(p_key, sizeof(char) * key_len, 0);
    hp0 = std_hazard_get(head_hash->hazard_head, 0);

    for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(head_hash->next); pos != NULL; pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next)) {
        p_node = std_lock_free_list_head_entry(pos, std_lock_free_key_hash_node_t, link_hash);
        std_hazard_set_ptr(hp0, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(p_node));
        if (u64_key == p_node->u64_key) {
            value = p_node->value;
            std_hazard_set_ptr(hp0, NULL);
            return value;
        }
    }

    std_hazard_set_ptr(hp0, NULL);
    return NULL;
}


/**
 * stl_lock_free_key_hash_del
 * @brief   
 * @param   p_hash
 * @param   p_key
 * @param   key_len
 * @return  std_rv_t
 */
STD_CALL std_rv_t std_lock_free_key_hash_del(IN std_lock_free_key_hash_t *p_hash, IN const std_char_t *p_key, IN std_size_t key_len)
{
    std_uint_t hash;
    std_lock_free_list_head_t *head_hash;
    const std_lock_free_key_hash_node_t *p_node;
    std_u64_t u64_key;
    std_hazard_ptr_t *hp0;

    STD_ASSERT_RV(p_key != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(p_hash != NULL, STD_RV_ERR_INVALIDARG);

    hash = std_full_name_hash(p_key, key_len, p_hash->hash_buckets);
    head_hash = p_hash->list_head + hash;

    if (head_hash->hazard_head == NULL) {
        return STD_RV_ERR_POINTER;
    }

    if (std_lock_free_list_is_empty(head_hash)) {
        return STD_RV_ERR_POINTER;
    }

    u64_key = XXH64(p_key, sizeof(char) * key_len, 0);
    hp0 = std_hazard_get(head_hash->hazard_head, 0);

    for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(head_hash->next); pos != NULL;) {
        p_node = std_lock_free_list_head_entry(pos, std_lock_free_key_hash_node_t, link_hash);
        pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);
        std_hazard_set_ptr(hp0, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(p_node));
        if (u64_key == p_node->u64_key) {
            std_lock_free_list_del(head_hash, u64_key);
            ATOMIC_DEC(&p_hash->total_count);
            std_hazard_set_ptr(hp0, NULL);
            return STD_RV_SUC;
        }
    }
    std_hazard_set_ptr(hp0, NULL);
    return STD_RV_ERR_POINTER;
}

/**
 * stl_lock_free_key_hash_destroy
 * @brief   
 * @param   p_hash
 * @return  std_void_t
 */
STD_CALL std_void_t std_lock_free_key_hash_destroy(IN std_lock_free_key_hash_t *p_hash)
{
    std_lock_free_list_head_t *head_hash;
    const std_lock_free_key_hash_node_t *p_node;

    STD_ASSERT_RV(p_hash != NULL, );

    for (std_int_t i = 0; i < p_hash->hash_buckets; i++) {
        head_hash = p_hash->list_head + i;
        if (head_hash->hazard_head == NULL) {
            continue;
        }
        if (std_lock_free_list_is_empty(head_hash)) {
            std_lock_free_list_cleanup(head_hash);
            continue;
        }
        for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(head_hash->next); pos != NULL;) {
            p_node = std_lock_free_list_head_entry(pos, std_lock_free_key_hash_node_t, link_hash);
            pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);
            std_lock_free_list_del(head_hash, p_node->u64_key);
        }
        std_lock_free_list_cleanup(head_hash);
    }

    FREE(p_hash->list_head);
    FREE(p_hash);
}

/**
 * stl_lock_free_key_hash_value_destroy
 * @brief   
 * @param   p_hash
 * @return  std_void_t
 */
STD_CALL std_void_t std_lock_free_key_hash_value_destroy(IN std_lock_free_key_hash_t *p_hash)
{
    std_lock_free_list_head_t *head_hash;
    const std_lock_free_key_hash_node_t *p_node;

    STD_ASSERT_RV(p_hash != NULL, );

    for (std_int_t i = 0; i < p_hash->hash_buckets; i++) {
        head_hash = p_hash->list_head + i;
        if (head_hash->hazard_head == NULL) {
            continue;
        }
        if (std_lock_free_list_is_empty(head_hash)) {
            std_lock_free_list_cleanup(head_hash);
            continue;
        }
        for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(head_hash->next); pos != NULL;) {
            p_node = std_lock_free_list_head_entry(pos, std_lock_free_key_hash_node_t, link_hash);
            pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);
            FREE(p_node->value);
            std_lock_free_list_del(head_hash, p_node->u64_key);
        }
        std_lock_free_list_cleanup(head_hash);
    }

    FREE(p_hash->list_head);
    FREE(p_hash);
}

typedef std_void_t (*destroy_callback_t)(std_void_t *data, std_void_t *callback_arg);
/**
 * std_lock_free_key_hash_value_callback_destroy
 * @brief   
 * @param   p_hash
 * @param   destroy_callback
 * @param   callback_arg
 * @return  std_void_t
 */
STD_CALL std_void_t std_lock_free_key_hash_value_callback_destroy(IN std_lock_free_key_hash_t *p_hash, IN destroy_callback_t destroy_callback, IN std_void_t *callback_arg)
{
    std_lock_free_list_head_t *head_hash;
    const std_lock_free_key_hash_node_t *p_node;

    STD_ASSERT_RV(p_hash != NULL, );

    for (std_int_t i = 0; i < p_hash->hash_buckets; i++) {
        head_hash = p_hash->list_head + i;
        if (head_hash->hazard_head == NULL) {
            continue;
        }
        if (std_lock_free_list_is_empty(head_hash)) {
            std_lock_free_list_cleanup(head_hash);
            continue;
        }
        for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(head_hash->next); pos != NULL;) {
            p_node = std_lock_free_list_head_entry(pos, std_lock_free_key_hash_node_t, link_hash);
            pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);

            if (destroy_callback != NULL) {
                destroy_callback(p_node->value, callback_arg);
            }

            std_lock_free_list_del(head_hash, p_node->u64_key);
        }
        std_lock_free_list_cleanup(head_hash);
    }

    FREE(p_hash->list_head);
    FREE(p_hash);
}

typedef std_void_t (*walk_callback_t)(const std_char_t *key, std_void_t **data, std_void_t *callback_arg);
/**
 * std_lock_free_key_hash_value_walk
 * @brief   
 * @param   p_hash
 * @param   walk_callback
 * @param   p_cb_data
 * @return  std_void_t
 */
STD_CALL std_void_t std_lock_free_key_hash_value_walk(IN std_lock_free_key_hash_t *p_hash, IN walk_callback_t walk_callback, IN std_void_t *p_cb_data)
{
    std_lock_free_list_head_t *head_hash;
    std_lock_free_key_hash_node_t *p_node;

    STD_ASSERT_RV(p_hash != NULL, );

    for (std_int_t i = 0; i < p_hash->hash_buckets; i++) {
        head_hash = p_hash->list_head + i;
        if (head_hash->hazard_head == NULL) {
            continue;
        }
        if (std_lock_free_list_is_empty(head_hash)) {
            continue;
        }
        for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(head_hash->next); pos != NULL;) {
            p_node = std_lock_free_list_head_entry(pos, std_lock_free_key_hash_node_t, link_hash);
            pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);

            if (walk_callback != NULL) {
                walk_callback(p_node->key, &(p_node->value), p_cb_data);
            }
        }
    }
}

/**
 * std_lock_free_key_hash_get_total_count
 * @brief   
 * @param   p_hash
 * @return  std_uint_t
 */
STD_CALL std_uint_t std_lock_free_key_hash_get_total_count(IN const std_lock_free_key_hash_t *p_hash)
{
    return p_hash->total_count;
}
