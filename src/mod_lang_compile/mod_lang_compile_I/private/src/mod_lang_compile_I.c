/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_compile_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */

#include "mod_lang_compile_I.h"
#include "ast_code.h"
#include "ast_compile.h"
#include "xxh3.h"

/**
 * mod_lang_compile_I_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_I_init(IN mod_lang_compile_t *p_m, IN const std_char_t *arg, IN std_int_t arg_len)
{
    mod_lang_compile_imp_t *p_imp_m = (mod_lang_compile_imp_t *) p_m;

    p_imp_m->p_compiled_body = std_lock_free_key_hash_create(128);

    gen_buffer_reset();
    return STD_RV_SUC;
}

/**
 * mod_lang_compile_I_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_I_cleanup(mod_lang_compile_t *p_m)
{
    mod_lang_compile_imp_t *p_imp_m = (mod_lang_compile_imp_t *) p_m;

    std_lock_free_key_hash_value_destroy(p_imp_m->p_compiled_body);

    return STD_RV_SUC;
}

/***func_implementation***/

/**
 * mod_lang_compile_I_func
 * @brief   
 * @param   p_m
 * @param   func_id
 * @param   func_params
 * @param   func_body
 * @return  STD_CALL             std_void_t
 */
STD_CALL std_void_t mod_lang_compile_I_func(IN mod_lang_compile_t *p_m, IN lang_ast_t *func_id,
                                            IN lang_ast_t *func_params, IN lang_ast_t *func_body)
{
    define_function(func_id, func_params, func_body);
}

/**
 * mod_lang_compile_I_cmd
 * @brief   
 * @param   p_m
 * @param   cmd
 * @return  STD_CALL             std_void_t
 */
STD_CALL std_void_t mod_lang_compile_I_cmd(IN mod_lang_compile_t *p_m, IN lang_ast_t *cmd)
{
    compile_command_statements(cmd);
}

/**
 * mod_lang_compile_I_expr
 * @brief   
 * @param   p_m
 * @param   cmd
 * @return  STD_CALL             std_void_t
 */
STD_CALL std_void_t mod_lang_compile_I_expr(IN mod_lang_compile_t *p_m, IN lang_ast_t *expr)
{
    compile_expr(expr);
}

/**
 * mod_lang_compile_I_generated_bytecode
 * @brief   
 * @param   p_m
 * @param   file_name
 * @return  STD_CALL             std_char_t  *
 */
STD_CALL std_char_t *mod_lang_compile_I_generated_bytecode(IN mod_lang_compile_t *p_m, IN std_char_t *file_name)
{
    std_char_t *compiled_body;
    mod_lang_compile_imp_t *p_imp_m = (mod_lang_compile_imp_t *) p_m;
    std_u64_t u64_key;
    std_size_t buf_len;

    compiled_body = std_lock_free_key_hash_find(p_imp_m->p_compiled_body, file_name, std_safe_strlen(file_name, BUF_SIZE_128));
    if (compiled_body == NULL) {
        compiled_body = (std_char_t *) CALLOC(1, BUF_SIZE_8192 * 10);
        std_lock_free_key_hash_add(p_imp_m->p_compiled_body, file_name, std_safe_strlen(file_name, BUF_SIZE_128), compiled_body);
    } else {
        memset(compiled_body, 0, BUF_SIZE_8192 * 10);
    }
    gen_buffer_output(compiled_body, BUF_SIZE_8192 * 10);

    gen_buffer_reset();

    buf_len = std_safe_strlen(compiled_body, BUF_SIZE_8192 * 10);
    u64_key = XXH64(compiled_body, sizeof(char) * buf_len, 0);
    u64_key += VERSION_NUMBER;

    std_char_t body_file_name[BUF_SIZE_128] = "\0";
    snprintf(body_file_name, sizeof(body_file_name), "dynamic_exec/src/execute_compiled_code_%s-%lu.body", file_name, u64_key);
    FILE *fp = fopen(body_file_name, "w+");
    if (fp) {
        fwrite(compiled_body, buf_len, 1, fp);
        fclose(fp);
    }

    return compiled_body;
}

/**
 * mod_lang_compile_I_reset
 * @brief
 * @param   p_m
 * @return  STD_CALL             std_void_t
 */
STD_CALL std_void_t mod_lang_compile_I_reset(IN mod_lang_compile_t * p_m)
{
    compile_reset();
}

struct mod_lang_compile_ops_st mod_lang_compile_I_ops = {
        mod_lang_compile_I_init,
        mod_lang_compile_I_cleanup,

        /***func_ops***/
        mod_lang_compile_I_func,
        mod_lang_compile_I_cmd,
        mod_lang_compile_I_expr,
        mod_lang_compile_I_generated_bytecode,
        mod_lang_compile_I_reset,
};

/**
 * mod_lang_compile_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_I_create_instance(INOUT std_void_t **pp_handle)
{
    mod_lang_compile_imp_t *p_m = NULL;

    p_m = (mod_lang_compile_imp_t *) CALLOC(1, sizeof(mod_lang_compile_imp_t));
    p_m->unique_id = std_random_u64();
    p_m->p_ops = &mod_lang_compile_I_ops;

    mod_ownership_register_ops((mod_ownership_t *) p_m);
    mod_ownership_init((mod_ownership_t *) p_m);
    *pp_handle = p_m;

    return STD_RV_SUC;
}
