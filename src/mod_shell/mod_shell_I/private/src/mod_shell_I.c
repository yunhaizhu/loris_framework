/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_shell_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-18
 *
 */
#include "mod_shell_I.h"
#include "shell.h"
#include "std_lock_free_key_hash.h"

extern std_lock_free_key_hash_t *global_func_hash;
mod_lang_parse_t *p_global_mod_lang_parse = NULL;
mod_lang_compile_t *p_global_mod_lang_compile = NULL;
mod_lang_vm_t *p_global_mod_lang_vm = NULL;

/**
 * mod_shell_I_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_shell_I_init(IN mod_shell_t *p_m, IN const std_char_t *arg, IN std_int_t arg_len)
{
    mod_shell_imp_t *p_imp_m = (mod_shell_imp_t *) p_m;
    mod_iid_t mod_lang_parse_iid = MOD_LANG_PARSE_IID;
    mod_iid_t mod_lang_compile_iid = MOD_LANG_COMPILE_IID;
    mod_iid_t mod_lang_vm_iid = MOD_LANG_VM_IID;

    p_imp_m->hash_head = global_func_hash;

    if (p_global_mod_lang_parse == NULL) {
        mod_create_instance(&mod_lang_parse_iid, (std_void_t **) &p_imp_m->p_mod_lang_parse,
                            (mod_ownership_t *) p_imp_m);
        mod_lang_parse_init(p_imp_m->p_mod_lang_parse, NULL, 0);
        p_global_mod_lang_parse = p_imp_m->p_mod_lang_parse;
    }

    if (p_global_mod_lang_compile == NULL) {
        mod_create_instance(&mod_lang_compile_iid, (std_void_t **) &p_imp_m->p_mod_lang_compile,
                            (mod_ownership_t *) p_imp_m);
        mod_lang_compile_init(p_imp_m->p_mod_lang_compile, NULL, 0);
        p_global_mod_lang_compile = p_imp_m->p_mod_lang_compile;
    }

    if (p_global_mod_lang_vm == NULL) {
        mod_create_instance(&mod_lang_vm_iid, (std_void_t **) &p_imp_m->p_mod_lang_vm,
                            (mod_ownership_t *) p_imp_m);
        mod_lang_vm_init(p_imp_m->p_mod_lang_vm, NULL, 0);
        p_global_mod_lang_vm = p_imp_m->p_mod_lang_vm;
    }

    return STD_RV_SUC;
}

/**
 * mod_shell_I_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_shell_I_cleanup(IN mod_shell_t *p_m)
{
    mod_shell_imp_t *p_imp_m = (mod_shell_imp_t *) p_m;
    mod_iid_t mod_lang_parse_iid = MOD_LANG_PARSE_IID;
    mod_iid_t mod_lang_compile_iid = MOD_LANG_COMPILE_IID;
    mod_iid_t mod_lang_vm_iid = MOD_LANG_VM_IID;

    p_imp_m->hash_head = NULL;

    if (p_imp_m->p_mod_lang_parse) {
        mod_lang_parse_cleanup(p_imp_m->p_mod_lang_parse);
        mod_delete_instance(&mod_lang_parse_iid, (std_void_t **) &p_imp_m->p_mod_lang_parse,
                            (mod_ownership_t *) p_imp_m);
    }

    if (p_imp_m->p_mod_lang_compile) {
        mod_lang_compile_cleanup(p_imp_m->p_mod_lang_compile);
        mod_delete_instance(&mod_lang_compile_iid, (std_void_t **) &p_imp_m->p_mod_lang_compile,
                            (mod_ownership_t *) p_imp_m);
    }

    if (p_imp_m->p_mod_lang_vm) {
        mod_lang_vm_cleanup(p_imp_m->p_mod_lang_vm);
        mod_delete_instance(&mod_lang_vm_iid, (std_void_t **) &p_imp_m->p_mod_lang_vm,
                            (mod_ownership_t *) p_imp_m);
    }

    return STD_RV_SUC;
}

/***func_implementation***/


/**
 * mod_shell_I_register
 * @brief   
 * @param   m
 * @param   name
 * @param   name_len
 * @param   func
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_shell_I_register(IN mod_shell_t *p_m, IN const std_char_t *name, IN std_int_t name_len, IN const shell_stub_callback_t func, IN std_void_t *p_handle)
{
    mod_shell_imp_t *p_imp_m = (mod_shell_imp_t *) p_m;
    shell_func_node_t *p_node;
    std_rv_t ret;

    p_node = (shell_func_node_t *) CALLOC(1, sizeof(shell_func_node_t));
    STD_ASSERT_RV(p_node != NULL, STD_RV_ERR_OUTOFMEMORY);

    snprintf(p_node->func_name, sizeof(p_node->func_name), "%s", name);
    p_node->func_name_len = name_len;
    p_node->func_callback = func;
    p_node->module_handle = p_handle;

    ret = std_lock_free_key_hash_add(p_imp_m->hash_head, name, name_len, p_node);
    STD_ASSERT_RV_ACTION(ret == STD_RV_SUC, STD_RV_ERR_FAIL, FREE(p_node));

    return ret;
}

/**
 * mod_shell_I_unregister
 * @brief   
 * @param   m
 * @param   name
 * @return  stl_int_t
 */
STD_CALL std_rv_t mod_shell_I_unregister(IN mod_shell_t *p_m, IN const std_char_t *name, IN std_int_t name_len)
{
    mod_shell_imp_t *p_imp_m = (mod_shell_imp_t *) p_m;
    shell_func_node_t *p_node;
    std_rv_t ret;

    p_node = (shell_func_node_t *) std_lock_free_key_hash_find(p_imp_m->hash_head, name, name_len);
    STD_ASSERT_RV(p_node != NULL, STD_RV_ERR_POINTER);
    FREE(p_node);

    ret = std_lock_free_key_hash_del(p_imp_m->hash_head, name, name_len);
    STD_ASSERT_RV(ret == STD_RV_SUC, STD_RV_ERR_POINTER);

    return ret;
}


/**
 * mod_shell_I_shell
 * @brief   
 * @param   m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_shell_I_shell(IN mod_shell_t *p_m)
{
    cmd_shell();
    return STD_RV_SUC;
}

struct mod_shell_ops_st mod_shell_I_ops = {
        mod_shell_I_init,
        mod_shell_I_cleanup,
        /***func_ops***/
        mod_shell_I_register,
        mod_shell_I_unregister,
        mod_shell_I_shell,
};


/**
 * mod_shell_I_create_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_shell_I_create_instance(INOUT std_void_t **pp_handle)
{
    mod_shell_imp_t *p_m = NULL;

    p_m = (mod_shell_imp_t *) CALLOC(1, sizeof(mod_shell_imp_t));
    p_m->p_ops = &mod_shell_I_ops;

    mod_ownership_register_ops((mod_ownership_t *) p_m);
    mod_ownership_init((mod_ownership_t *) p_m);
    *pp_handle = p_m;

    return STD_RV_SUC;
}
