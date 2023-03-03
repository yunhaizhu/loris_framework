/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_READ.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-04
 *
 */
#ifndef NONCLEDB_VIRTUAL_MACHINE_READ_H
#define NONCLEDB_VIRTUAL_MACHINE_READ_H

#include "virtual_machine.h"

/**
 * read_code
 * @brief   
 * @param   buffer
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t read_code(IN const std_char_t *buffer);

/**
 * clean_codes
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t clean_codes();

/**
 * find_label
 * @brief   
 * @param   name
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t find_label(IN std_char_t *name, IN std_int_t line);

/**
 * find_label_ex
 * @brief   
 * @param   name
 * @param   args_count
 * @param   line
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t find_label_ex(IN std_char_t *name, IN std_int_t args_count, IN std_int_t line);

#endif