/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_safe_var.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#ifndef NONCLEDB_VIRTUAL_MACHINE_SAFE_VAR_H
#define NONCLEDB_VIRTUAL_MACHINE_SAFE_VAR_H

#include "std_common.h"
#include "virtual_machine.h"
#include "virtual_machine_object.h"
#include "virtual_machine_var_type.h"
#include "virtual_machine_array_type.h"
#include "virtual_machine_tuple_type.h"
#include "virtual_machine_hash_type.h"

#define LOCK_FREE_HASH_TABLE_SIZE 1024

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
STD_CALL std_void_t declare_VAR(ownership_object_symbol_t *symbol, symbol_type_t symbol_type, std_int_t size, own_value_t init_value, std_bool_t copy);

/**
 * set_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_rv_t set_VAR(own_value_t root, own_value_t index_key, own_value_t value);

/**
 * get_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t get_VAR(own_value_t root, own_value_t index_key, std_bool_t reenter);

/**
 * del_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t del_VAR(own_value_t root, own_value_t index_key);

/**
 * del_VARS
 * @brief   
 * @param   root
 * @param   del_tuple_or_hash
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t del_VARS(IN own_value_t root, IN std_bool_t del_tuple_or_hash);
#endif
