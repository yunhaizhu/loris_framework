/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_tuple_type.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#ifndef NONCLEDB_VIRTUAL_MACHINE_TUPLE_TYPE_H
#define NONCLEDB_VIRTUAL_MACHINE_TUPLE_TYPE_H

#include "std_common.h"
#include "std_lock_free_list.h"
#include "virtual_machine.h"
#include "virtual_machine_var_type.h"


/**
 * declare_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_tuple_type(IN ownership_object_symbol_t *symbol);

/**
 * get_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   index
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t get_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index);

/**
 * add_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   value
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t add_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN own_value_t value, IN std_bool_t copy);

/**
 * find_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t find_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key);

/**
 * del_VAR_with_tuple_type
 * @brief   
 * @param   symbol
 * @param   value
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t del_VAR_with_tuple_type(IN const ownership_object_symbol_t *symbol, IN own_value_t value);

/**
 * del_VARS_with_tuple_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t del_VARS_with_tuple_type(IN ownership_object_symbol_t *symbol);

/**
 * get_VAR_total_with_tuple_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_VAR_total_with_tuple_type(IN const ownership_object_symbol_t *symbol);

/**
 * move_VAR_with_tuple_type
 * @brief   
 * @param   from_symbol
 * @param   to_tuple
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t move_VAR_with_tuple_type(IN ownership_object_symbol_t *from_symbol, IN ownership_object_symbol_t *to_tuple);

#endif