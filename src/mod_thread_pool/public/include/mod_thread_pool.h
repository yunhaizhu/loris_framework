/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_thread_pool.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-20
 *
 */

#ifndef MOD_THREAD_POOL_H
#define MOD_THREAD_POOL_H

#include "mod.h"
#include "mod_ownership.h"
#include "std_common.h"

/*****************************************************
 *                                                   *
 *                      Define                       *
 *                                                   *
 ****************************************************/

/***struct_define***/
typedef std_void_t (*thread_callback_t)(std_void_t *params);

/***macro_define***/

/****************************************************
 *                                                  *
 *                     MOD Define                   *
 *                                                  *
 ***************************************************/

typedef struct mod_thread_pool_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_thread_pool_ops_st *p_ops;
} mod_thread_pool_t;

struct mod_thread_pool_ops_st {
    std_int_t (*init)(IN mod_thread_pool_t *m, IN const std_char_t *arg, IN std_int_t arg_len);
    std_int_t (*cleanup)(IN mod_thread_pool_t *m);

    /***func_define***/
    std_int_t (*add_work)(IN mod_thread_pool_t *m, IN thread_callback_t task_func, IN std_void_t *data);
};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/
#define MOD_THREAD_POOL_IID MOD_GUID(0x390d860f, 0x1ade, 0x2fc4, 0x1d, 0xcf, 0x11, 0xbb, 0xd8, 0x8e, 0xc6, 0xd3)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/
#define mod_thread_pool_init(m, arg, arg_len) ((m)->p_ops->init((mod_thread_pool_t *) (m), arg, arg_len))
#define mod_thread_pool_cleanup(m) ((m)->p_ops->cleanup((mod_thread_pool_t *) (m)))

/***interface_define***/
#define mod_thread_pool_add_work(m, task_func, data) ((m)->p_ops->add_work((mod_thread_pool_t *) (m), task_func, data))

#endif
