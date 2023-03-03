/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    stub_shell_mod_thread_pool_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-20
 *
 */
#include "json-maker.h"
#include "mod_thread_pool.h"
#include "std_common.h"
#include "tiny-json.h"

/**
 * shell_stub_mod_thread_pool_I_add_work
 * @brief   
 * @param   params
 * @return  std_char_t *
 */
std_char_t *shell_stub_mod_thread_pool_I_add_work(std_void_t *p_handle, std_char_t *params)
{
    json_t const *json;
    std_char_t response_string[BUF_SIZE_128];
    std_char_t *response;
    std_char_t *dest;
    mod_thread_pool_t *mod_thread_pool = (mod_thread_pool_t *) p_handle;
    /****server_stub_args_def****/
    std_int_t ret = 0;
    std_void_t *task_func = NULL;
    std_void_t *data = NULL;

    /*****server_stub_args_body*****/
    json_t mem[32];
    json = json_create(params, mem, sizeof mem / sizeof *mem);
    STD_ASSERT_RV(json != NULL, NULL);

    json_t const *param_obj = NULL;
    param_obj = json_getProperty(json, "task_func");

    task_func = (std_void_t *) json_getUInteger(param_obj);

    param_obj = json_getProperty(json, "data");

    data = (std_void_t *) json_getUInteger(param_obj);

    /******server_stub_call*****/
    ret = mod_thread_pool_add_work(mod_thread_pool, task_func, data);

    dest = json_objOpen(response_string, NULL);
    dest = json_objOpen(dest, "response");
    dest = json_objOpen(dest, "return");

    /******server_stub_ret******/
    dest = json_str(dest, "ret_type", "INTEGER");
    dest = json_verylong(dest, "ret", ret);

    dest = json_objClose(dest);
    dest = json_objClose(dest);
    dest = json_objClose(dest);
    json_end(dest);

    response = strdup(response_string);

    return response;
}
