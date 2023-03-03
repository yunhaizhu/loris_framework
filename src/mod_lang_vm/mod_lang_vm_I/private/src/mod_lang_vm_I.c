/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_vm_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */

#include "mod_lang_vm_I.h"
#include "virtual_machine.h"

/**
 * mod_lang_vm_I_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_vm_I_init(IN mod_lang_vm_t *p_m, IN const std_char_t *arg, IN std_int_t arg_len)
{
    //	mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *) p_m;

    return STD_RV_SUC;
}

/**
 * mod_lang_vm_I_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_vm_I_cleanup(mod_lang_vm_t *p_m)
{
    //	mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *) p_m;

    return STD_RV_SUC;
}

/***func_implementation***/

/**
 * mod_lang_vm_I_run_init
 * @brief   
 * @param   p_m
 * @param   file_name
 * @param   bytecode_buffer
 * @return  STD_CALL             std_int_t
 */
STD_CALL std_int_t mod_lang_vm_I_run_init(IN mod_lang_vm_t *p_m, IN std_char_t *file_name,
                                          IN std_char_t *bytecode_buffer)
{
    return vm_init(file_name, bytecode_buffer);
}

/**
 * mod_lang_vm_I_run_execute
 * @brief   
 * @param   p_m
 * @param   file_name
 * @param   u64_key
 * @return  STD_CALL             std_int_t
 */
STD_CALL std_int_t mod_lang_vm_I_run_execute(IN mod_lang_vm_t *p_m, IN std_char_t *file_name, IN std_u64_t u64_key)
{
    return vm_execute(file_name, u64_key);
}

/**
 * mod_lang_vm_I_run_cleanup
 * @brief   
 * @param   p_m
 * @param   file_name
 * @return  STD_CALL             std_int_t
 */
STD_CALL std_int_t mod_lang_vm_I_run_cleanup(IN mod_lang_vm_t *p_m, IN std_char_t *file_name)
{
    return vm_cleanup(file_name);
}

struct mod_lang_vm_ops_st mod_lang_vm_I_ops = {
        mod_lang_vm_I_init,
        mod_lang_vm_I_cleanup,

        /***func_ops***/
        mod_lang_vm_I_run_init,
        mod_lang_vm_I_run_execute,
        mod_lang_vm_I_run_cleanup,

};

/**
 * mod_lang_vm_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_vm_I_create_instance(INOUT std_void_t **pp_handle)
{
    mod_lang_vm_imp_t *p_m = NULL;

    p_m = (mod_lang_vm_imp_t *) CALLOC(1, sizeof(mod_lang_vm_imp_t));
    p_m->unique_id = std_random_u64();
    p_m->p_ops = &mod_lang_vm_I_ops;

    mod_ownership_register_ops((mod_ownership_t *) p_m);
    mod_ownership_init((mod_ownership_t *) p_m);
    *pp_handle = p_m;

    return STD_RV_SUC;
}
