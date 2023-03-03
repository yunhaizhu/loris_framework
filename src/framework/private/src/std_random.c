/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_random.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-19
 *
 */
#include "std_random.h"
#include <gsl/gsl_rng.h>

static gsl_rng_env_t global_gsl_rng_env[GSL_RNG_ENV_MAX_THREAD];
std_int_t global_tidSeed = 0;

/**
 * get_thread_id
 * @brief
 * @return  static int
 */
STD_CALL std_int_t get_std_thread_id()
{
    static __thread std_int_t _tid = -1;

    if (_tid >= 0) {
        return _tid;
    }

    _tid = ATOMIC_INC(&global_tidSeed);

    return _tid;
}

/**
 * inline_get_gsl_rng_r
 * @brief   
 * @return  STD_CALL static inline gsl_rng *
 */
STD_CALL static inline gsl_rng *inline_get_gsl_rng_r()
{
    gsl_rng *r;

    r = global_gsl_rng_env[get_std_thread_id()].r;
    if (r == NULL) {
        r = gsl_rng_alloc(gsl_rng_default);
        gsl_rng_set(r, (get_std_thread_id() + 13) * 3233323443);
        global_gsl_rng_env[get_std_thread_id()].r = r;
    }

    return r;
}

/**
 * std_random_u64
 * @brief   
 * @return  std_u64_t
 */
STD_CALL std_u64_t std_random_u64()
{
    const gsl_rng *r;
    uint64_t u64_number;

    r = inline_get_gsl_rng_r();
    u64_number = ((gsl_rng_get(r) << 0) & 0x00000000FFFFFFFFULL) | ((gsl_rng_get(r) << 32) & 0xFFFFFFFF00000000ULL);

    return u64_number;
}

/**
 * std_random_string
 * @brief   
 * @param   length
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *std_random_string(IN std_int_t length)
{
    const std_char_t *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t string_len = 26 * 2 + 10 + 7;
    std_char_t *random_string;
    std_uint_t key;
    const gsl_rng *r;

    STD_ASSERT_RV_WARN(length <= BUF_SIZE_8192, NULL);

    random_string = CALLOC(length + 1, sizeof(std_char_t));

    r = inline_get_gsl_rng_r();

    for (std_int_t n = 0; n < length - 1; n++) {
        key = (std_uint_t) (gsl_rng_get(r) % string_len);
        random_string[n] = string[key];
    }

    random_string[length - 1] = '\0';

    return random_string;
}