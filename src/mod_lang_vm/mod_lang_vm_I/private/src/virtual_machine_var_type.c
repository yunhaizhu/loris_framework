/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_var_type.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#include "virtual_machine_var_type.h"
#include "virtual_machine.h"

extern environment_vm_t vm[128];


/**
 * inline_get_var
 * @brief   
 * @param   symbol
 * @param   pp_var
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline own_value_t inline_get_var(IN const ownership_object_symbol_t *symbol)
{
    own_value_t var;

    var = symbol->env_value.data.val.value;

    return var;
}


/**
 * inline_set_var
 * @brief   
 * @param   symbol
 * @param   var
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_void_t inline_set_var(IN ownership_object_symbol_t *symbol, IN own_value_t var)
{
    symbol->env_value.symbol_type = var_type;
    symbol->env_value.data.val.value = var;
}


/**
 * get_VAR_with_var_type
 * @brief
 * @param   variable
 * @param   read_write
 * @return  STD_CALL object_t *
 */
STD_CALL own_value_t get_VAR_with_var_type(IN const ownership_object_symbol_t *symbol)
{
    return inline_get_var(symbol);
}

/**
 * set_VAR_with_var_type
 * @brief
 * @param   variable
 * @param   value
 * @param   copy
 * @param   del
 * @return  STD_CALL std_void_t
 */
STD_CALL std_rv_t set_VAR_with_var_type(IN ownership_object_symbol_t *symbol, IN own_value_t value, IN std_bool_t copy, IN std_bool_t del)
{
    own_value_t old_value = NAN_BOX_Null;
    own_value_t new_value = NAN_BOX_Null;
    own_value_type_t old_value_type;
    own_value_type_t new_value_type;
    ownership_object_t *new_value_obj;

    assert(symbol != NULL);

    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);

    old_value = get_VAR_with_var_type(symbol);
    old_value_type = get_own_value_type(old_value);
    new_value_type = get_own_value_type(value);

    if ((old_value_type == OWN_TYPE_OBJECT || old_value_type == OWN_TYPE_OBJECT_STRING) && del) {
        free_ownership_ownvalue(symbol, old_value);
    }

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

    inline_set_var(symbol, new_value);

    return STD_RV_SUC;
}


/**
 * del_VAR_with_var_type
 * @brief   
 * @param   variable
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t del_VAR_with_var_type(IN ownership_object_symbol_t *variable)
{
    return set_VAR_with_var_type(variable, NAN_BOX_Null, STD_BOOL_FALSE, STD_BOOL_TRUE);
}


/**
 * move_VAR_with_var_type
 * @brief
 * @param   from_variable
 * @param   to_variable
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t move_VAR_with_var_type(IN ownership_object_symbol_t *from_symbol, ownership_object_symbol_t *to_symbol)
{
    own_value_t value;

    value = inline_get_var(from_symbol);

    inline_set_var(from_symbol, NAN_BOX_Null);
    inline_set_var(to_symbol, value);

    return STD_RV_SUC;
}


/**
 * declare_VAR_with_var_type
 * @brief
 * @param   symbol
 * @param   init_value_obj
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_rv_t declare_VAR_with_var_type(IN ownership_object_symbol_t *symbol, IN own_value_t init_value, IN std_bool_t copy)
{
    ownership_object_t *init_value_obj;
    own_value_t dup_value;

    STD_ASSERT_RV(symbol != NULL, STD_RV_ERR_INVALIDARG);

    switch (get_own_value_type(init_value)) {
        case OWN_TYPE_NUMBER:
        case OWN_TYPE_DOUBLE:
        case OWN_TYPE_BOOL:
        case OWN_TYPE_ADDRESS:
            inline_set_var(symbol, init_value);
            return STD_RV_SUC;
        case OWN_TYPE_OBJECT:
        case OWN_TYPE_OBJECT_STRING:
        case OWN_TYPE_OBJECT_SYMBOL:
            if (copy) {
                dup_value = duplicate_ownership_value(symbol, init_value);
            } else {
                init_value_obj = get_own_value_object(init_value);
                if (get_own_value_type(init_value) != OWN_TYPE_OBJECT_SYMBOL && create_ownership_signature(symbol, init_value_obj) != STD_BOOL_TRUE) {
                    init_value = duplicate_ownership_value(symbol, init_value);
                }
                dup_value = init_value;
            }
            inline_set_var(symbol, dup_value);
            return STD_RV_SUC;
        default:
            break;
    }
    return STD_RV_SUC;
}
