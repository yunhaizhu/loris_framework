/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_compile_I.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */
#ifndef MOD_LANG_COMPILE_I_H
#define MOD_LANG_COMPILE_I_H

#include "mod_lang_compile.h"

typedef struct mod_lang_compile_imp_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_lang_compile_ops_st *p_ops;
    std_lock_free_key_hash_t *p_compiled_body;
} mod_lang_compile_imp_t;

/****shell_interface*****/


/****rpc_service_interface*****/

/**
 * mod_lang_compile_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  extern std_int_t
 */
extern std_int_t mod_lang_compile_I_create_instance(INOUT std_void_t **pp_handle);

#endif
