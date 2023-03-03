/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_parse_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */

#include "mod_lang_parse_I.h"
#include "lang_parse.h"

mod_lang_compile_t *p_global_mod_compile;

/**
 * mod_lang_parse_I_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_I_init(IN mod_lang_parse_t *p_m, IN const std_char_t *arg, IN std_int_t arg_len)
{
    mod_lang_parse_imp_t *p_imp_m = (mod_lang_parse_imp_t *) p_m;
    mod_iid_t mod_lang_compile_iid = MOD_LANG_COMPILE_IID;

    mod_create_instance(&mod_lang_compile_iid, (std_void_t **) &p_imp_m->p_mod_compile,
                        (mod_ownership_t *) p_imp_m);
    mod_lang_compile_init(p_imp_m->p_mod_compile, NULL, 0);

    p_global_mod_compile = p_imp_m->p_mod_compile;

    return STD_RV_SUC;
}

/**
 * mod_lang_parse_I_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_I_cleanup(mod_lang_parse_t *p_m)
{
    mod_lang_parse_imp_t *p_imp_m = (mod_lang_parse_imp_t *) p_m;
    mod_iid_t mod_lang_compile_iid = MOD_LANG_COMPILE_IID;

    mod_lang_compile_cleanup(p_imp_m->p_mod_compile);
    mod_delete_instance(&mod_lang_compile_iid, (std_void_t **) &p_imp_m->p_mod_compile,
                        (mod_ownership_t *) p_imp_m);
    p_global_mod_compile = NULL;

    return STD_RV_SUC;
}

/***func_implementation***/

/**
 * mod_lang_parse_I_parse
 * @brief   
 * @param   p_m
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  STD_CALL             std_int_t
 */
STD_CALL std_int_t mod_lang_parse_I_parse(IN mod_lang_parse_t *p_m, IN std_char_t *file_name,
                                          IN std_char_t *source_buffer, IN std_int_t source_buffer_len)
{
    mod_lang_parse_imp_t *p_imp = (mod_lang_parse_imp_t *) p_m;

    lang_parse(&p_imp->lang_state, file_name, source_buffer, source_buffer_len);

    return 0;
}

struct mod_lang_parse_ops_st mod_lang_parse_I_ops = {
        mod_lang_parse_I_init,
        mod_lang_parse_I_cleanup,

        /***func_ops***/
        mod_lang_parse_I_parse,

};

/**
 * mod_lang_parse_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_I_create_instance(INOUT std_void_t **pp_handle)
{
    mod_lang_parse_imp_t *p_m = NULL;

    p_m = (mod_lang_parse_imp_t *) CALLOC(1, sizeof(mod_lang_parse_imp_t));
    p_m->unique_id = std_random_u64();
    p_m->p_ops = &mod_lang_parse_I_ops;

    mod_ownership_register_ops((mod_ownership_t *) p_m);
    mod_ownership_init((mod_ownership_t *) p_m);
    *pp_handle = p_m;

    return STD_RV_SUC;
}
