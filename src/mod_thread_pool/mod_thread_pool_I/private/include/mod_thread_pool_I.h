/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_thread_pool_I.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-20
 *
 */
#ifndef MOD_THREAD_POOL_I_H
#define MOD_THREAD_POOL_I_H

#include "mod_shell.h"
#include "mod_thread_pool.h"
#include "thpool.h"

typedef struct mod_thread_pool_imp_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_thread_pool_ops_st *p_ops;
    mod_shell_t *p_mod_shell;
    threadpool thread_pool;
} mod_thread_pool_imp_t;

/****shell_interface*****/

/**
 * shell_stub_mod_thread_pool_I_add_work
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  extern std_char_t *
 */
extern std_char_t *shell_stub_mod_thread_pool_I_add_work(std_void_t *p_handle, std_char_t *params);

/**
 * shell_stub_rpc_mod_thread_pool_I_add_work
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  extern std_char_t *
 */
extern std_char_t *shell_stub_rpc_mod_thread_pool_I_add_work(std_void_t *p_handle, std_char_t *params);

/**
 * mod_thread_pool_I_create_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @return  extern STD_CALL std_int_t
 */
extern STD_CALL std_int_t mod_thread_pool_I_create_instance(INOUT std_void_t **pp_handle);

#endif
