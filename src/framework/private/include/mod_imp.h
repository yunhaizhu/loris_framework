/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_imp.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#ifndef SLOWLORIS_MOD_IMP_H
#define SLOWLORIS_MOD_IMP_H

#include "mod.h"
#include "std_common.h"
#include "std_lock_free_list.h"

typedef struct mod_head_s {
    mod_iid_t iid;
    create_instance_func_t p_func_create_instance;
    std_lock_free_list_head_t head;
    std_int_t instance_counts;
} mod_head_t;

typedef struct mod_instance_s {
    std_void_t *p_handle;
    std_lock_free_list_head_t list;
} mod_instance_t;

/**
 * mod_init
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_init();

/**
 * mod_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_cleanup();

#endif