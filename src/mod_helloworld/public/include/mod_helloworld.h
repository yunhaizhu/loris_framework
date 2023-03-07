/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_helloworld.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-03-07
 *
 */
#ifndef MOD_HELLOWORLD_H
#define MOD_HELLOWORLD_H

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

typedef struct mod_helloworld_st {
	mod_ownership_t ownership;
	std_u64_t unique_id;
	struct mod_helloworld_ops_st *p_ops;
} mod_helloworld_t;

struct mod_helloworld_ops_st {
	std_int_t(*init) (IN mod_helloworld_t * m, IN const std_char_t * arg, IN std_int_t arg_len);
	std_int_t(*cleanup) (IN mod_helloworld_t * m);

    /***func_define***/
	std_int_t(*say_hello) (IN mod_helloworld_t * m, IN std_char_t * str);

};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/

#define MOD_HELLOWORLD_IID MOD_GUID(0x5a087b81, 0x9070, 0xe1ee, 0x08, 0x3e, 0xe6, 0x8f, 0xa8, 0xe2, 0x82, 0x79)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/

#define mod_helloworld_init(m, arg, arg_len) ((m)->p_ops->init((mod_helloworld_t *)(m), arg, arg_len))
#define mod_helloworld_cleanup(m) ((m)->p_ops->cleanup((mod_helloworld_t *)(m)))

/***interface_define***/
#define mod_helloworld_say_hello(m, str) ((m)->p_ops->say_hello((mod_helloworld_t *)(m), str))

#endif
