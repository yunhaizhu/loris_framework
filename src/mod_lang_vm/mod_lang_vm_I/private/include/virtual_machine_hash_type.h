/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_hash_type.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#ifndef NONCLEDB_VIRTUAL_MACHINE_HASH_TYPE_H
#define NONCLEDB_VIRTUAL_MACHINE_HASH_TYPE_H

#include "std_common.h"
#include "virtual_machine.h"
#include "virtual_machine_object.h"

/**
 * declare_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_hash_type(IN ownership_object_symbol_t *symbol);

/**
 * add_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   key
 * @param   value
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t add_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key, IN own_value_t value, IN std_bool_t copy);

/**
 * del_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t del_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key);

/**
 * find_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   key
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t find_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN own_value_t key);

/**
 * del_VARS_with_hash_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t del_VARS_with_hash_type(ownership_object_symbol_t *symbol);

/**
 * move_VAR_with_hash_type
 * @brief   
 * @param   from_symbol
 * @param   to_hash
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t move_VAR_with_hash_type(IN ownership_object_symbol_t *from_symbol, IN ownership_object_symbol_t *to_hash);

/**
 * walk_VAR_with_hash_type
 * @brief   
 * @param   symbol
 * @param   walk_callback
 * @param   p_cb_data
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t walk_VAR_with_hash_type(IN const ownership_object_symbol_t *symbol, IN walk_callback_t walk_callback, IN std_void_t *p_cb_data);


#endif//NONCLEDB_VIRTUAL_MACHINE_HASH_TYPE_H
