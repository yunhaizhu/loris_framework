/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_ownership.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#ifndef SLOWLORIS_MOD_OWNERSHIP_H
#define SLOWLORIS_MOD_OWNERSHIP_H

#include "rsa.h"
#include "std_common.h"

#define VERIFY_LIMIT 4
typedef struct ownership_token_s {
    std_u16_t token[VERIFY_LIMIT];
} ownership_token_t;

typedef struct ownership_signature_s {
    std_u128_t signature[VERIFY_LIMIT];
} ownership_signature_t;

typedef struct ownership_key_s {
    struct public_key_class public_key;
    struct private_key_class private_key;
} ownership_key_t;

typedef struct mod_ownership_st {
    struct mod_ownership_ops_st *p_ops;
    ownership_token_t ownership_token;
    ownership_signature_t ownership_signature;
    ownership_key_t ownership_key;
} mod_ownership_t;

#define MOD_OWNERSHIP(type)                                                              \
    std_bool_t (*init_key)(INOUT type * owner_inst);                                     \
    std_bool_t (*create_signature)(IN const type *owner_inst, INOUT type *need_inst);    \
    std_bool_t (*verify_signature)(IN const type *owner_inst, IN const type *need_inst); \
    std_bool_t (*clear_signature)(IN const type *owner_inst, INOUT type *need_inst);

struct mod_ownership_ops_st {
    MOD_OWNERSHIP(mod_ownership_t);
};

/******************************************************************
 *                                                                *
 *                     Global Variable                            *
 *                                                                *     
 ******************************************************************/


/******************************************************************
 *                                                                *
 *                     Interface Function                         *
 *                                                                *     
 ******************************************************************/

#define mod_ownership_init(owner_obj) \
    (((mod_ownership_t *) (owner_obj))->p_ops->init_key((mod_ownership_t *) (owner_obj)))
#define mod_ownership_create(owner_obj, obj) \
    (((mod_ownership_t *) (owner_obj))->p_ops->create_signature((mod_ownership_t *) (owner_obj), (mod_ownership_t *) (obj)))
#define mod_ownership_verify(owner_obj, obj) \
    (((mod_ownership_t *) (owner_obj))->p_ops->verify_signature((mod_ownership_t *) (owner_obj), (mod_ownership_t *) (obj)))
#define mod_ownership_clear(owner_obj, obj) \
    (((mod_ownership_t *) (owner_obj))->p_ops->clear_signature((mod_ownership_t *) (owner_obj), (mod_ownership_t *) (obj)))

/**
 * mod_ownership_register_ops
 * @brief   
 * @param   obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t mod_ownership_register_ops(IN mod_ownership_t *obj);

#endif