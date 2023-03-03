/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_thread_pool_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-20
 *
 */
#include "mod_thread_pool_I.h"
#include "thpool.h"


extern threadpool global_thpool;

/**
 * mod_thread_pool_I_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_thread_pool_I_init(IN mod_thread_pool_t *p_m, IN const std_char_t *arg, IN std_int_t arg_len)
{
    mod_thread_pool_imp_t *p_imp_m = (mod_thread_pool_imp_t *) p_m;
    mod_iid_t mod_shell_iid = MOD_SHELL_IID;
    std_char_t key[BUF_SIZE_128] = "\0";

    p_imp_m->thread_pool = global_thpool;

    mod_create_instance(&mod_shell_iid, (std_void_t **) &p_imp_m->p_mod_shell, (mod_ownership_t *) p_imp_m);

    mod_shell_init(p_imp_m->p_mod_shell, NULL, 0);

    /****shell_register******/
    snprintf(key, sizeof(key), "%lul-%s", p_imp_m->unique_id, "mod_thread_pool_add_work");
    mod_shell_register(p_imp_m->p_mod_shell, key, std_safe_strlen(key, sizeof(key)), shell_stub_mod_thread_pool_I_add_work, p_imp_m);

    return STD_RV_SUC;
}


/**
 * mod_thread_pool_I_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_thread_pool_I_cleanup(IN mod_thread_pool_t *p_m)
{
    mod_thread_pool_imp_t *p_imp_m = (mod_thread_pool_imp_t *) p_m;
    mod_iid_t mod_shell_iid = MOD_SHELL_IID;
    std_char_t key[BUF_SIZE_128] = "\0";

    /****shell_unregister******/
    snprintf(key, sizeof(key), "%lul-%s", p_imp_m->unique_id, "mod_thread_pool_add_work");
    mod_shell_unregister(p_imp_m->p_mod_shell, key, std_safe_strlen(key, sizeof(key)));

    mod_shell_cleanup(p_imp_m->p_mod_shell);

    mod_delete_instance(&mod_shell_iid, (std_void_t **) &p_imp_m->p_mod_shell, (mod_ownership_t *) p_imp_m);

    p_imp_m->thread_pool = NULL;

    return STD_RV_SUC;
}

/***func_implementation***/

/**
 * mod_thread_pool_I_add_work
 * @brief   
 * @param   p_m
 * @param   task_func
 * @param   data
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_thread_pool_I_add_work(IN mod_thread_pool_t *p_m, IN thread_callback_t task_func, IN std_void_t *data)
{
    const mod_thread_pool_imp_t *p_imp_m = (mod_thread_pool_imp_t *) p_m;

    STD_ASSERT_RV(task_func != NULL, STD_RV_ERR_INVALIDARG);

    return thpool_add_work(p_imp_m->thread_pool, task_func, data);
}

struct mod_thread_pool_ops_st mod_thread_pool_I_ops = {
        mod_thread_pool_I_init,
        mod_thread_pool_I_cleanup,
        /***func_ops***/
        mod_thread_pool_I_add_work,
};


/**
 * mod_thread_pool_I_create_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_thread_pool_I_create_instance(INOUT std_void_t **pp_handle)
{
    mod_thread_pool_imp_t *p_m = NULL;

    p_m = (mod_thread_pool_imp_t *) CALLOC(1, sizeof(mod_thread_pool_imp_t));
    p_m->unique_id = std_random_u64();
    p_m->p_ops = &mod_thread_pool_I_ops;

    mod_ownership_register_ops((mod_ownership_t *) p_m);
    mod_ownership_init((mod_ownership_t *) p_m);
    *pp_handle = p_m;

    return STD_RV_SUC;
}
