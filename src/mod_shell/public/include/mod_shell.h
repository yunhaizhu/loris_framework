/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_shell.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-18
 *
 */
#ifndef MOD_SHELL_H
#define MOD_SHELL_H

#include "mod.h"
#include "mod_ownership.h"
#include "std_common.h"

/*****************************************************
 *                                                   *
 *                      Define                       *
 *                                                   *
 ****************************************************/

/***struct_define***/
typedef std_char_t *(*shell_stub_callback_t)(std_void_t *p_handle, std_char_t *params);

/***macro_define***/

/****************************************************
 *                                                  *
 *                     MOD Define                   *
 *                                                  *
 ***************************************************/

typedef struct mod_shell_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_shell_ops_st *p_ops;
} mod_shell_t;

struct mod_shell_ops_st {
    std_int_t (*init)(IN mod_shell_t *m, IN const std_char_t *arg, IN std_int_t arg_len);
    std_int_t (*cleanup)(IN mod_shell_t *m);

    /***func_define***/
    std_int_t (*regist)(IN mod_shell_t *m, IN const std_char_t *name, IN std_int_t name_len, IN shell_stub_callback_t func, IN std_void_t *p_handle);
    std_int_t (*unregister)(IN mod_shell_t *m, IN const std_char_t *name, IN std_int_t name_len);
    std_int_t (*shell)(IN mod_shell_t *m);
};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/
#define MOD_SHELL_IID MOD_GUID(0x3041c6f2, 0x7fbc, 0xc65e, 0x66, 0xe4, 0x99, 0x80, 0xcf, 0x64, 0x7f, 0x21)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/

#define mod_shell_init(m, arg, arg_len) ((m)->p_ops->init((mod_shell_t *) (m), arg, arg_len))
#define mod_shell_cleanup(m) ((m)->p_ops->cleanup((mod_shell_t *) (m)))

/***interface_define***/

#define mod_shell_register(m, name, name_len, func, handle) ((m)->p_ops->regist((mod_shell_t *) (m), name, name_len, func, handle))
#define mod_shell_unregister(m, name, name_len) ((m)->p_ops->unregister((mod_shell_t *) (m), name, name_len))
#define mod_shell_shell(m) ((m)->p_ops->shell((mod_shell_t *) (m)))

#endif
