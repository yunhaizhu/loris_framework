/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_lock_free_list.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */
#ifndef SLOWLORIS_STD_LOCK_FREE_LIST_H
#define SLOWLORIS_STD_LOCK_FREE_LIST_H

#include "std_common.h"
#include "std_hazard_ptr.h"

typedef struct std_lock_free_list_head {
    std_u64_t key;
    std_uint_t offset;
    std_bool_t free_enable : 1;
    std_bool_t key_enable : 1;
    std_hazard_head_t *hazard_head;
    struct std_lock_free_list_head *next;
} std_lock_free_list_head_t;

/**
 * std_lock_free_list_init
 * @brief   
 * @param   head
 * @param   offset
 * @param   free_enable
 * @param   key_enable
 * @return  std_rv_t
 */
STD_CALL std_rv_t std_lock_free_list_init(INOUT std_lock_free_list_head_t *head, IN std_uint_t offset, IN std_bool_t free_enable, IN std_bool_t key_enable);

/**
 * std_lock_free_list_add
 * @brief   
 * @param   head
 * @param   entry
 * @param   u64_key
 * @return  std_rv_t
 */
STD_CALL std_rv_t std_lock_free_list_add(INOUT std_lock_free_list_head_t *head, IN std_lock_free_list_head_t *entry, IN std_u64_t u64_key);

/**
 * std_lock_free_list_del
 * @brief   
 * @param   head
 * @param   u64_key
 * @return  std_rv_t
 */
STD_CALL std_rv_t std_lock_free_list_del(INOUT std_lock_free_list_head_t *head, IN std_u64_t u64_key);


/**
 * std_lock_free_list_del_flag
 * @brief   
 * @param   head
 * @param   u64_key
 * @param   del_flag
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t std_lock_free_list_del_flag(IN std_lock_free_list_head_t *head, IN std_u64_t u64_key, IN std_bool_t del_flag);

/**
 * std_lock_free_list_find
 * @brief   
 * @param   head
 * @param   u64_key
 * @return  std_lock_free_list_head_t *
 */
STD_CALL std_lock_free_list_head_t *std_lock_free_list_find(INOUT std_lock_free_list_head_t *head, IN std_u64_t u64_key);

/**
 * std_lock_free_list_count
 * @brief   
 * @param   head
 * @return  std_uint_t
 */
STD_CALL std_uint_t std_lock_free_list_count(IN std_lock_free_list_head_t *head);

/**
 * std_lock_free_list_cleanup
 * @brief   
 * @param   head
 * @return  std_rv_t
 */
STD_CALL std_rv_t std_lock_free_list_cleanup(IN std_lock_free_list_head_t *head);

#define std_lock_free_list_is_empty(head) ((head)->next == NULL)
#define std_lock_free_list_head_entry(ptr, type, member) ((type *) ((std_uchar_t *) (ptr) - (std_uchar_t *) (&((type *) 0)->member)))
#define std_lock_free_list_head_entry_offset(type, member) (long long) ((std_uchar_t *) (&((type *) 0)->member))

typedef size_t mark_able_t;
#define STD_LOCK_FREE_LIST_MARK(ptr) (std_lock_free_list_head_t *) ((mark_able_t) (ptr) | 0x01)
#define STD_LOCK_FREE_LIST_HAS_MARK(ptr) ((mark_able_t) (ptr) &0x01)
#define STD_LOCK_FREE_LIST_STRIP_MARK(ptr) ((std_lock_free_list_head_t *) ((mark_able_t) (ptr) & ~(0x01)))

#endif
