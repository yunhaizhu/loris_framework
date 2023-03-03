/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */
#ifndef SLOWLORIS_MOD_H
#define SLOWLORIS_MOD_H

#include "mod_ownership.h"
#include "std_common.h"

typedef struct mod_guid_st {
    std_u32_t data1;
    std_u16_t data2;
    std_u16_t data3;
    std_u8_t data4[8];
} mod_guid_t, mod_iid_t;


#define MOD_GUID(l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    {                                                       \
        l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 }       \
    }

typedef struct mod_default_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_default_ops_st *p_ops;
} mod_default_t;

struct mod_default_ops_st {
    std_int_t (*init)(IN mod_default_t *m, IN const std_char_t *arg, IN std_int_t arg_len);
    std_int_t (*cleanup)(IN mod_default_t *m);
};

/******************************************************************
 *                                                                *
 *                     Interface Function                         *
 *                                                                *
 ******************************************************************/

/**
 * std_rv_t
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @return  typedef
 */
typedef std_rv_t (*create_instance_func_t)(INOUT std_void_t **pp_handle);

/**
 * mod_register_instance_func
 * @brief   
 * @param   p_iid
 * @param   p_func_create_instance
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_register_instance_func(IN const mod_iid_t *p_iid, IN create_instance_func_t p_func_create_instance);

/**
 * mod_unregister_instance_func
 * @brief   
 * @param   p_iid
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_unregister_instance_func(IN const mod_iid_t *p_iid);


/**
 * mod_create_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_create_instance(IN const mod_iid_t *p_iid, OUT std_void_t **pp_handle, mod_ownership_t *owner_obj);

/**
 * mod_create_instance_with_iid_string
 * @brief   
 * @param   p_iid_string
 * @param   p_iid_string_size
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_create_instance_with_iid_string(IN const std_char_t *p_iid_string, IN std_size_t p_iid_string_size, OUT std_void_t **pp_handle, mod_ownership_t *owner_obj);


/**
 * mod_delete_instance
 * @brief   
 * @param   p_iid
 * @param   p_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_delete_instance(IN const mod_iid_t *p_iid, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj);

/**
 * mod_delete_instance_with_iid_string
 * @brief   
 * @param   p_iid_string
 * @param   p_iid_string_size
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_delete_instance_with_iid_string(IN const std_char_t *p_iid_string, IN std_size_t p_iid_string_size, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj);

/**
 * mod_query_instance
 * @brief   
 * @param   p_iid
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_query_instance(IN const mod_iid_t *p_iid, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj);

/**
 * mod_query_instance_with_iid_string
 * @brief   
 * @param   p_iid_string
 * @param   p_iid_string_size
 * @param   pp_handle
 * @param   owner_obj
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_query_instance_with_iid_string(IN const std_char_t *p_iid_string, IN std_size_t p_iid_string_size, IN std_void_t **pp_handle, IN const mod_ownership_t *owner_obj);

/**
 * mod_get_instance_counts
 * @brief   
 * @param   p_iid
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t mod_get_instance_counts(IN const mod_iid_t *p_iid);

#endif
