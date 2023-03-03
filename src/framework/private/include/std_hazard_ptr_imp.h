/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_hazard_ptr_imp.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#ifndef SLOWLORIS_STD_HAZARD_PTR_IMP_H
#define SLOWLORIS_STD_HAZARD_PTR_IMP_H

#include "std_hazard_ptr.h"

#define HAZARD_MAX_COUNT HAZARD_MAX_THREAD *HAZARD_MAX_COUNT_PER_THREAD

typedef struct hazard_free_s {
    void *p;
    struct hazard_free_s *next;
} hazard_free_t;

typedef struct hazard_head_s {
    std_hazard_ptr_t hazard_array[HAZARD_MAX_COUNT];
    hazard_free_t hazard_free_list[HAZARD_MAX_THREAD];
} hazard_head_t;

#endif