/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_compile.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */
#ifndef MOD_LANG_COMPILE_H
#define MOD_LANG_COMPILE_H

#include "lang_ast.h"
#include "mod.h"
#include "mod_ownership.h"
#include "std_common.h"

/*****************************************************
 *                                                   *
 *                      Define                       *
 *                                                   *
 ****************************************************/

/***struct_define***/

/***macro_define***/

/****************************************************
 *                                                  *
 *                     MOD Define                   *
 *                                                  *
 ***************************************************/

typedef struct mod_lang_compile_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_lang_compile_ops_st *p_ops;
} mod_lang_compile_t;

struct mod_lang_compile_ops_st {
    std_int_t (*init)(IN mod_lang_compile_t *m, IN const std_char_t *arg, IN std_int_t arg_len);
    std_int_t (*cleanup)(IN mod_lang_compile_t *m);

    /***func_define***/
    std_void_t (*func)(IN mod_lang_compile_t *m, IN lang_ast_t *func_id, IN lang_ast_t *func_params,
                       IN lang_ast_t *func_body);
    std_void_t (*cmd)(IN mod_lang_compile_t *m, IN lang_ast_t *cmd);
    std_void_t (*expr)(IN mod_lang_compile_t *m, IN lang_ast_t *cmd);
    std_char_t *(*generated_bytecode)(IN mod_lang_compile_t *m, IN std_char_t *file_name);
    std_void_t (*reset) (IN mod_lang_compile_t * m);
};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/

#define MOD_LANG_COMPILE_IID MOD_GUID(0x88695ae1, 0xf662, 0x3356, 0xee, 0xef, 0xcc, 0x8b, 0x4c, 0x0e, 0xdd, 0xf9)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/

#define mod_lang_compile_init(m, arg, arg_len) ((m)->p_ops->init((mod_lang_compile_t *) (m), arg, arg_len))
#define mod_lang_compile_cleanup(m) ((m)->p_ops->cleanup((mod_lang_compile_t *) (m)))

/***interface_define***/
#define mod_lang_compile_func(m, func_id, func_params, func_body) ((m)->p_ops->func((mod_lang_compile_t *) (m), func_id, func_params, func_body))
#define mod_lang_compile_cmd(m, cmd) ((m)->p_ops->cmd((mod_lang_compile_t *) (m), cmd))
#define mod_lang_compile_expr(m, cmd) ((m)->p_ops->expr((mod_lang_compile_t *) (m), cmd))
#define mod_lang_compile_generated_bytecode(m, file_name) ((m)->p_ops->generated_bytecode((mod_lang_compile_t *) (m), file_name))
#define mod_lang_compile_reset(m) ((m)->p_ops->reset((mod_lang_compile_t *)(m)))

#endif
