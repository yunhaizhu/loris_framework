/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_parse_I.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */
#ifndef MOD_LANG_PARSE_I_H
#define MOD_LANG_PARSE_I_H

#include "lang_lex.h"
#include "mod_lang_compile.h"
#include "mod_lang_parse.h"

typedef struct mod_lang_parse_imp_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_lang_parse_ops_st *p_ops;
    mod_lang_compile_t *p_mod_compile;
    lang_state_t lang_state;
} mod_lang_parse_imp_t;


/****rpc_service_interface*****/

/**
 * mod_lang_parse_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  extern std_int_t
 */
extern std_int_t mod_lang_parse_I_create_instance(INOUT std_void_t **pp_handle);

#endif
