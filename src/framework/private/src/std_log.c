/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_log.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-11
 *
 */

#include "std_log.h"
#include "std_common.h"

FILE *global_log_file = NULL;
std_int_t debug_log_level = DEBUG;


/**
 * STD_LOG_INIT
 * @brief   
 * @param   log_level
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t STD_LOG_INIT(IN std_int_t log_level)
{
    debug_log_level = log_level;
    global_log_file = fopen("test.log", "w");
}
