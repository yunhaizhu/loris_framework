/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_shell_I_bundle.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-18
 *
 */
#include "mod.h"
#include "mod_bundle.h"
#include "mod_shell.h"
#include "mod_shell_I.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"

static std_uint_t bundle_service_id = 0;
static mod_iid_t mod_shell_iid = MOD_SHELL_IID;

std_lock_free_key_hash_t *global_func_hash;

/**
 * bundle_create
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t bundle_create(std_uint_t bundle_id)
{
    bundle_service_id = bundle_id;
    return STD_RV_SUC;
}

/**
 * bundle_destroy
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t bundle_destroy()
{
    bundle_service_id = 0;
    return STD_RV_SUC;
}

/****shell_intf*****/


/**
 * bundle_start
 * @brief   
 * @param   arg
 * @return  int
 */
STD_CALL std_int_t bundle_start(IN const std_char_t *arg, IN const std_int_t arg_len)
{
    STD_ASSERT(mod_register_instance_func(&mod_shell_iid, mod_shell_I_create_instance) == STD_RV_SUC);
    STD_ASSERT(mod_bundle_IID_register(bundle_service_id, &mod_shell_iid) == STD_RV_SUC);

    global_func_hash = std_lock_free_key_hash_create(128);
    return STD_RV_SUC;
}

/**
 * bundle_stop
 * @brief   
 */
STD_CALL std_void_t bundle_stop()
{
    STD_ASSERT_RV(mod_unregister_instance_func(&mod_shell_iid) == STD_RV_SUC, );
    STD_ASSERT_RV(mod_bundle_IID_unregister(bundle_service_id) == STD_RV_SUC, );

    std_lock_free_key_hash_destroy(global_func_hash);
}
