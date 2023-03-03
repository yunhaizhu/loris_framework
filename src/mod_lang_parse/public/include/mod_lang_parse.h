/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_parse.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */
#ifndef MOD_LANG_PARSE_H
#define MOD_LANG_PARSE_H

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

typedef struct mod_lang_parse_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_lang_parse_ops_st *p_ops;
} mod_lang_parse_t;

struct mod_lang_parse_ops_st {
    std_int_t (*init)(IN mod_lang_parse_t *m, IN const std_char_t *arg, IN std_int_t arg_len);
    std_int_t (*cleanup)(IN mod_lang_parse_t *m);

    /***func_define***/
    std_int_t (*parse)(IN mod_lang_parse_t *m, IN std_char_t *file_name, IN std_char_t *source_buffer,
                       IN std_int_t source_buffer_len);
};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/

#define MOD_LANG_PARSE_IID MOD_GUID(0xa0bb6965, 0x8e6c, 0xf1f6, 0x5c, 0x80, 0x4a, 0x2d, 0x38, 0xde, 0x86, 0xba)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/

#define mod_lang_parse_init(m, arg, arg_len) ((m)->p_ops->init((mod_lang_parse_t *) (m), arg, arg_len))
#define mod_lang_parse_cleanup(m) ((m)->p_ops->cleanup((mod_lang_parse_t *) (m)))

/***interface_define***/
#define mod_lang_parse_parse(m, file_name, source_buffer, source_buffer_len) ((m)->p_ops->parse((mod_lang_parse_t *) (m), file_name, source_buffer, source_buffer_len))

#endif
