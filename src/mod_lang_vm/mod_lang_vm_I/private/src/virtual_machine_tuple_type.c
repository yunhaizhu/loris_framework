/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_tuple_type.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */

#include "virtual_machine_tuple_type.h"
#include "virtual_machine.h"

extern environment_vm_t vm[128];


/**
 * declare_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_tuple_type(IN ownership_object_symbol_t *symbol)
{
    std_int_t offset;
    std_lock_free_list_head_t *tuples = NULL;

    STD_ASSERT_RV(symbol != NULL, );

    tuples = (std_lock_free_list_head_t *) CALLOC(sizeof(std_lock_free_list_head_t), 1);
    offset = std_lock_free_list_head_entry_offset(ownership_object_t, list);
    std_lock_free_list_init(tuples, offset, STD_BOOL_FALSE, STD_BOOL_TRUE);

    symbol->env_value.symbol_type = tuple_type;
    symbol->env_value.data.tuple.tuples = tuples;
}


/**
 * inline_get_tuples
 * @brief   
 * @param   symbol
 * @param   pp_tuples
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t inline_get_tuples(IN const ownership_object_symbol_t *symbol, INOUT std_lock_free_list_head_t **pp_tuples)
{
    std_lock_free_list_head_t *tuples = NULL;

    STD_ASSERT_RV(symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(pp_tuples != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(symbol->env_value.symbol_type == tuple_type, STD_RV_ERR_UNEXPECTED);
    tuples = symbol->env_value.data.tuple.tuples;

    STD_ASSERT_RV_WARN(tuples != NULL, STD_RV_ERR_POINTER);

    *pp_tuples = tuples;

    return STD_RV_SUC;
}


/**
 * get_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   index
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t get_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index)
{
    ownership_object_t *value = NULL;
    ownership_object_t *item = NULL;
    std_lock_free_list_head_t *tuples = NULL;

    STD_ASSERT_RV(inline_get_tuples(symbol, &tuples) == STD_RV_SUC, NAN_BOX_Null);

    std_uint_t count = 0;
    std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(tuples->next);
    while (pos) {
        if (!STD_LOCK_FREE_LIST_HAS_MARK(pos->next)) {
            if (count == index) {
                item = std_lock_free_list_head_entry(pos, ownership_object_t, list);
                break;
            }
            count++;
        }
        pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);
    }
    STD_ASSERT_RV_WARN(count == index, NAN_BOX_Null);
    STD_ASSERT_RV_WARN(item != NULL, NAN_BOX_Null);

    value = item;

    return value->own_value;
}



/**
 * add_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   value
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t add_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN own_value_t value, IN std_bool_t copy)
{
    std_lock_free_list_head_t *tuples = NULL;
    ownership_object_t *value_obj = NULL;
    ownership_object_t *new_value_obj = NULL;
    own_value_type_t new_value_type;
    own_value_t new_value = NAN_BOX_Null;
    own_value_t new_object_value;
    std_u64_t u64_key;

    STD_ASSERT_RV(inline_get_tuples(symbol, &tuples) == STD_RV_SUC, );

    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
    STD_ASSERT_RV(check_support_own_value_type(value) == STD_BOOL_TRUE, );

    new_value_type = get_own_value_type(value);
    if (new_value_type == OWN_TYPE_OBJECT_STRING || new_value_type == OWN_TYPE_OBJECT_SYMBOL || new_value_type == OWN_TYPE_OBJECT) {
        if (copy) {
            new_object_value = duplicate_ownership_value(symbol, value);
        } else {
            value_obj = get_own_value_object(value);
            if (new_value_type != OWN_TYPE_OBJECT_SYMBOL && create_ownership_signature(symbol, value_obj) != STD_BOOL_TRUE) {
                new_object_value = duplicate_ownership_value(symbol, value);
            } else {
                new_object_value = value;
            }
        }
    } else {
        new_value = value;
        new_object_value = make_own_value_object(new_value);
        new_value_obj = get_own_value_object(new_object_value);

        create_ownership_signature(symbol, new_value_obj);
    }
    new_value_obj = get_own_value_object(new_object_value);
    new_value_obj->own_value = new_object_value;

    u64_key = build_u64key_with_object_value(new_value_obj);

    STD_ASSERT_RV_WARN_ACTION(std_lock_free_list_add(tuples, &new_value_obj->list, u64_key) == STD_RV_SUC, , free_ownership_ownvalue(symbol, new_object_value); );
}

/**
 * find_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t find_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key)
{
    ownership_object_t *item = NULL;
    std_lock_free_list_head_t *tuples = NULL;
    std_lock_free_list_head_t *pos;
    std_u64_t u64_key;

    STD_ASSERT_RV(key != NAN_BOX_Null, NAN_BOX_Null);
    STD_ASSERT_RV(inline_get_tuples(symbol, &tuples) == STD_RV_SUC, NAN_BOX_Null);

    u64_key = build_u64key_with_value(key);
    pos = std_lock_free_list_find(tuples, u64_key);
    STD_ASSERT_RV_WARN(pos != NULL, NAN_BOX_Null);

    item = std_lock_free_list_head_entry(pos, ownership_object_t, list);

    return NAN_BOX_SIGNATURE_OBJECT | (uint64_t) item;
}


/**
 * del_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   value
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t del_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN own_value_t value)
{
    ownership_object_t const *item = NULL;
    std_int_t ret;
    std_lock_free_list_head_t *tuples = NULL;
    std_lock_free_list_head_t *pos;
    std_u64_t u64_key;

    STD_ASSERT_RV(value != NAN_BOX_Null, NAN_BOX_Null);
    STD_ASSERT_RV(inline_get_tuples(symbol, &tuples) == STD_RV_SUC, NAN_BOX_Null);

    u64_key = build_u64key_with_value(value);
    pos = std_lock_free_list_find(tuples, u64_key);
    STD_ASSERT_RV(pos != NULL, NAN_BOX_Null);

    item = std_lock_free_list_head_entry(pos, ownership_object_t, list);

    ret = std_lock_free_list_del_flag(tuples, u64_key, STD_BOOL_FALSE);
    free_ownership_ownvalue(symbol, item->own_value);

    return make_own_value_number(ret);
}



/**
 * del_VARS_with_tuple_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t del_VARS_with_tuple_type(IN ownership_object_symbol_t *symbol)
{
    std_lock_free_list_head_t *tuples = NULL;

    STD_ASSERT_RV(symbol != NULL, STD_RV_ERR_INVALIDARG);

    std_int_t total = get_VAR_total_with_tuple_type(symbol);
    for (int j = 0; j < total; ++j) {
        own_value_t item = get_VAR_with_tuple_type(symbol, 0);

        STD_LOG(DEBUG, "FREE %s TUPLE %d %p \n", __FUNCTION__, 0, item);
        del_VAR_with_tuple_type(symbol, item);
    }

    tuples = symbol->env_value.data.tuple.tuples;
    symbol->env_value.data.tuple.tuples = NULL;

    STD_ASSERT_RV_WARN(tuples != NULL, STD_RV_ERR_UNEXPECTED);
    std_lock_free_list_cleanup(tuples);
    FREE(tuples);

    return STD_RV_SUC;
}



/**
 * get_VAR_total_with_tuple_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_VAR_total_with_tuple_type(IN const ownership_object_symbol_t *symbol)
{
    std_lock_free_list_head_t *tuples = NULL;
    std_int_t count;

    STD_ASSERT_RV_WARN(inline_get_tuples(symbol, &tuples) == STD_RV_SUC, 0);

    count = (std_int_t) std_lock_free_list_count(tuples);

    return count;
}


/**
 * move_VAR_with_tuple_type
 * @brief   
 * @param   from_symbol
 * @param   to_tuple
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t move_VAR_with_tuple_type(IN ownership_object_symbol_t *from_symbol, IN ownership_object_symbol_t *to_tuple)
{
    std_lock_free_list_head_t *tuples = NULL;

    STD_ASSERT_RV(from_symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(to_tuple != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(from_symbol->env_value.symbol_type == tuple_type, STD_RV_ERR_INVALIDARG);
    tuples = from_symbol->env_value.data.tuple.tuples;
    from_symbol->env_value.data.tuple.tuples = NULL;

    if (tuples == NULL){
        tuples = (std_lock_free_list_head_t *) CALLOC(sizeof(std_lock_free_list_head_t), 1);
        std_int_t offset = std_lock_free_list_head_entry_offset(ownership_object_t, list);
        std_lock_free_list_init(tuples, offset, STD_BOOL_FALSE, STD_BOOL_TRUE);
    }

    to_tuple->env_value.symbol_type = tuple_type;
    to_tuple->env_value.data.tuple.tuples = tuples;

    return STD_RV_SUC;
}

