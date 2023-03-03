/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_random.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#ifndef SLOWLORIS_STD_RANDOM_H
#define SLOWLORIS_STD_RANDOM_H

#include "std_common.h"
#include <gsl/gsl_rng.h>

typedef struct gsl_rng_env_s {
    gsl_rng *r;
} gsl_rng_env_t;

#define GSL_RNG_ENV_MAX_THREAD 128


/**
 * std_random_u64
 * @brief   
 * @return  std_u64_t
 */
STD_CALL std_u64_t std_random_u64();

/**
 * std_random_string
 * @brief   
 * @param   length
 * @return  std_char_t *
 */
STD_CALL std_char_t *std_random_string(IN std_int_t length);


/**
 * get_std_thread_id
 * @brief   
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_std_thread_id();

#endif