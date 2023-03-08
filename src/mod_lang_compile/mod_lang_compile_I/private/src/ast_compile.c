/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    ast_compile.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */

#include "ast_compile.h"
#include "ast_code.h"
#include "lang_ast.h"
#include "std_common.h"

static std_bool_t parse_no_error = STD_BOOL_TRUE;
environment_t global_gsl_rng_env[128];

extern gen_code_t gen_codes[MAX_GEN_CODES];
extern std_int_t n_code;

/**
 * compile_error
 * @brief   
 * @return  std_void_t
 */
std_void_t compile_error()
{
    parse_no_error = STD_BOOL_FALSE;
}
/**
 * declare_var_arg
 * @brief   
 * @param   Env
 * @param   i
 * @param   v
 * @return  static std_void_t
 */
static std_void_t declare_var_arg(const Environment *Env, std_int_t i, lang_ast_t *v, std_int_t line)
{
    if (v) {
        compile_expr(v);
        gen_codeI(STOREA, Env[i].pos, 0, line);
        gen_code(POP, line);
    }
}

/**
 * var_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   init_value_ast
 * @param   line
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t var_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *init_value_ast, std_int_t line)
{
    if (init_value_ast) {
        compile_expr(init_value_ast);
        gen_codeI(STOREL, Env[i].pos, 0, line);
        gen_code(POP, line);
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
std_void_t compile_declare_var(symbol_t *var, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            if (Env[i].var_kind == VAR_ARG) {
                declare_var_arg(Env, i, init_value_ast, line);
                return;
            } else if (Env[i].var_kind == VAR_LOCAL) {
                var_var_init_value(Env, i, init_value_ast, line);
                return;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_store_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
std_void_t compile_store_var(symbol_t *var, lang_ast_t *v, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    compile_expr(v);
    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(STOREA, Env[i].pos, 0, line);
                    gen_code(POP, line);
                    return;
                case VAR_LOCAL:
                    gen_codeI(STOREL, Env[i].pos, 0, line);
                    gen_code(POP, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_store_copy_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
std_void_t compile_store_copy_var(symbol_t *var, lang_ast_t *v, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    compile_expr(v);
    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(STOREA, Env[i].pos, 0, line);
                    gen_code(POP, line);
                    return;
                case VAR_LOCAL:
                    gen_codeI(STOREL_C, Env[i].pos, 0, line);
                    gen_code(POP, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_store_nocopy_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
std_void_t compile_store_nocopy_var(symbol_t *var, lang_ast_t *v, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    compile_expr(v);
    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(STOREA, Env[i].pos, 0, line);
                    gen_code(POP, line);
                    return;
                case VAR_LOCAL:
                    gen_codeI(STOREL_NC, Env[i].pos, 0, line);
                    gen_code(POP, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_load_var
 * @brief   
 * @param   var
 * @return  std_void_t
 */
