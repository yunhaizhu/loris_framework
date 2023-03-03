/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_ownership.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#include "mod_ownership.h"
#include "std_random.h"

/**
 * init_mod_ownership_key
 * @brief   
 * @param   owner_obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t init_mod_ownership_key(INOUT mod_ownership_t *owner_obj)
{
    rsa_gen_keys(&owner_obj->ownership_key.public_key, &owner_obj->ownership_key.private_key, PRIME_SOURCE_FILE);
    return STD_BOOL_TRUE;
}

/**
 * create_mod_ownership_signature
 * @brief   
 * @param   owner_obj
 * @param   obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t create_mod_ownership_signature(IN const mod_ownership_t *owner_obj, INOUT mod_ownership_t *obj)
{
    STD_ASSERT_RV(obj->ownership_token.token[0] == 0, STD_BOOL_FALSE);

    for (int i = 0; i < VERIFY_LIMIT; ++i) {
        obj->ownership_token.token[i] = (std_u16_t) (std_random_u64() % UINT16_MAX);
        obj->ownership_signature.signature[i] =
                rsa_ptr_encrypt(obj->ownership_token.token[i], &owner_obj->ownership_key.public_key);
    }

    return STD_BOOL_TRUE;
}

/**
 * verify_mod_ownership_signature
 * @brief   
 * @param   owner_obj
 * @param   obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t verify_mod_ownership_signature(IN const mod_ownership_t *owner_obj, IN const mod_ownership_t *obj)
{
    STD_ASSERT_RV(obj != NULL, STD_BOOL_FALSE);
    STD_ASSERT_RV(obj->ownership_token.token[0] != 0, STD_BOOL_FALSE);
    STD_ASSERT_RV(obj->ownership_signature.signature[0] != 0, STD_BOOL_FALSE);

    for (int i = 0; i < VERIFY_LIMIT; ++i) {
        std_u16_t token_decrypt = rsa_ptr_decrypt(obj->ownership_signature.signature[i], &owner_obj->ownership_key.private_key);
        STD_ASSERT_RV(token_decrypt == obj->ownership_token.token[i], STD_BOOL_FALSE);
    }

    STD_LOG(DEBUG, "PASS %s \n", __FUNCTION__);

    return STD_BOOL_TRUE;
}

/**
 * clear_mod_ownership_signature
 * @brief   
 * @param   owner_obj
 * @param   obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t clear_mod_ownership_signature(IN const mod_ownership_t *owner_obj, INOUT mod_ownership_t *obj)
{
    STD_ASSERT_RV(verify_mod_ownership_signature(owner_obj, obj) == STD_BOOL_TRUE, STD_BOOL_FALSE);

    for (int i = 0; i < VERIFY_LIMIT; ++i) {
        obj->ownership_token.token[i] = 0;
        obj->ownership_signature.signature[i] = 0;
    }

    return STD_BOOL_TRUE;
}

struct mod_ownership_ops_st ownership_ops = {
        init_mod_ownership_key,
        create_mod_ownership_signature,
        verify_mod_ownership_signature,
        clear_mod_ownership_signature,
};


/**
 * mod_ownership_register_ops
 * @brief   
 * @param   obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t mod_ownership_register_ops(IN mod_ownership_t *obj)
{
    obj->p_ops = &ownership_ops;

    return STD_BOOL_TRUE;
}