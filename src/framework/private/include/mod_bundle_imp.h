/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_bundle_imp.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#ifndef SLOWLORIS_MOD_BUNDLE_IMP_H
#define SLOWLORIS_MOD_BUNDLE_IMP_H

#include "mod.h"
#include "std_common.h"
#include "std_lock_free_list.h"

typedef enum mod_bundle_state_s {
    BUNDLE_UNINSTALLEDD = 0x00000001,
    BUNDLE_INSTALLED = 0x00000002,
    BUNDLE_START_FAILED = 0x00000004,
    BUNDLE_STARTING = 0x00000008,
    BUNDLE_STOPPING = 0x00000010,
    BUNDLE_ACTIVE = 0x00000020,
} mod_bundle_state_t;

typedef struct {
    std_int_t major;
    std_int_t minor;
    std_int_t patch;
} mod_version_t;

typedef struct mod_srv_s {
    std_char_t name[BUF_SIZE_128];
    std_char_t impl_name[BUF_SIZE_128];
    mod_version_t ver;

    std_uint_t id;
    std_void_t *dl_handle;

    /**
 * std_rv_t
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL
 */
    std_rv_t (*create)(IN const std_uint_t bundle_id);
    /**
 * std_rv_t
 * @brief   
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL
 */
    std_rv_t (*start)(IN const std_char_t *arg, IN const std_int_t arg_len);
    /**
 * std_rv_t
 * @brief   
 * @param   stop
 * @return  STD_CALL
 */
    std_rv_t (*stop)();
    /**
 * std_rv_t
 * @brief   
 * @param   destroy
 * @return  STD_CALL
 */
    std_rv_t (*destroy)();

    mod_bundle_state_t state;
    const mod_iid_t *p_iid;
    std_char_t *desc;
    std_size_t desc_size;
    std_lock_free_list_head_t list;
} mod_srv_t;

#endif