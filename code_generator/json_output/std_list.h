/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * @file    std_list.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-04
 *
 */

#ifndef STD_LIST_H
#define STD_LIST_H

typedef struct std_list_head {
    struct std_list_head *prev;
    struct std_list_head *next;
} std_list_head_t;


/**
 * STD_INIT_LIST_HEAD
 * @brief   
 * @param   ptr
 * @return  static inline void
 */
static inline void STD_INIT_LIST_HEAD(std_list_head_t *ptr)
{
    ptr->prev = ptr;
    ptr->next = ptr;
}


/**
 * std_list_add
 * @brief   
 * @param   new
 * @param   head
 * @return  static inline void
 */
static inline void std_list_add(struct std_list_head *new, struct std_list_head *head)
{
    (head->next)->prev = new;
    new->next = (head->next);
    new->prev = head;
    head->next = new;
}

/**
 * std_list_addp
 * @brief   
 * @param   new
 * @param   head
 * @return  static inline void
 */
static inline void std_list_addp(struct std_list_head *new, struct std_list_head *head)
{
    new->prev = head->prev;
    new->next = head->prev->next;
    head->prev->next = new;
    head->prev = new;
}


/**
 * std_list_del
 * @brief   
 * @param   entry
 * @return  static inline void
 */
static inline void std_list_del(struct std_list_head *entry)
{
    (entry->next)->prev = entry->prev;
    (entry->prev)->next = entry->next;

    entry->next = entry->prev = entry;
}

/**
 * std_is_list_empty
 * @brief   
 * @param   head
 * @return  static inline int
 */
static inline int std_is_list_empty(struct std_list_head *head)
{
    return head->next == head;
}

#define std_list_entry(ptr, type, member) ((type *) ((unsigned char *) (ptr) - (unsigned char *) (&((type *) 0)->member)))

#endif
