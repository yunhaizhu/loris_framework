/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_parse.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */

#ifndef NONCLEDB_LANG_PARSE_H
#define NONCLEDB_LANG_PARSE_H

#include "lang_ast.h"
#include "lang_lex.h"

/**
 * lang_parse
 * @brief   
 * @param   state
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  lang_ast_t *
 */
lang_ast_t *lang_parse(lang_state_t *state, IN std_char_t *file_name, IN std_char_t *source_buffer, IN std_int_t source_buffer_len);

#endif
