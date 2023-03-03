/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_hazard_ptr.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#ifndef SLOWLORIS_STD_HAZARD_PTR_H
#define SLOWLORIS_STD_HAZARD_PTR_H

#include "std_common.h"

typedef std_void_t *std_hazard_ptr_t;
typedef std_void_t *std_hazard_head_t;

#define HAZARD_MAX_THREAD 64
#define HAZARD_MAX_COUNT_PER_THREAD 6

/**
 * std_hazard_get
 * @brief   
 * @param   hazard_head
 * @param   idx
 * @return  std_hazard_ptr_t *
 */
std_hazard_ptr_t *std_hazard_get(std_hazard_head_t *hazard_head, std_int_t idx);

/**
 * std_hazard_defer_free
 * @brief   
 * @param   hazard_head
 * @param   p
 * @return  std_void_t
 */
std_void_t std_hazard_defer_free(std_hazard_head_t *hazard_head, std_void_t *p);

/**
 * std_hazard_gc
 * @brief   
 * @param   hazard_head
 * @return  std_void_t
 */
std_void_t std_hazard_gc(std_hazard_head_t *hazard_head);

#define std_hazard_set_ptr(hazard, p) ((*(std_void_t **) (hazard)) = (p))

#endif