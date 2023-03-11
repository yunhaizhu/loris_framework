/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_ast.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */
//
// Created by yun on 12/22/21.
//

#ifndef NONCLEDB_LANG_AST_H
#define NONCLEDB_LANG_AST_H

#include "rsa.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "std_lock_free_list.h"

typedef enum lang_ast_code {
    NUMBER_OP,
    STRING_OP,
    ADDRESS_OP,
    BOOL_OP,

    LIST_OP,
    SYMBOL_OP,

    EQ_OP,

    PLUS_OP,
    Inp_PLUS_OP,
    MINUS_OP,
    Inp_MINUS_OP,
    MUL_OP,
    Inp_MUL_OP,
    DIV_OP,
    Inp_DIV_OP,
    MOD_OP,
    Inp_MOD_OP,
    LT_OP,
    GT_OP,
    GET_ARRAY_OP,
    SET_ARRAY_OP,

    DECLARE_VAR_OP,
    DECLARE_TUPLE_OP,
    DECLARE_HASH_OP,
    DECLARE_ARRAY_OP,
    GET_TUPLE_OP,
    COUNT_TUPLE_OP,
    ADD_ITEM_OP,
    ADD_KEY_ITEM_OP,
    DEL_ITEM_OP,
    FIND_ITEM_OP,

    SAFE_SET_OP,
    SAFE_GET_OP,

    EQUAL_OP,
    NON_EQUAL_OP,
    LGE_OP,
    BGE_OP,

    AND_OP,
    OR_OP,

    CALL_OP,
    LOAD_LIB_OP,

    IF_STATEMENT,
    BLOCK_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT
} lang_ast_code_t;

typedef enum number_type_s {
    NUMBER_TYPE_U64,
    NUMBER_TYPE_I64,
} number_type_t;

typedef struct debug_info_s {
    std_char_t *source_file;
    std_int_t line;
} debug_info_t;

typedef struct lang_ast_s {
    lang_ast_code_t op;

    struct symbol_s *symbol;
    struct lang_ast_s *left, *right;

    number_type_t number_type;
    union {
        std_u64_t u64;
        std_64_t i64;
    } number;
    std_char_t *string;
    std_void_t *address;
    std_bool_t bool_value;

    debug_info_t debug_info;
} lang_ast_t;

typedef struct symbol_s {
    std_char_t *name;
    lang_ast_t *type_symbol;
    lang_ast_t *check_block;
} symbol_t;

/**
 * make_lang_ast_symbol
 * @brief   
 * @param   name
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_symbol(IN std_char_t *name, IN std_char_t *source_file, IN std_int_t line);


/**
 * lookup_lang_ast_symbol
 * @brief   
 * @param   name
 * @param   check_error
 * @return  STD_CALL symbol_t *
 */
STD_CALL symbol_t *lookup_lang_ast_symbol(IN std_char_t *name, IN std_bool_t check_error);
/**
 * get_lang_ast_symbol
 * @brief   
 * @param   ast
 * @return  STD_CALL symbol_t *
 */
STD_CALL symbol_t *get_lang_ast_symbol(IN lang_ast_t *ast);

/**
 * make_lang_ast_bool
 * @brief   
 * @param   bool_value
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_bool(IN std_bool_t bool_value, IN std_char_t *source_file, IN std_int_t line);
/**
 * make_lang_ast_number
 * @brief   
 * @param   number
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_number(IN std_64_t number, IN std_char_t *source_file, IN std_int_t line);
/**
 * make_lang_ast_u_number
 * @brief   
 * @param   number
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_u_number(IN std_u64_t number, IN std_char_t *source_file, IN std_int_t line);
/**
 * make_lang_ast_string
 * @brief   
 * @param   string
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_string(IN std_char_t *string, IN std_char_t *source_file, IN std_int_t line);
/**
 * make_lang_ast_address
 * @brief   
 * @param   address
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_address(IN std_void_t *address, IN std_char_t *source_file, IN std_int_t line);

/**
 * callback_destroy_lang_symbol
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t callback_destroy_lang_symbol(IN std_void_t *data, IN const std_void_t *callback_arg);

/**
 * callback_destroy_lang_ast
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t callback_destroy_lang_ast(IN std_void_t *data, IN const std_void_t *callback_arg);
/**
 * get_lang_ast_symbol_name
 * @brief   
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_symbol_name(IN lang_ast_t *ast);
/**
 * get_lang_ast_bool
 * @brief   
 * @param   ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t get_lang_ast_bool(IN const lang_ast_t *ast);
/**
 * get_lang_ast_number
 * @brief   
 * @param   ast
 * @return  STD_CALL std_64_t
 */
STD_CALL std_64_t get_lang_ast_number(IN const lang_ast_t *ast);

/**
 * get_lang_ast_u_number
 * @brief   
 * @param   ast
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t get_lang_ast_u_number(IN const lang_ast_t *ast);
/**
 * get_lang_ast_string
 * @brief   
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_string(IN lang_ast_t *ast);
/**
 * get_lang_ast_address
 * @brief   
 * @param   ast
 * @return  STD_CALL std_void_t *
 */
STD_CALL std_void_t *get_lang_ast_address(IN lang_ast_t *ast);

/**
 * make_ast
 * @brief   
 * @param   op
 * @param   left
 * @param   right
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast(IN lang_ast_code_t op, IN lang_ast_t *left, IN lang_ast_t *right, IN std_char_t *source_file, IN std_int_t line);

#define make_lang_ast_list1(x1, s, l) make_lang_ast(LIST_OP, x1, NULL, s, l)
#define make_lang_ast_list2(x1, x2, s, l) make_lang_ast(LIST_OP, x1, make_lang_ast(LIST_OP, x2, NULL, s, l), s, l)
#define make_lang_ast_list3(x1, x2, x3, s, l) make_lang_ast(LIST_OP, x1, make_lang_ast(LIST_OP, x2, make_lang_ast(LIST_OP, x3, NULL, s, l), s, l), s, l)

#define get_lang_ast_first(ast) get_lang_ast_nth(ast, 0)
/**
 * get_lang_ast_nth
 * @brief   
 * @param   ast
 * @param   nth
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *get_lang_ast_nth(IN lang_ast_t *ast, IN std_int_t nth);
/**
 * get_lang_ast_next
 * @brief   
 * @param   ast
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *get_lang_ast_next(IN lang_ast_t *ast);
/**
 * add_lang_ast_last
 * @brief   
 * @param   last
 * @param   ast
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *add_lang_ast_last(IN lang_ast_t *last, IN lang_ast_t *ast);

/**
 * print_lang_ast
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_lang_ast(IN lang_ast_t *p);

/**
 * clear_lang_ast
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t clear_lang_ast(IN lang_ast_t *p);

/**
 * add_require_func
 * @brief
 * @param   func_name
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t add_require_func(std_char_t *func_name);

#endif//NONCLEDB_LANG_AST_H
