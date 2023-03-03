/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_library.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#ifndef NONCLEDB_VIRTUAL_MACHINE_LIBRARY_H
#define NONCLEDB_VIRTUAL_MACHINE_LIBRARY_H

#include "std_common.h"
#include "virtual_machine_safe_var.h"

/**
 * std_void_t
 * @brief   
 * @param   args
 * @return  typedef
 */
typedef std_void_t (*register_func)(IN std_int_t thread_id, IN std_int_t args);
typedef struct func_entry_s {
    std_int_t arg_counts;
    register_func reg_func;
} func_entry_t;

/**
 * library_register
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_register();

/**
 * library_func_register
 * @brief   
 * @param   register_id
 * @param   func_name
 * @param   arg_counts
 * @param   func
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_func_register(IN std_int_t *register_id, IN std_char_t *func_name, IN std_int_t arg_counts, IN register_func func);

#endif
