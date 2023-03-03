/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_shell_I.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-18
 *
 */
#ifndef MOD_SHELL_I_H
#define MOD_SHELL_I_H

#include "mod_lang_compile.h"
#include "mod_lang_parse.h"
#include "mod_lang_vm.h"
#include "mod_shell.h"
#include "mod_thread_pool.h"
#include "std_lock_free_key_hash.h"

typedef struct mod_shell_imp_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_shell_ops_st *p_ops;
    std_lock_free_key_hash_t *hash_head;

    mod_thread_pool_t *p_mod_thread_pool;
    mod_lang_parse_t *p_mod_lang_parse;
    mod_lang_compile_t *p_mod_lang_compile;
    mod_lang_vm_t *p_mod_lang_vm;
} mod_shell_imp_t;

typedef struct shell_func_node_st {
    std_char_t func_name[BUF_SIZE_128];
    std_int_t func_name_len;
    shell_stub_callback_t func_callback;
    std_void_t *module_handle;
} shell_func_node_t;

/**
 * mod_shell_I_create_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @return  extern STD_CALL std_rv_t
 */
extern STD_CALL std_rv_t mod_shell_I_create_instance(INOUT std_void_t **pp_handle);

#endif
