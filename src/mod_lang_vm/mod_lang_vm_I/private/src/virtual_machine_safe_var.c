/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_safe_var.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#include "virtual_machine_safe_var.h"
#include "virtual_machine_array_type.h"
#include "virtual_machine_tuple_type.h"
#include "virtual_machine_hash_type.h"

extern environment_vm_t vm[128];


/**
 * declare_VAR
 * @brief   
 * @param   symbol
 * @param   symbol_type
 * @param   size
 * @param   init_value
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR(ownership_object_symbol_t *symbol, symbol_type_t symbol_type, std_int_t size, own_value_t init_value, std_bool_t copy)
{
    STD_ASSERT_RV(symbol != NULL, );

    symbol->env_value.symbol_type = symbol_type;

    switch (symbol_type) {
        case var_type:
            declare_VAR_with_var_type(symbol, init_value, copy);
            break;

        case array_type:
            declare_VAR_with_array_type(symbol, size, init_value);
            break;

        case tuple_type:
            declare_VAR_with_tuple_type(symbol);
            break;

        case hash_type:
            declare_VAR_with_hash_type(symbol);
            break;

        default:
            break;
    }
}

/**
 * inline_set_VAR_copy_with_var_type
 * @brief   
 * @param   root
 * @param   value
 * @param   copy
 * @return  STD_CALL inline std_void_t
 */
STD_CALL static inline std_void_t inline_set_VAR_with_var_type(IN ownership_object_symbol_t *root_symbol, IN own_value_t value)
{
    own_value_t root_value;
    own_value_type_t root_value_type;

    root_value = get_VAR_with_var_type(root_symbol);

    if (root_value == value) {
        return;
    }

    root_value_type = get_own_value_type(root_value);

    if (root_value_type == OWN_TYPE_OBJECT_SYMBOL) {
        if (set_VAR(root_value, NAN_BOX_Null, value) != STD_RV_SUC){
            set_VAR_with_var_type(root_symbol, value, STD_BOOL_FALSE, STD_BOOL_TRUE);
        }
    } else {
        set_VAR_with_var_type(root_symbol, value, STD_BOOL_FALSE, STD_BOOL_TRUE);
    }
}


/**
 * set_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_rv_t set_VAR(own_value_t root, own_value_t index_key, own_value_t value)
{
    std_int_t idx = 0;
    ownership_object_symbol_t *root_symbol;

    root_symbol = get_own_value_object_symbol(root);

    switch (root_symbol->env_value.symbol_type) {
        case var_type:
            inline_set_VAR_with_var_type(root_symbol, value);
            break;

        case array_type:
            if (index_key != NAN_BOX_Null) {
                if (get_own_value_type(index_key) == OWN_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }
                idx = (std_int_t) get_own_value_number(index_key);
            }

            set_VAR_with_array_type(root_symbol, idx, value, STD_BOOL_FALSE);
            break;

        case tuple_type:
            add_VAR_with_tuple_type(root_symbol, value, STD_BOOL_FALSE);
            break;

        case hash_type:
            if (index_key != NAN_BOX_Null && get_own_value_type(index_key) == OWN_TYPE_OBJECT_SYMBOL) {
                index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
            }else if (index_key == NAN_BOX_Null){
                return STD_RV_ERR_FAIL;
            }else {
                add_VAR_with_hash_type(root_symbol, index_key, value, STD_BOOL_TRUE);
            }

            break;

        default:
            break;
    }
    return STD_RV_SUC;
}


/**
 * inline_get_VAR_switch_var
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL inline object_t *
 */
STD_CALL static inline own_value_t inline_get_VAR_switch_var(IN ownership_object_symbol_t *root_symbol, IN const own_value_t index_key, IN std_bool_t reenter)
{
    return get_VAR_with_var_type(root_symbol);
}


/**
 * inline_get_VAR_switch_array
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL inline object_t *
 */
STD_CALL static inline own_value_t inline_get_VAR_switch_array(IN own_value_t root, own_value_t index_key, IN std_bool_t reenter)
{
    own_value_t value = NAN_BOX_Null;
    std_int_t idx;

    if (reenter) {
        return root;
    }

    if (index_key != NAN_BOX_Null && get_own_value_type(index_key) == OWN_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
        idx = (std_int_t) get_own_value_number(index_key);
    } else if (index_key != NAN_BOX_Null) {
        idx = (std_int_t) get_own_value_number(index_key);
    } else {
        return root;
    }

    value = get_VAR_with_array_type(get_own_value_object_symbol(root), idx);

    return value;
}