std_void_t compile_load_var(symbol_t *var, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(LOADA, Env[i].pos, 0, line);
                    return;
                case VAR_LOCAL:
                    gen_codeI(LOADL, Env[i].pos, 0, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * array_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   item
 * @param   init_value_ast
 * @param   line
 * @param   count
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t array_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *item, lang_ast_t *init_value_ast, std_int_t line, std_int_t count)
{
    if (init_value_ast) {
        std_int_t index = 0;
        for (lang_ast_t *args = init_value_ast; args != NULL; args = get_lang_ast_next(args)) {
            STD_ASSERT_RV(index <= count, STD_RV_ERR_FAIL);

            item = get_lang_ast_first(args);
            gen_codeI(PUSHI, index, 0, line);

            compile_expr(item);
            gen_codeI(SET_ITEM, Env[i].pos, 0, line);
            index++;
        }
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_array_var
 * @brief   
 * @param   var
 * @param   size_ast
 * @param   init_value_ast
 * @return  std_void_t
 */
std_void_t compile_declare_array_var(symbol_t *var, IN const lang_ast_t *size_ast, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    lang_ast_t *item = NULL;
    std_int_t count;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            count = (std_int_t) get_lang_ast_number(size_ast);
            if (count <= 0) {
                STD_LOG(ERR, "array size can not be zero %d\n", count);
                compile_error();
                return;
            }
            gen_codeI(NEW_ARRAY, Env[i].pos, count, line);
            STD_ASSERT_RV(array_var_init_value(Env, i, item, init_value_ast, line, count) == STD_RV_SUC, );
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_get_item_var
 * @brief   
 * @param   var
 * @param   index_ast
 * @return  std_void_t
 */
std_void_t compile_get_item_var(symbol_t *var, lang_ast_t *index_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    compile_expr(index_ast);
                    gen_codeI(GET_ITEM, Env[i].pos, 1, line);
                    return;

                case VAR_LOCAL:
                    compile_expr(index_ast);
                    gen_codeI(GET_ITEM, Env[i].pos, 0, line);
                    return;

                default:
                    break;
            }
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_set_item_var
 * @brief   
 * @param   var
 * @param   index_ast
 * @param   value_ast
 * @return  std_void_t
 */
std_void_t compile_set_item_var(symbol_t *var, lang_ast_t *index_ast, lang_ast_t *value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(index_ast);
            compile_expr(value_ast);
            gen_codeI(SET_ITEM, Env[i].pos, 1, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(index_ast);
            compile_expr(value_ast);
            gen_codeI(SET_ITEM, Env[i].pos, 0, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_add_item_var
 * @brief   
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_add_item_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(item_ast);
            gen_codeI(ADD_ITEM, Env[i].pos, 1, line);
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(item_ast);
            gen_codeI(ADD_ITEM, Env[i].pos, 0, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_add_key_item_var
 * @brief   
 * @param   var
 * @param   index_ast
 * @param   value_ast
 * @return  std_void_t
 */
std_void_t compile_add_key_item_var(symbol_t *var, lang_ast_t *index_ast, lang_ast_t *value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(index_ast);
            compile_expr(value_ast);
            gen_codeI(ADD_KEY_ITEM, Env[i].pos, 1, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(index_ast);
            compile_expr(value_ast);
            gen_codeI(ADD_KEY_ITEM, Env[i].pos, 0, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_del_item_var
 * @brief   
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_del_item_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(item_ast);
            gen_codeI(DEL_ITEM, Env[i].pos, 1, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(item_ast);
            gen_codeI(DEL_ITEM, Env[i].pos, 0, line);
            return;
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * compile_count_item_var
 * @brief   
 * @param   var
 * @return  std_void_t
 */
std_void_t compile_count_item_var(symbol_t *var, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            gen_codeI(COUNT_ITEM, Env[i].pos, 1, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            gen_codeI(COUNT_ITEM, Env[i].pos, 0, line);
            return;
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_find_item_var
 * @brief   
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_find_item_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    compile_expr(item_ast);
                    gen_codeI(FIND_ITEM, Env[i].pos, 1, line);
                    return;

                case VAR_LOCAL:
                    compile_expr(item_ast);
                    gen_codeI(FIND_ITEM, Env[i].pos, 0, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * tuple_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   item
 * @param   init_value_ast
 * @param   line
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t tuple_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *item, lang_ast_t *init_value_ast, std_int_t line)
{
    if (init_value_ast) {
        for (lang_ast_t *args = init_value_ast; args != NULL; args = get_lang_ast_next(args)) {
            item = get_lang_ast_first(args);
            compile_expr(item);
            gen_codeI(ADD_ITEM, Env[i].pos, 0, line);
        }
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_tuple_var
 * @brief   
 * @param   var
 * @param   init_value_ast
 * @return  std_void_t
 */
std_void_t compile_declare_tuple_var(symbol_t *var, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    lang_ast_t *item = NULL;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            gen_codeI(NEW_LIST, Env[i].pos, 0, line);
            tuple_var_init_value(Env, i, item, init_value_ast, line);
            return;
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * hash_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   item
 * @param   init_value_ast
 * @param   line
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t hash_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *item, lang_ast_t *init_value_ast, std_int_t line)
{
    if (init_value_ast) {
        for (lang_ast_t *args = init_value_ast; args != NULL; args = get_lang_ast_next(args)) {
            item = get_lang_ast_first(args);
            lang_ast_t *key_ast = get_lang_ast_nth(item, 0);
            lang_ast_t *value_ast = get_lang_ast_nth(item, 1);

            STD_ASSERT_RV_WARN(key_ast != NULL && value_ast != NULL, STD_RV_ERR_FAIL);
            compile_expr(key_ast);
            compile_expr(value_ast);
            gen_codeI(ADD_KEY_ITEM, Env[i].pos, 0, line);
        }
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_hash_var
 * @brief   
 * @param   var
 * @param   init_value_ast
 * @return  std_void_t
 */
std_void_t compile_declare_hash_var(symbol_t *var, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    lang_ast_t *item = NULL;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            gen_codeI(NEW_KEY_HASH, Env[i].pos, 0, line);
            STD_ASSERT_RV_WARN(hash_var_init_value(Env, i, item, init_value_ast, line) == STD_RV_SUC, );
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * compile_sym
 * @brief   
 * @param   var
 * @return  std_void_t
 */
std_void_t compile_sym(symbol_t *var, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(SYM_A, Env[i].pos, 0, line);
                    return;
                case VAR_LOCAL:
                    gen_codeI(SYM_L, Env[i].pos, 0, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * define_function
 * @brief   
 * @param   fsym
 * @param   params
 * @param   body
 * @return  std_void_t
 */
std_void_t define_function(lang_ast_t *func_name, lang_ast_t *arg_params, lang_ast_t *body)
{
    symbol_t *fsym = get_lang_ast_symbol(func_name);
    lang_ast_t *params = arg_params;
    std_int_t param_pos;
    std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    symbol_t *last_symbol;

    if (parse_no_error == STD_BOOL_FALSE) {
        return;
    }

    gen_code_init();
    *envp = 0;
    param_pos = 0;
    global_gsl_rng_env[get_std_thread_id()].local_var_pos = 0;
    for (; params != NULL; params = get_lang_ast_next(params)) {
        Env[*envp].var = get_lang_ast_symbol(get_lang_ast_first(params));
        Env[*envp].var_kind = VAR_ARG;
        Env[*envp].pos = param_pos++;
        last_symbol = Env[*envp].var;

        gen_codeI(VAR_A, Env[*envp].pos, 0, get_lang_ast_first(params)->debug_info.line);
        (*envp)++;

        if (last_symbol->type_symbol) {
            Env[*envp].var = get_lang_ast_symbol(last_symbol->type_symbol->left);
            Env[*envp].var_kind = VAR_LOCAL;
            Env[*envp].pos = global_gsl_rng_env[get_std_thread_id()].local_var_pos++;

            gen_codeI(VAR_L, Env[*envp].pos, 0, get_lang_ast_first(params)->debug_info.line);
            (*envp)++;
            compile_expr(last_symbol->type_symbol);
        }

        if (last_symbol->check_block) {
            compile_statement(last_symbol->check_block);
        }
    }

    compile_statement(body);

    for (std_int_t i = 0; i < *envp; i++) {
        if (Env[i].var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(VAR_A_CLEAN, Env[i].pos, 0, 0);
                    break;
                case VAR_LOCAL:
                    gen_codeI(VAR_L_CLEAN, Env[i].pos, 0, 0);
                    break;
                default:
                    break;
            }
        }
    }

    gen_code_func(fsym->name, global_gsl_rng_env[get_std_thread_id()].local_var_pos, param_pos);

    *envp = 0; /* reset */

    if (parse_no_error) {
        clear_lang_ast(func_name);
        clear_lang_ast(arg_params);
        clear_lang_ast(body);
    }
}

/**
 * compile_statement
 * @brief   
 * @param   p
 * @return  std_void_t
 */
std_void_t compile_statement(lang_ast_t *p)
{
    if (p == NULL) {
        return;
    }

    switch (p->op) {
        case BLOCK_STATEMENT:
            compile_block(p->left, p->right);
            break;

        case RETURN_STATEMENT:
            compile_return(p->left);
            break;

        case BREAK_STATEMENT:
            compile_break(p);
            break;

        case IF_STATEMENT:
            compile_if(p->left, get_lang_ast_nth(p->right, 0), get_lang_ast_nth(p->right, 1));
            break;

        case WHILE_STATEMENT:
            compile_while(p->left, p->right);
            break;

        case FOR_STATEMENT:
            compile_for(get_lang_ast_nth(p->left, 0),
                        get_lang_ast_nth(p->left, 1),
                        get_lang_ast_nth(p->left, 2),
                        p->right);
            break;

        default:
            compile_expr(p);
    }
}

/**
 * compile_block
 * @brief   
 * @param   local_vars
 * @param   statements
 * @return  std_void_t
 */
std_void_t compile_block(lang_ast_t *local_vars, lang_ast_t *statements)
{
    std_int_t envp_save;
    std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    std_int_t *local_var_pos = &global_gsl_rng_env[get_std_thread_id()].local_var_pos;

    envp_save = *envp;
    global_gsl_rng_env[get_std_thread_id()].envp_save = envp_save;

    for (lang_ast_t *vars = local_vars; vars != NULL; vars = get_lang_ast_next(vars)) {
        if (get_lang_ast_first(vars) && get_lang_ast_first(vars)->left) {
            Env[*envp].var = get_lang_ast_symbol(get_lang_ast_first(vars)->left);
            Env[*envp].var_kind = VAR_LOCAL;
            Env[*envp].pos = (*local_var_pos)++;

            gen_codeI(VAR_L, Env[*envp].pos, 0, get_lang_ast_first(vars)->left->debug_info.line);
            (*envp)++;
        }
        compile_expr(get_lang_ast_first(vars));
    }

    for (; statements != NULL; statements = get_lang_ast_next(statements)) {
        compile_statement(get_lang_ast_first(statements));
    }

    for (std_int_t i = envp_save; i < *envp; i++) {
        if (Env[i].var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(VAR_A_CLEAN, Env[i].pos, 0, 0);
                    break;
                case VAR_LOCAL:
                    gen_codeI(VAR_L_CLEAN, Env[i].pos, 0, 0);
                    break;
                default:
                    break;
            }
        }
    }

    *envp = envp_save;
}

/**
 * inline_var_arg_local_clean
 * @brief   
 * @param   Env
 * @param   envp
 * @param   envp_save
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_var_arg_local_clean(const Environment *Env, const std_int_t *envp, std_int_t envp_save)
{
    for (std_int_t i = envp_save; i < *envp; i++) {
        if (Env[i].var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeI(VAR_A_CLEAN, Env[i].pos, 0, 0);
                    break;
                case VAR_LOCAL:
                    gen_codeI(VAR_L_CLEAN, Env[i].pos, 0, 0);
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * compile_return
 * @brief   
 * @param   expr
 * @return  std_void_t
 */
std_void_t compile_return(lang_ast_t *expr)
{
    std_int_t envp_save = 0;
    const std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    inline_var_arg_local_clean(Env, envp, envp_save);

    compile_expr(expr);
    gen_code(RET, expr ? expr->debug_info.line : 0);
}

/**
 * compile_break
 * @brief   
 * @param   expr
 * @return  std_void_t
 */
std_void_t compile_break(const lang_ast_t *expr)
{
    std_int_t for_break_l2 = global_gsl_rng_env[get_std_thread_id()].break_env[global_gsl_rng_env[get_std_thread_id()].break_env_index];
    std_int_t envp_save = global_gsl_rng_env[get_std_thread_id()].break_envp_save[global_gsl_rng_env[get_std_thread_id()].break_env_index];
    const std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    inline_var_arg_local_clean(Env, envp, envp_save);

    gen_codeI(JUMP, for_break_l2, 0, expr->debug_info.line);
}

/**
 * compile_call_func
 * @brief   
 * @param   f
 * @param   args
 * @param   line
 * @return  std_void_t
 */
std_void_t compile_call_func(const symbol_t *f, lang_ast_t *args, std_int_t line)
{
    std_int_t nargs;

    nargs = compile_args(args);

    gen_codeS(CALL, f->name, nargs, line);
    gen_codeI(POPR, nargs, 0, line);
}

/**
 * compile_args
 * @brief   
 * @param   args
 * @return  std_int_t
 */
std_int_t compile_args(lang_ast_t *args)
{
    std_int_t nargs;

    if (args != NULL) {
        nargs = compile_args(get_lang_ast_next(args));
        if (get_lang_ast_first(args)->op == SYMBOL_OP){
            compile_expr_sym(get_lang_ast_first(args));
        }else {
            compile_expr(get_lang_ast_first(args));
        }

        return nargs + 1;
    } else {
        return 0;
    }
}

/**
 * compile_if
 * @brief   
 * @param   cond
 * @param   then_part
 * @param   else_part
 * @return  std_void_t
 */
std_void_t compile_if(lang_ast_t *cond, lang_ast_t *then_part, lang_ast_t *else_part)
{
    std_int_t l1;
    std_int_t l2;
    std_int_t line;

    line = cond->debug_info.line;
    compile_expr(cond);
    l1 = global_gsl_rng_env[get_std_thread_id()].label_counter++;
    gen_codeI(BEQ0, l1, 0, line);

    line = then_part->debug_info.line;
    compile_statement(then_part);
    l2 = global_gsl_rng_env[get_std_thread_id()].label_counter++;

    if (else_part != NULL) {
        line = else_part->debug_info.line;
        gen_codeI(JUMP, l2, 0, line);
        gen_codeI(LABEL, l1, 0, line);
        compile_statement(else_part);
        gen_codeI(LABEL, l2, 0, line);
    } else {
        gen_codeI(LABEL, l1, 0, line);
    }
}

/**
 * compile_while
 * @brief   
 * @param   cond
 * @param   body
 * @return  std_void_t
 */
std_void_t compile_while(lang_ast_t *cond, lang_ast_t *body)
{
    std_int_t l1;
    std_int_t l2;
    std_int_t line;

    l1 = global_gsl_rng_env[get_std_thread_id()].label_counter++;
    l2 = global_gsl_rng_env[get_std_thread_id()].label_counter++;

    global_gsl_rng_env[get_std_thread_id()].break_env[++global_gsl_rng_env[get_std_thread_id()].break_env_index] = l2;
    global_gsl_rng_env[get_std_thread_id()].break_envp_save[global_gsl_rng_env[get_std_thread_id()].break_env_index] = global_gsl_rng_env[get_std_thread_id()].envp;

    line = cond->debug_info.line;
    gen_codeI(LABEL, l1, 0, line);
    compile_expr(cond);
    gen_codeI(BEQ0, l2, 0, line);

    line = body->debug_info.line;
    compile_statement(body);
    gen_codeI(JUMP, l1, 0, line);
    gen_codeI(LABEL, l2, 0, line);

    global_gsl_rng_env[get_std_thread_id()].break_env_index--;
}

/**
 * compile_for
 * @brief   
 * @param   init
 * @param   cond
 * @param   iter
 * @param   body
 * @return  std_void_t
 */
std_void_t compile_for(lang_ast_t *init, lang_ast_t *cond, lang_ast_t *iter, lang_ast_t *body)
{
    std_int_t l1;
    std_int_t l2;
    std_int_t line;

    l1 = global_gsl_rng_env[get_std_thread_id()].label_counter++;
    l2 = global_gsl_rng_env[get_std_thread_id()].label_counter++;

    global_gsl_rng_env[get_std_thread_id()].break_env[++global_gsl_rng_env[get_std_thread_id()].break_env_index] = l2;
    global_gsl_rng_env[get_std_thread_id()].break_envp_save[global_gsl_rng_env[get_std_thread_id()].break_env_index] = global_gsl_rng_env[get_std_thread_id()].envp;

    compile_expr(init);

    line = cond->debug_info.line;
    gen_codeI(LABEL, l1, 0, line);
    compile_expr(cond);
    gen_codeI(BEQ0, l2, 0, line);

    line = body->debug_info.line;
    compile_statement(body);
    compile_expr(iter);
    gen_codeI(JUMP, l1, 0, line);

    gen_codeI(LABEL, l2, 0, line);

    global_gsl_rng_env[get_std_thread_id()].break_env_index--;
}


/**
 * compile_command_statements
 * @brief   
 * @param   statements
 * @return  std_void_t
 */
std_void_t compile_command_statements(lang_ast_t *statements)
{
    if (parse_no_error == STD_BOOL_FALSE) {
        clear_lang_ast(statements);
        return;
    }

    for (lang_ast_t *statement = statements; statement != NULL;) {
        gen_code_init();
        gen_codeS(LOAD_LIB, "\"shell_lib\"", 0, 0);
        gen_code_expr();

        gen_code_init();
        compile_expr(get_lang_ast_first(statement));
        gen_code(POP, 0);

        if (get_lang_ast_first(statement)) {
            gen_code_func("function__main", 0, 0);
        }
        statement = get_lang_ast_next(statement);
    }

    clear_lang_ast(statements);
}

/**
 * check_call_function_assign
 * @brief   
 * @param   ast
 * @return  std_bool_t
 */
std_bool_t check_call_function_assign(lang_ast_t *ast)
{
    if (ast && ast->op == EQ_OP && ast->left->op == SYMBOL_OP && ast->right->op == CALL_OP) {
        return STD_BOOL_FALSE;
    }
    return STD_BOOL_TRUE;
}

/**
 * compile_expr_sym
 * @brief   
 * @param   p
 * @return  std_void_t
 */
std_void_t compile_expr_sym(lang_ast_t *p)
{
    if (p == NULL) {
        return;
    }

    switch (p->op) {
        case SYMBOL_OP:
            compile_sym(get_lang_ast_symbol(p), p->debug_info.line);
            return;
        default:
            break;
    }
}
/**
 * compile_expr
 * @brief   
 * @param   p
 * @return  std_void_t
 */
std_void_t compile_expr(lang_ast_t *p)
{
    lang_ast_t *item;
    std_int_t reg_id;

    if (p == NULL) {
        return;
    }

    switch (p->op) {
        case NUMBER_OP:
            if (p->number_type == NUMBER_TYPE_U64) {
                gen_codeU(PUSHU, get_lang_ast_u_number(p), 0, p->debug_info.line);
            } else {
                gen_codeI(PUSHI, get_lang_ast_number(p), 0, p->debug_info.line);
            }
            return;

        case ADDRESS_OP:
            gen_codeI(PUSHA, (intptr_t) get_lang_ast_address(p), 0, p->debug_info.line);
            return;

        case BOOL_OP:
            gen_codeI(PUSHI, get_lang_ast_bool(p), 1, p->debug_info.line);
            return;

        case STRING_OP:
            gen_codeS(PUSHS, p->string, 0, p->debug_info.line);
            return;

        case SYMBOL_OP:
            compile_load_var(get_lang_ast_symbol(p), p->debug_info.line);
//            compile_sym(get_lang_ast_symbol(p), p->debug_info.line);
            return;

        case EQ_OP:
        case SAFE_SET_OP:
            if (check_call_function_assign(p) == STD_BOOL_TRUE) {
                compile_store_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            } else {
                compile_error();
                STD_LOG(ERR, "Not passed check, please check line: %d\n", p->debug_info.line);
            }
            return;

        case EQ_COPY_OP:
            compile_store_copy_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case EQ_NOCOPY_OP:
            compile_store_nocopy_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case PLUS_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(ADD, p->debug_info.line);
            return;

        case Inp_PLUS_OP:
            if (p->right->op == NUMBER_OP) {
                compile_expr(p->left);

                if (gen_codes[n_code - 1].opcode == SYM_A) {
                    reg_id = (std_int_t)(3 + gen_codes[n_code - 1].operand);
                } else {
                    reg_id = (std_int_t)(11 + gen_codes[n_code - 1].operand);
                }
                gen_codeI(Inp_ADDI, p->right->number.i64, reg_id, p->debug_info.line);
            } else {
                compile_expr(p->left);
                compile_expr(p->right);

                if (gen_codes[n_code - 2].opcode == SYM_A) {
                    reg_id = (std_int_t)(3 + gen_codes[n_code - 2].operand);
                } else {
                    reg_id = (std_int_t)(11 + gen_codes[n_code - 2].operand);
                }
                gen_codeI(Inp_ADD, 0, reg_id, p->debug_info.line);
            }

            return;

        case MINUS_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(SUB, p->debug_info.line);
            return;

        case Inp_MINUS_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            if (gen_codes[n_code - 2].opcode == SYM_A) {
                reg_id = (std_int_t)(3 + gen_codes[n_code - 2].operand);
            } else {
                reg_id = (std_int_t)(11 + gen_codes[n_code - 2].operand);
            }
            gen_codeI(Inp_SUB, 0, reg_id, p->debug_info.line);
            return;

        case MUL_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(MUL, p->debug_info.line);
            return;

        case Inp_MUL_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            if (gen_codes[n_code - 2].opcode == SYM_A) {
                reg_id = (std_int_t)(3 + gen_codes[n_code - 2].operand);
            } else {
                reg_id = (std_int_t)(11 + gen_codes[n_code - 2].operand);
            }
            gen_codeI(Inp_MUL, 0, reg_id, p->debug_info.line);
            return;

        case DIV_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(DIV, p->debug_info.line);
            return;

        case Inp_DIV_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            if (gen_codes[n_code - 2].opcode == SYM_A) {
                reg_id = (std_int_t)(3 + gen_codes[n_code - 2].operand);
            } else {
                reg_id = (std_int_t)(11 + gen_codes[n_code - 2].operand);
            }
            gen_codeI(Inp_DIV, 0, reg_id, p->debug_info.line);
            return;

        case MOD_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(MOD, p->debug_info.line);
            return;

        case Inp_MOD_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            if (gen_codes[n_code - 2].opcode == SYM_A) {
                reg_id = (std_int_t)(3 + gen_codes[n_code - 2].operand);
            } else {
                reg_id = (std_int_t)(11 + gen_codes[n_code - 2].operand);
            }
            gen_codeI(Inp_MOD, 0, reg_id, p->debug_info.line);
            return;

        case LT_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(LT, p->debug_info.line);
            return;

        case GT_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(GT, p->debug_info.line);
            return;

        case EQUAL_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(EQ, p->debug_info.line);
            return;

        case NON_EQUAL_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(NEQ, p->debug_info.line);
            return;

        case LGE_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(LGE, p->debug_info.line);
            return;

        case BGE_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(BGE, p->debug_info.line);
            return;

        case AND_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(AND, p->debug_info.line);
            return;

        case OR_OP:
            compile_expr(p->left);
            compile_expr(p->right);
            gen_code(OR, p->debug_info.line);
            return;

        case CALL_OP:
            compile_call_func(get_lang_ast_symbol(p->left), p->right, p->left->debug_info.line);
            return;

        case DECLARE_VAR_OP:
            compile_declare_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case DECLARE_ARRAY_OP:
            compile_declare_array_var(get_lang_ast_symbol(p->left),
                                      get_lang_ast_nth(p->right, 0),
                                      get_lang_ast_nth(p->right, 1), p->debug_info.line);
            return;

        case DECLARE_TUPLE_OP:
            compile_declare_tuple_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case DECLARE_HASH_OP:
            compile_declare_hash_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case SAFE_GET_OP:
            compile_load_var(get_lang_ast_symbol(p->left), p->debug_info.line);
            return;

        case GET_ARRAY_OP:
        case GET_TUPLE_OP:
            compile_get_item_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case SET_ARRAY_OP:
            compile_set_item_var(get_lang_ast_symbol(get_lang_ast_nth(p->left, 0)),
                                 get_lang_ast_nth(p->left, 1),
                                 p->right, p->debug_info.line);
            return;

        case ADD_ITEM_OP:
            for (int i = 0;; ++i) {
                item = get_lang_ast_nth(p->right, i);
                if (item == NULL) {
                    break;
                }
                compile_add_item_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            }
            return;

        case ADD_KEY_ITEM_OP:
            compile_add_key_item_var(get_lang_ast_symbol(p->left),
                                     get_lang_ast_nth(p->right, 0),
                                     get_lang_ast_nth(p->right, 1), p->debug_info.line);
            return;

        case DEL_ITEM_OP:
            for (int i = 0;; ++i) {
                item = get_lang_ast_nth(p->right, i);
                if (item == NULL) {
                    break;
                }
                compile_del_item_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            }
            return;

        case FIND_ITEM_OP:
            item = get_lang_ast_nth(p->right, 0);
            compile_find_item_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            return;

        case COUNT_TUPLE_OP:
            compile_count_item_var(get_lang_ast_symbol(p->left), p->debug_info.line);
            return;

        case LOAD_LIB_OP:
            gen_code_init();
            gen_codeS(LOAD_LIB, get_lang_ast_string(p->left), 0, p->debug_info.line);
            gen_code_expr();
            clear_lang_ast(p);
            return;

        default:
            STD_LOG(DEBUG, "UNKNOWN OP\n");
    }
}

std_void_t compile_reset()
{
    global_gsl_rng_env[get_std_thread_id()].label_counter = 0;
}