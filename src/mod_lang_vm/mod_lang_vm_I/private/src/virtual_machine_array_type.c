/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_array_type.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#include "virtual_machine_array_type.h"
#include "virtual_machine.h"

extern environment_vm_t vm[128];


/**
 * declare_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   count
 * @param   var
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_array_type(IN ownership_object_symbol_t *symbol, IN std_int_t count, IN own_value_t var)
{
    STD_ASSERT_RV(symbol != NULL, );

    symbol->env_value.symbol_type = array_type;
    symbol->env_value.data.array.array_length = count;
    symbol->env_value.data.array.array = (own_value_t *) CALLOC(sizeof(own_value_t), count);
}

/**
 * inline_get_array
 * @brief   
 * @param   symbol
 * @param   pp_array
 * @param   p_count
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t inline_get_array(IN const ownership_object_symbol_t *symbol, INOUT own_value_t **pp_array, INOUT std_int_t *p_count)
{
    own_value_t *array = NULL;
    std_int_t count;

    STD_ASSERT_RV(symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(pp_array != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(p_count != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(symbol->env_value.symbol_type == array_type, STD_RV_ERR_UNEXPECTED);
    count = symbol->env_value.data.array.array_length;
    array = symbol->env_value.data.array.array;

    STD_ASSERT_RV_WARN(array != NULL, STD_RV_ERR_POINTER);

    *pp_array = array;
    *p_count = count;

    return STD_RV_SUC;
}

/**
 * get_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   index
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t get_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index)
{
    own_value_t *array = NULL;
    std_int_t count = 0;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &count) == STD_RV_SUC, NAN_BOX_Null);
    STD_ASSERT_RV_WARN(count > index, NAN_BOX_Null);

    return array[index];
}

/**
 * get_VAR_total_with_array_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_VAR_total_with_array_type(IN const ownership_object_symbol_t *symbol)
{
    own_value_t *array = NULL;
    std_int_t count = 0;

    STD_ASSERT_RV(inline_get_array(symbol, &array, &count) == STD_RV_SUC, 0);

    return count;
}

/**
 * set_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   index
 * @param   value
 * @param   copy
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t set_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index, IN own_value_t value, IN std_bool_t copy)
{
    own_value_t *array = NULL;
    std_int_t count = 0;
    ownership_object_t *new_value_obj;
    own_value_type_t new_value_type;
    own_value_t new_value = NAN_BOX_Null;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &count) == STD_RV_SUC, NAN_BOX_Null);
    STD_ASSERT_RV_WARN(count > index, NAN_BOX_Null);

    if (array[index]) {
        free_ownership_ownvalue(symbol, array[index]);
    }

    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
    STD_ASSERT_RV(check_support_own_value_type(value) == STD_BOOL_TRUE, NAN_BOX_Null);

    new_value_type = get_own_value_type(value);
    if (new_value_type == OWN_TYPE_OBJECT_STRING || new_value_type == OWN_TYPE_OBJECT_SYMBOL) {
        if (copy) {
            new_value = duplicate_ownership_value(symbol, value);
        } else {
            new_value_obj = get_own_value_object(value);
            if (new_value_type != OWN_TYPE_OBJECT_SYMBOL && create_ownership_signature(symbol, new_value_obj) != STD_BOOL_TRUE) {
                new_value = duplicate_ownership_value(symbol, value);
            } else {
                new_value = value;
            }
        }
    } else {
        new_value = value;
    }

    array[index] = new_value;

    return new_value;
}


/**
 * del_VARS_with_array_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t del_VARS_with_array_type(IN ownership_object_symbol_t *symbol)
{
    own_value_t *array = NULL;
    std_int_t count = 0;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &count) == STD_RV_SUC, STD_RV_ERR_FAIL);

    for (int i = 0; i < count; ++i) {
        if (array[i]) {
            free_ownership_ownvalue(symbol, array[i]);
        }
    }

    if (array) {
        FREE(array);
    }

    symbol->env_value.data.array.array = NULL;
    symbol->env_value.data.array.array_length = 0;

    return STD_RV_SUC;
}


/**
 * move_VAR_with_array_type
 * @brief   
 * @param   from_symbol
 * @param   to_symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t move_VAR_with_array_type(IN ownership_object_symbol_t *from_symbol, IN ownership_object_symbol_t *to_symbol)
{
    own_value_t *array = NULL;
    std_int_t array_length;

    STD_ASSERT_RV(from_symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(to_symbol != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(from_symbol->env_value.symbol_type == array_type, STD_RV_ERR_INVALIDARG);
    array = from_symbol->env_value.data.array.array;
    array_length = from_symbol->env_value.data.array.array_length;
    from_symbol->env_value.data.array.array = NULL;
    from_symbol->env_value.data.array.array_length = 0;

    to_symbol->env_value.data.array.array = array;
    to_symbol->env_value.data.array.array_length = array_length;
    to_symbol->env_value.symbol_type = array_type;

    return STD_RV_SUC;
}

