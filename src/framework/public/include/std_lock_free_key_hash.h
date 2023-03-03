/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_lock_free_key_hash.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#ifndef SLOWLORIS_STD_LOCK_FREE_KEY_HASH_H
#define SLOWLORIS_STD_LOCK_FREE_KEY_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "std_common.h"

typedef struct std_lock_free_key_hash_t std_lock_free_key_hash_t;

/**
 * std_lock_free_key_hash_create
 * @brief   
 * @param   hash_buckets
 * @return  std_lock_free_key_hash_t *
 */
std_lock_free_key_hash_t *std_lock_free_key_hash_create(std_uint_t hash_buckets);

/**
 * std_lock_free_key_hash_add
 * @brief   
 * @param   p_hash
 * @param   key
 * @param   key_len
 * @param   value
 * @return  std_rv_t
 */
std_rv_t std_lock_free_key_hash_add(IN std_lock_free_key_hash_t *p_hash, IN const std_char_t *key, IN const std_size_t key_len, IN std_void_t *value);

/**
 * stl_lock_free_key_hash_find
 * @brief   
 * @param   p_hash
 * @param   key
 * @param   key_len
 * @return  std_void_t *
 */
std_void_t *std_lock_free_key_hash_find(IN std_lock_free_key_hash_t *p_hash, IN const std_char_t *key, IN const std_size_t key_len);

/**
 * stl_lock_free_key_hash_del
 * @brief   
 * @param   p_hash
 * @param   key
 * @param   key_len
 * @return  std_rv_t
 */
std_rv_t std_lock_free_key_hash_del(IN std_lock_free_key_hash_t *p_hash, IN const std_char_t *key, IN const std_size_t key_len);

/**
 * stl_lock_free_key_hash_destroy
 * @brief   
 * @param   p_hash
 * @return  std_void_t
 */
std_void_t std_lock_free_key_hash_destroy(IN std_lock_free_key_hash_t *p_hash);

/**
 * stl_lock_free_key_hash_value_destroy
 * @brief   
 * @param   p_hash
 * @return  std_void_t
 */
std_void_t std_lock_free_key_hash_value_destroy(IN std_lock_free_key_hash_t *p_hash);

/**
 * std_void_t
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  typedef
 */
typedef std_void_t (*destroy_callback_t)(IN std_void_t *data, IN const std_void_t *callback_arg);
/**
 * stl_lock_free_key_hash_value_callback_destroy
 * @brief   
 * @param   p_hash
 * @param   destroy_callback
 * @param   callback_arg
 * @return  std_void_t
 */
std_void_t std_lock_free_key_hash_value_callback_destroy(IN std_lock_free_key_hash_t *p_hash, IN destroy_callback_t destroy_callback, IN std_void_t *callback_arg);

/**
 * std_void_t
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  typedef
 */
typedef std_void_t (*walk_callback_t)(const std_char_t *key, std_void_t **data, std_void_t *callback_arg);
/**
 * stl_lock_free_key_hash_value_walk
 * @brief   
 * @param   p_hash
 * @param   walk_callback
 * @param   p_cb_data
 * @return  std_void_t
 */
std_void_t std_lock_free_key_hash_value_walk(IN std_lock_free_key_hash_t *p_hash, IN walk_callback_t walk_callback, IN std_void_t *p_cb_data);

/**
 * std_lock_free_key_hash_get_total_count
 * @brief   
 * @param   p_hash
 * @return  std_uint_t
 */
std_uint_t std_lock_free_key_hash_get_total_count(IN const std_lock_free_key_hash_t *p_hash);

#ifdef __cplusplus
}
#endif
#endif