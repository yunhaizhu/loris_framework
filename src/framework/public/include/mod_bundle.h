/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_bundle.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#ifndef SLOWLORIS_MOD_BUNDLE_H
#define SLOWLORIS_MOD_BUNDLE_H

#include "mod.h"
#include "std_common.h"

typedef enum mod_bundle_state {
    BUNDLE_UNINSTALLEDD = 0x00000001,
    BUNDLE_INSTALLED = 0x00000002,
    BUNDLE_RESOLVED = 0x00000004,
    BUNDLE_STARTING = 0x00000008,
    BUNDLE_STOPPING = 0x00000010,
    BUNDLE_ACTIVE = 0x00000020,
} mod_bundle_state_t;

/******************************************************************
 *                                                                *
 *                     Interface Function                         *
 *                                                                *     
 ******************************************************************/

/**
 * mod_bundle_init
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_init();

/**
 * mod_bundle_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cleanup();

/**
 * mod_bundle_cmd_install
 * @brief   
 * @param   bundle_name
 * @param   bundle_name_len
 * @param   p_bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_install(IN const std_char_t *bundle_name, IN std_int_t bundle_name_len, INOUT std_uint_t *p_bundle_id);

/**
 * mod_bundle_cmd_uninstall
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_uninstall(IN std_uint_t bundle_id);

/**
 * mod_bundle_cmd_start
 * @brief   
 * @param   bundle_id
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_start(IN std_uint_t bundle_id, IN const std_char_t *arg, IN std_int_t arg_len);

/**
 * mod_bundle_cmd_stop
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_stop(IN std_uint_t bundle_id);

/**
 * mod_bundle_get_desc
 * @brief   
 * @param   bundle_id
 * @param   desc
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_get_desc(IN std_uint_t bundle_id, INOUT std_char_t **desc);
/**
 * mod_bundle_IID_register
 * @brief   
 * @param   bundle_id
 * @param   p_iid
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_IID_register(IN std_uint_t bundle_id, IN const mod_iid_t *p_iid);

/**
 * mod_bundle_IID_unregister
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_IID_unregister(std_uint_t bundle_id);

/**
 * std_void_t
 * @brief   
 * @param   data
 * @return  typedef
 */
typedef std_void_t (*callback_func_t)(std_void_t *data);
/**
 * mod_bundle_walk
 * @brief
 * @param   callback_func
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_walk(IN callback_func_t callback_func);

#endif