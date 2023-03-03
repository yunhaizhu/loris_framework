/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_hash_type.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#if 1
#include "virtual_machine_hash_type.h"
#include "virtual_machine.h"

extern environment_vm_t vm[128];



/**
 * declare_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_hash_type(IN ownership_object_symbol_t *symbol)
{
    const std_lock_free_key_hash_t *hash_head = NULL;

    STD_ASSERT_RV(symbol != NULL, );

    if (hash_head == NULL) {
        symbol->env_value.symbol_type = hash_type;
        symbol->env_value.data.hash.hash_head = std_lock_free_key_hash_create(LOCK_FREE_HASH_TABLE_SIZE);
    }
}

/**
 * inline_get_hash_head
 * @brief   
 * @param   symbol
 * @param   pp_hash
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t inline_get_hash_head(IN const ownership_object_symbol_t *symbol, INOUT std_lock_free_key_hash_t **pp_hash)
{
    std_lock_free_key_hash_t *hash_head = NULL;

    STD_ASSERT_RV(symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(pp_hash != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(symbol->env_value.symbol_type == hash_type, STD_RV_ERR_UNEXPECTED);
    hash_head = symbol->env_value.data.hash.hash_head;

    STD_ASSERT_RV_WARN(hash_head != NULL, STD_RV_ERR_POINTER);

    *pp_hash = hash_head;

    return STD_RV_SUC;
}



/**
 * add_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   key
 * @param   value
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t add_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key, IN own_value_t value, IN std_bool_t copy)
{
    std_char_t key_buf[BUF_SIZE_128] = "\0";
    std_lock_free_key_hash_t *hash_head = NULL;
    own_value_type_t value_type;
    own_value_t new_value = NAN_BOX_Null;
    ownership_object_t *value_obj = NULL;

    STD_ASSERT_RV(key != NAN_BOX_Null, );
    STD_ASSERT_RV(inline_get_hash_head(symbol, &hash_head) == STD_RV_SUC, );

    print_own_value_to_buf(key, key_buf, STD_BOOL_FALSE);

    value_type = get_own_value_type(value);
    if (value_type == OWN_TYPE_OBJECT_STRING || value_type == OWN_TYPE_OBJECT_SYMBOL) {
        if (copy) {
            new_value = duplicate_ownership_value(symbol, value);
        } else {
            value_obj = get_own_value_object(value);
            if (value_type != OWN_TYPE_OBJECT_SYMBOL && create_ownership_signature(symbol, value_obj) != STD_BOOL_TRUE) {
                new_value = duplicate_ownership_value(symbol, value);
            } else {
                new_value = value;
            }
        }
    } else {
        new_value = value;
    }

    del_VAR_with_hash_type(symbol, key);

    std_lock_free_key_hash_add(hash_head, key_buf, std_safe_strlen(key_buf, sizeof(key_buf)), (std_void_t *) new_value);
}



/**
 * del_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t del_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key)
{
    std_int_t ret;
    std_char_t key_buf[BUF_SIZE_128] = "\0";
    own_value_t value;
    std_lock_free_key_hash_t *hash_head = NULL;

    STD_ASSERT_RV(key != NAN_BOX_Null, NAN_BOX_Null);
    STD_ASSERT_RV(inline_get_hash_head(symbol, &hash_head) == STD_RV_SUC, NAN_BOX_Null);

    print_own_value_to_buf(key, key_buf, STD_BOOL_FALSE);
    value = (own_value_t ) std_lock_free_key_hash_find(hash_head, key_buf, std_safe_strlen(key_buf, sizeof(key_buf)));
    free_ownership_ownvalue(symbol, value);

    ret = std_lock_free_key_hash_del(hash_head, key_buf, std_safe_strlen(key_buf, sizeof(key_buf)));

    return make_own_value_number(ret);
}



/**
 * find_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t find_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key)
{
    own_value_t ret;
    std_char_t key_buf[BUF_SIZE_128] = "\0";
    std_lock_free_key_hash_t *hash_head = NULL;

    STD_ASSERT_RV(key != NAN_BOX_Null, NAN_BOX_Null);
    STD_ASSERT_RV(inline_get_hash_head(symbol, &hash_head) == STD_RV_SUC, NAN_BOX_Null);

    print_own_value_to_buf(key, key_buf, STD_BOOL_FALSE);
    ret = (own_value_t )std_lock_free_key_hash_find(hash_head, key_buf, std_safe_strlen(key_buf, sizeof(key_buf)));

    return  ret;
}



/**
 * del_VAR_with_hash_type_callback
 * @brief   
 * @param   value
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t del_VAR_with_hash_type_callback(IN std_void_t *value, IN const std_void_t *callback_arg)
{
    STD_LOG(DEBUG, "FREE %s  %p\n", __FUNCTION__, value);
    free_ownership_ownvalue( callback_arg, (own_value_t ) value);
}


/**
 * del_VARS_with_hash_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t del_VARS_with_hash_type(ownership_object_symbol_t *symbol)
{
    std_lock_free_key_hash_t *hash_head = NULL;

    STD_ASSERT_RV_WARN(inline_get_hash_head(symbol, &hash_head) == STD_RV_SUC, STD_RV_ERR_FAIL);

    std_lock_free_key_hash_value_callback_destroy(hash_head, del_VAR_with_hash_type_callback, symbol);
    symbol->env_value.data.hash.hash_head = NULL;

    return STD_RV_SUC;
}


/**
 * move_VAR_with_hash_type
 * @brief   
 * @param   from_symbol
 * @param   to_symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t move_VAR_with_hash_type(IN ownership_object_symbol_t *from_symbol, IN ownership_object_symbol_t *to_symbol)
{
    std_lock_free_key_hash_t *hash_head = NULL;

    STD_ASSERT_RV(from_symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(to_symbol != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(from_symbol->env_value.symbol_type == hash_type, STD_RV_ERR_INVALIDARG);
    hash_head = from_symbol->env_value.data.hash.hash_head;
    from_symbol->env_value.data.hash.hash_head = NULL;

    to_symbol->env_value.data.hash.hash_head = hash_head;
    to_symbol->env_value.symbol_type = hash_type;

    return STD_RV_SUC;
}


/**
 * walk_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   walk_callback
 * @param   p_cb_data
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t walk_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN walk_callback_t walk_callback, IN std_void_t *p_cb_data)
{
    std_lock_free_key_hash_t *hash_head = NULL;

    STD_ASSERT_RV(inline_get_hash_head(symbol, &hash_head) == STD_RV_SUC, );

    std_lock_free_key_hash_value_walk(hash_head, walk_callback, p_cb_data);
}
#endif