/**
 * inline_get_VAR_switch_tuple
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t inline_get_VAR_switch_tuple(IN own_value_t root, own_value_t index_key, IN std_bool_t reenter)
{
    own_value_t value = NAN_BOX_Null;
    std_int_t idx;

    if (reenter) {
        return root;
    }

    if (index_key != NAN_BOX_Null && get_own_value_type(index_key) == OWN_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
        idx = (std_int_t) get_own_value_number(index_key);
    } else if (index_key != NAN_BOX_Null) {
        if (get_own_value_type(index_key) != OWN_TYPE_NUMBER){
            value = find_VAR_with_tuple_type(get_own_value_object_symbol(root), index_key);
            if (value != NAN_BOX_Null){
                return value;
            }

            return value;
        }
        idx = (std_int_t) get_own_value_number(index_key);
    } else {
        return root;
    }

    value = get_VAR_with_tuple_type(get_own_value_object_symbol(root), idx);

    return value;
}


/**
 * inline_get_VAR_switch_hash
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t inline_get_VAR_switch_hash(IN own_value_t root, own_value_t index_key, IN std_bool_t reenter)
{
    own_value_t value = NAN_BOX_Null;

    if (reenter) {
        return root;
    }
    if (index_key != NAN_BOX_Null && get_own_value_type(index_key) == OWN_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
    } else if (index_key == NAN_BOX_Null) {
        return root;
    }

    value = find_VAR_with_hash_type(get_own_value_object_symbol(root), index_key);

    return value;
}

#define LOOP_CHECK_AND_RETURN()              \
    index_key = NAN_BOX_Null;                        \
    if (value != NAN_BOX_Null && get_own_value_type(value) != OWN_TYPE_OBJECT_SYMBOL) { \
        return value;                        \
    }                                        \
    if (reenter) {                           \
        keep_loop = STD_BOOL_FALSE;          \
    } else {                                 \
        reenter = STD_BOOL_TRUE;             \
        keep_loop = STD_BOOL_TRUE;           \
    }


/**
 * get_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t get_VAR(own_value_t root, own_value_t index_key, std_bool_t reenter)
{
    own_value_t value = root;
    std_bool_t keep_loop;
    std_int_t loop_max = 1;
    own_value_type_t value_type;

    do {
        keep_loop = STD_BOOL_FALSE;

        value_type = get_own_value_type(value);

        if (value_type == OWN_TYPE_OBJECT_SYMBOL) {
            ownership_object_symbol_t *value_symbol;
            value_symbol = get_own_value_object_symbol(value);

            switch (value_symbol->env_value.symbol_type) {
                case var_type:
                    value = inline_get_VAR_switch_var(value_symbol, index_key, reenter);
                    keep_loop = STD_BOOL_TRUE;
                    loop_max++;
                    if (loop_max > 10) {
                        keep_loop = STD_BOOL_FALSE;
                        break;
                    }
                    break;

                case array_type:
                    value = inline_get_VAR_switch_array(value, index_key, reenter);
                    LOOP_CHECK_AND_RETURN()
                    break;

                case tuple_type:
                    value = inline_get_VAR_switch_tuple(value, index_key, reenter);
                    LOOP_CHECK_AND_RETURN()

                    break;

                case hash_type:
                    value = inline_get_VAR_switch_hash(value, index_key, reenter);
                    LOOP_CHECK_AND_RETURN()
                    break;

                case func_type:
                    return value;

                default:
                    break;
            }
        } else {
            return value;
        }
    } while (keep_loop);

    return value;
}


/**
 * del_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t del_VAR(own_value_t root, own_value_t index_key)
{
    own_value_type_t root_type;
    ownership_object_symbol_t const *root_symbol;

    root_type = get_own_value_type(root);

    if (root_type == OWN_TYPE_OBJECT_SYMBOL) {
        root_symbol = get_own_value_object_symbol(root);

        switch (root_symbol->env_value.symbol_type) {
            case tuple_type:
                if (get_own_value_type(index_key) == OWN_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }
                return del_VAR_with_tuple_type(get_own_value_object_symbol(root), index_key);

            case hash_type:
                if (get_own_value_type(index_key) == OWN_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }
                return del_VAR_with_hash_type(get_own_value_object_symbol(root), index_key);

            default:
                break;
        }
    }
    return NAN_BOX_Null;
}

/**
 * del_VARS
 * @brief   
 * @param   root
 * @param   del_tuple_or_hash
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t del_VARS(IN own_value_t root, IN std_bool_t del_tuple_or_hash)
{
    own_value_type_t root_type;
    ownership_object_symbol_t *root_symbol;

    root_type = get_own_value_type(root);
    if (root_type == OWN_TYPE_OBJECT_SYMBOL) {
        root_symbol = get_own_value_object_symbol(root);

        switch (root_symbol->env_value.symbol_type) {

            case var_type:
                del_VAR_with_var_type(root_symbol);
                break;

            case array_type:
                if (del_tuple_or_hash) {
                    del_VARS_with_array_type(root_symbol);
                }
                break;

            case tuple_type:
                if (del_tuple_or_hash) {
                    del_VARS_with_tuple_type(root_symbol);
                }
                break;

            case hash_type:
                if (del_tuple_or_hash) {
                    del_VARS_with_hash_type(root_symbol);
                }
                break;

            default:
                break;
        }
    }
}