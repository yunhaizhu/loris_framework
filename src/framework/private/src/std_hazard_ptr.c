/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_hazard_ptr.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#include "std_hazard_ptr_imp.h"

static std_int_t tidSeed = 0;

/**
 * get_thread_id
 * @brief   
 * @param   hazard_head
 * @return  static std_int_t
 */
static std_int_t get_thread_id(std_hazard_head_t *hazard_head)
{
    static __thread std_int_t _tid = -1;
    hazard_head_t *hazard_head_imp = (hazard_head_t *) hazard_head;

    if (_tid >= 0) {
        return _tid;
    }
    _tid = ATOMIC_INC(&tidSeed);

    hazard_head_imp->hazard_free_list[_tid].next = NULL;
    return _tid;
}

/**
 * hazard_conflict
 * @brief   
 * @param   hazard_head
 * @param   self
 * @param   p
 * @return  static int
 */
static int hazard_conflict(std_hazard_head_t *hazard_head, std_int_t self, const std_void_t *p)
{
    std_int_t self_p = self * HAZARD_MAX_COUNT_PER_THREAD;
    const hazard_head_t *hazard_head_imp = (hazard_head_t *) hazard_head;

    for (std_int_t i = 0; i < HAZARD_MAX_COUNT; ++i) {
        if (i >= self_p && i < self_p + HAZARD_MAX_COUNT_PER_THREAD)
            continue; /* skip self */
        if (hazard_head_imp->hazard_array[i] == p)
            return STD_BOOL_TRUE;
    }
    return STD_BOOL_FALSE;
}

/**
 * std_hazard_get
 * @brief   
 * @param   hazard_head
 * @param   idx
 * @return  std_hazard_ptr_t *
 */
std_hazard_ptr_t *std_hazard_get(std_hazard_head_t *hazard_head, std_int_t idx)
{
    std_int_t tid = get_thread_id(hazard_head);
    hazard_head_t *hazard_head_imp = (hazard_head_t *) hazard_head;

    return &(hazard_head_imp->hazard_array[tid * HAZARD_MAX_COUNT_PER_THREAD + idx]);
}

/**
 * std_hazard_defer_free
 * @brief   
 * @param   hazard_head
 * @param   p
 * @return  std_void_t
 */
std_void_t std_hazard_defer_free(std_hazard_head_t *hazard_head, std_void_t *p)
{
    std_int_t tid = get_thread_id(hazard_head);
    hazard_head_t *hazard_head_imp = (hazard_head_t *) hazard_head;
    hazard_free_t *f = (hazard_free_t *) malloc(sizeof(hazard_free_t));

    f->p = p;
    f->next = hazard_head_imp->hazard_free_list[tid].next;
    hazard_head_imp->hazard_free_list[tid].next = f;

    std_hazard_gc(hazard_head);
}

/**
 * std_hazard_gc
 * @brief   
 * @param   hazard_head
 * @return  std_void_t
 */
std_void_t std_hazard_gc(std_hazard_head_t *hazard_head)
{
    std_int_t tid = get_thread_id(hazard_head);
    hazard_head_t *hazard_head_imp = (hazard_head_t *) hazard_head;
    hazard_free_t *head = &hazard_head_imp->hazard_free_list[tid];
    hazard_free_t *pred = head;
    hazard_free_t *next = head->next;

    while (next) {
        if (!hazard_conflict(hazard_head, tid, next->p)) { /* safe to free */
            hazard_free_t *tmp = next->next;
            STD_LOG(DEBUG, "hazard (%d) free ptr %p\n", tid, next->p);
            free(next->p);
            pred->next = tmp;
            free(next);
            next = tmp;
        } else {
            pred = next;
            next = next->next;
        }
    }
}