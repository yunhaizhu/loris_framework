/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_lock_free_key_hash_imp.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#ifndef SLOWLORIS_STD_LOCK_FREE_KEY_HASH_IMP_H
#define SLOWLORIS_STD_LOCK_FREE_KEY_HASH_IMP_H

#include "std_common.h"
#include "std_lock_free_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STD_LOCK_FREE_KEY_HASH_KEY_LEN BUF_SIZE_128

/**
 * @brief hash structure.
 *
 * The structure of hash.
 */
typedef struct {
    std_uint_t hash_buckets;
    std_lock_free_list_head_t *list_head;
    std_uint_t total_count;
} std_lock_free_key_hash_t;

typedef struct {
    std_char_t key[STD_LOCK_FREE_KEY_HASH_KEY_LEN]; /**< the key name #key. */
    std_u64_t u64_key;
    std_void_t *value;                   /**< the key value #value. */
    std_lock_free_list_head_t link_hash; /**< the double linked list  #link_hash. */
} std_lock_free_key_hash_node_t;

#ifdef __cplusplus
}
#endif

#endif