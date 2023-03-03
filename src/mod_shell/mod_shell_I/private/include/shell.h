/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    shell.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-19
 *
 */
#ifndef SLOWLORIS_SHELL_H
#define SLOWLORIS_SHELL_H

#include "std_common.h"

typedef enum cmd_id {
    cmd_id_help = 1,
    cmd_id_ps,
    cmd_id_install,
    cmd_id_uninstall,
    cmd_id_start,
    cmd_id_stop,
    cmd_id_show,
    cmd_id_sleep,
    cmd_id_shell,
    cmd_id_exit,
    cmd_id_grace_exit,
    cmd_id_run,
} cmd_id_t;

/**
 * cmd_script
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_script();

/**
 * cmd_install
 * @brief   
 * @param   bundle_name
 * @param   bundle_name_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_install(IN std_char_t *bundle_name, IN std_size_t bundle_name_len);

/**
 * cmd_uninstall
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_uninstall(IN std_uint_t bundle_id);

/**
 * cmd_start
 * @brief   
 * @param   bundle_id
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_start(IN std_uint_t bundle_id, IN std_char_t *arg, IN std_size_t arg_len);

/**
 * cmd_stop
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_stop(IN std_uint_t bundle_id);

/**
 * cmd_show
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_show(IN std_uint_t bundle_id);

/**
 * cmd_run
 * @brief   
 * @param   function
 * @param   function_len
 * @param   p_handle
 * @param   args
 * @param   args_len
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *cmd_run(IN const std_char_t *function, IN std_size_t function_len, IN std_void_t *p_handle, IN std_char_t *args, IN std_size_t args_len);
/**
 * cmd_help
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_help();

/**
 * cmd_ps
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_ps();

/**
 * cmd_grace_exit
 * @brief   
 * @param   bundle_id_max
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_grace_exit(IN int bundle_id_max);

/**
 * cmd_exit
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_exit();


/**
 * cmd_shell
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_shell();

/**
 * cmd_debug
 * @brief
 * @param   debug_level
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_debug(IN std_char_t *debug_level);

/**
 * cmd_create_instance
 * @brief
 * @param   iid_string
 * @param   iid_string_len
 * @param   pp_handle
 * @param   init_args
 * @param   init_args_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_create_instance(IN const std_char_t *iid_string, IN std_size_t iid_string_len, INOUT std_void_t **pp_handle, IN const std_char_t *init_args, IN std_size_t init_args_len);

/**
 * cmd_delete_instance
 * @brief
 * @param   iid_string
 * @param   iid_string_len
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_delete_instance(IN const std_char_t *iid_string, IN std_size_t iid_string_len, INOUT std_void_t **pp_handle);


#endif
