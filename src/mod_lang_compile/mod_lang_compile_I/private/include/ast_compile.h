/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    ast_compile.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-21
 *
 */
#ifndef NONCLEDB_AST_COMPILE_H
#define NONCLEDB_AST_COMPILE_H

#include "lang_ast.h"
#include <gsl/gsl_rng.h>
#include <setjmp.h>

#define MAX_ENV 100

#define VAR_ARG 0
#define VAR_LOCAL 1

typedef struct env {
    symbol_t *var;
    std_int_t var_kind;
    std_int_t var_type;
    std_int_t pos;
} Environment;

typedef struct environment_s {
    std_int_t envp_save;
    std_int_t envp;
    Environment Env[MAX_ENV];
    std_int_t label_counter;
    std_int_t local_var_pos;

    std_int_t break_env[10];
    std_int_t break_envp_save[10];
    std_int_t break_env_index;
} environment_t;

/**
 * define_function
 * @brief   
 * @param   fsym
 * @param   params
 * @param   body
 * @return  std_void_t
 */
STD_CALL std_void_t define_function(IN lang_ast_t *func_name, IN lang_ast_t *params, IN lang_ast_t *body);


/**
 * compile_command_statements
 * @brief   
 * @param   statements
 * @return  std_void_t
 */
STD_CALL std_void_t compile_command_statements(IN lang_ast_t *statements);

/**
 * compile_declare_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
STD_CALL std_void_t compile_declare_var(IN symbol_t *var, IN lang_ast_t *v, std_int_t line);


/**
 * compile_declare_array_var
 * @brief   
 * @param   var
 * @param   size_ast
 * @param   init_value_ast
 * @return  std_void_t
 */
STD_CALL std_void_t compile_declare_array_var(IN symbol_t *var, IN const lang_ast_t *size_ast, IN lang_ast_t *init_value_ast, std_int_t line);


/**
 * compile_declare_tuple_var
 * @brief   
 * @param   var
 * @param   init_value_ast
 * @return  std_void_t
 */
STD_CALL std_void_t compile_declare_tuple_var(IN symbol_t *var, IN lang_ast_t *init_value_ast, std_int_t line);


/**
 * compile_declare_hash_var
 * @brief   
 * @param   var
 * @param   init_value_ast
 * @return  std_void_t
 */
STD_CALL std_void_t compile_declare_hash_var(IN symbol_t *var, IN lang_ast_t *init_value_ast, std_int_t line);


/**
 * compile_store_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
STD_CALL std_void_t compile_store_var(IN symbol_t *var, IN lang_ast_t *v, std_int_t line);


/**
 * compile_load_var
 * @brief   
 * @param   var
 * @return  std_void_t
 */
STD_CALL std_void_t compile_load_var(IN symbol_t *var, std_int_t line);


/**
 * compile_statement
 * @brief   
 * @param   p
 * @return  std_void_t
 */
STD_CALL std_void_t compile_statement(IN lang_ast_t *p);


/**
 * compile_block
 * @brief   
 * @param   local_vars
 * @param   statements
 * @return  std_void_t
 */
STD_CALL std_void_t compile_block(IN lang_ast_t *local_vars, IN lang_ast_t *statements);


/**
 * compile_return
 * @brief   
 * @param   expr
 * @return  std_void_t
 */
STD_CALL std_void_t compile_return(IN lang_ast_t *expr);


/**
 * compile_break
 * @brief   
 * @param   expr
 * @return  std_void_t
 */
STD_CALL std_void_t compile_break(IN const lang_ast_t *expr);


/**
 * compile_call_func
 * @brief   
 * @param   f
 * @param   args
 * @param   line
 * @return  std_void_t
 */
STD_CALL std_void_t compile_call_func(IN const symbol_t *f, IN lang_ast_t *args, IN std_int_t line);


/**
 * compile_args
 * @brief   
 * @param   args
 * @return  std_int_t
 */
STD_CALL std_int_t compile_args(IN lang_ast_t *args);


/**
 * compile_if
 * @brief   
 * @param   cond
 * @param   then_part
 * @param   else_part
 * @return  std_void_t
 */
STD_CALL std_void_t compile_if(IN lang_ast_t *cond, IN lang_ast_t *then_part, IN lang_ast_t *else_part);


/**
 * compile_while
 * @brief   
 * @param   cond
 * @param   body
 * @return  std_void_t
 */
STD_CALL std_void_t compile_while(IN lang_ast_t *cond, IN lang_ast_t *body);


/**
 * compile_for
 * @brief   
 * @param   init
 * @param   cond
 * @param   iter
 * @param   body
 * @return  std_void_t
 */
STD_CALL std_void_t compile_for(IN lang_ast_t *init, IN lang_ast_t *cond, IN lang_ast_t *iter, IN lang_ast_t *body);


/**
 * compile_expr
 * @brief   
 * @param   p
 * @return  std_void_t
 */
STD_CALL std_void_t compile_expr(IN lang_ast_t *p);


/**
 * compile_sym
 * @brief   
 * @param   var
 * @return  std_void_t
 */
STD_CALL std_void_t compile_sym(IN symbol_t *var, std_int_t line);

/**
 * compile_expr_sym
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t compile_expr_sym(lang_ast_t *p);

/**
 * compile_reset
 * @brief
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t compile_reset();

#endif
