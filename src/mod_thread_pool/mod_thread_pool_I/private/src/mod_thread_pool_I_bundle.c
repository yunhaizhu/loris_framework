/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_thread_pool_I_bundle.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-20
 *
 */

#include "mod.h"
#include "mod_bundle.h"
#include "mod_thread_pool_I.h"
#include "std_common.h"
#include "thpool.h"

static std_uint_t bundle_service_id = 0;
static mod_iid_t mod_thread_pool_iid = MOD_THREAD_POOL_IID;

threadpool global_thpool;
/**
 * bundle_create
 * @brief   
 * @param   bundle_id
 */
STD_CALL std_rv_t bundle_create(std_uint_t bundle_id)
{
    bundle_service_id = bundle_id;
    return STD_RV_SUC;
}

/**
 * bundle_destroy
 * @brief   
 */
STD_CALL std_rv_t bundle_destroy()
{
    bundle_service_id = 0;
    return STD_RV_SUC;
}

/**
 * bundle_start
 * @brief   
 * @param   arg
 * @return  int
 */
STD_CALL std_int_t bundle_start(IN const std_char_t *arg, IN const std_int_t arg_len)
{
    std_int_t num_threads;

    STD_ASSERT_RV(arg != NULL && arg_len != 0, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(mod_register_instance_func(&mod_thread_pool_iid, mod_thread_pool_I_create_instance) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_IID_register(bundle_service_id, &mod_thread_pool_iid) == STD_RV_SUC, STD_RV_ERR_FAIL);

    num_threads = std_safe_atoi(arg);
    global_thpool = thpool_init(num_threads);

    return STD_RV_SUC;
}

/**
 * bundle_stop
 * @brief   
 */
STD_CALL std_void_t bundle_stop()
{
    STD_ASSERT_RV(mod_unregister_instance_func(&mod_thread_pool_iid) == STD_RV_SUC, );
    STD_ASSERT_RV(mod_bundle_IID_unregister(bundle_service_id) == STD_RV_SUC, );

    thpool_wait(global_thpool);
    thpool_destroy(global_thpool);
}
