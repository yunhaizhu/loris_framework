/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_helloworld_I.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-03-07
 *
 */
#ifndef MOD_HELLOWORLD_I_H
#define MOD_HELLOWORLD_I_H

#include "mod_shell.h"
#include "mod_helloworld.h"

typedef struct mod_helloworld_imp_st {
	mod_ownership_t ownership;
	std_u64_t unique_id;
	struct mod_helloworld_ops_st *p_ops;
	mod_shell_t *p_mod_shell;
} mod_helloworld_imp_t;

/****shell_interface*****/

/**
 * shell_stub_mod_helloworld_I_say_hello
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  extern STD_CALL std_char_t *
 */
extern STD_CALL std_char_t *shell_stub_mod_helloworld_I_say_hello(IN std_void_t * p_handle, IN std_char_t * params);

/****rpc_service_interface*****/

/**
 * mod_helloworld_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  extern std_int_t
 */
extern std_int_t mod_helloworld_I_create_instance(INOUT std_void_t ** pp_handle);

#endif
