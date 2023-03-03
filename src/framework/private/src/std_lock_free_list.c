/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_lock_free_list.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */

#include "std_hazard_ptr.h"
#include "std_lock_free_list_imp.h"

std_bool_t std_lock_free_list_inline_find(IN std_lock_free_list_head_t *head,
                                          IN std_u64_t key,
                                          IN std_uint_t offset,
                                          INOUT std_lock_free_list_head_t **prev_entry,
                                          INOUT std_lock_free_list_head_t **curr_entry,
                                          INOUT std_lock_free_list_head_t **next_entry);

/**
 * std_lock_free_list_init
 * @brief   
 * @param   head
 * @param   offset
 * @param   free_enable
 * @param   key_enable
 * @return  std_rv_t
 */
std_rv_t std_lock_free_list_init(INOUT std_lock_free_list_head_t *head, IN std_uint_t offset, IN std_bool_t free_enable, IN std_bool_t key_enable)
{
    STD_ASSERT_RV(head != NULL, STD_RV_ERR_INVALIDARG);

    head->next = NULL;
    head->offset = offset;
    head->free_enable = free_enable;
    head->key_enable = key_enable;

    if (head->hazard_head == NULL) {
        head->hazard_head = (hazard_head_t *) CALLOC(sizeof(hazard_head_t), 1);
    }

    return STD_RV_SUC;
}

/**
 * std_lock_free_list_add
 * @brief   
 * @param   head
 * @param   entry
 * @param   u64_key
 * @return  std_rv_t
 */
std_rv_t std_lock_free_list_add(IN std_lock_free_list_head_t *head, IN std_lock_free_list_head_t *entry, IN std_u64_t u64_key)
{
    std_lock_free_list_head_t *prev = NULL;
    std_lock_free_list_head_t *cur = NULL;
    std_lock_free_list_head_t *next = NULL;
    std_hazard_ptr_t *hp0 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 0);
    std_hazard_ptr_t *hp1 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 1);
    std_hazard_ptr_t *hp2 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 2);
    std_rv_t result = STD_RV_ERR_FAIL;

    STD_ASSERT_RV(head != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(entry != NULL, STD_RV_ERR_INVALIDARG);

    while (result != STD_RV_SUC) {
        if (std_lock_free_list_inline_find(head, u64_key, head->offset, &prev, &cur, &next) == STD_BOOL_TRUE) {
            result = STD_RV_ERR_FAIL;
            break;
        }

        entry->key = u64_key;

        if (entry->next == NULL) {
            entry->next = STD_LOCK_FREE_LIST_STRIP_MARK(cur);
        } else if (STD_LOCK_FREE_LIST_HAS_MARK(entry->next)) {
            entry->next = STD_LOCK_FREE_LIST_STRIP_MARK(cur);
        } else if (STD_LOCK_FREE_LIST_STRIP_MARK(cur) != NULL) {
            std_lock_free_list_head_t *entry_next = entry->next;
            while (entry_next->next != NULL) {
                entry_next = entry_next->next;
            }
            entry_next->next = STD_LOCK_FREE_LIST_STRIP_MARK(cur);
        }

        if (CAS(&prev->next, STD_LOCK_FREE_LIST_STRIP_MARK(cur), STD_LOCK_FREE_LIST_STRIP_MARK(entry))) {
            result = STD_RV_SUC;
        }
    }

    std_hazard_set_ptr(hp0, NULL);
    std_hazard_set_ptr(hp1, NULL);
    std_hazard_set_ptr(hp2, NULL);

    return result;
}

/**
 * std_lock_free_list_del
 * @brief   
 * @param   head
 * @param   u64_key
 * @return  std_rv_t
 */
std_rv_t std_lock_free_list_del(IN std_lock_free_list_head_t *head, IN std_u64_t u64_key)
{
    std_lock_free_list_head_t *prev = NULL;
    std_lock_free_list_head_t *cur = NULL;
    std_lock_free_list_head_t *next = NULL;
    std_hazard_ptr_t *hp0 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 0);
    std_hazard_ptr_t *hp1 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 1);
    std_hazard_ptr_t *hp2 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 2);
    std_rv_t result = STD_RV_ERR_FAIL;

    STD_ASSERT_RV(head != NULL, STD_RV_ERR_INVALIDARG);

    while (result != STD_RV_SUC) {
        if (std_lock_free_list_inline_find(head, u64_key, head->offset, &prev, &cur, &next) == STD_BOOL_FALSE) {
            result = STD_RV_ERR_FAIL;
            break;
        }

        if (CAS(&cur->next, STD_LOCK_FREE_LIST_STRIP_MARK(next), STD_LOCK_FREE_LIST_MARK(next)) != STD_BOOL_TRUE) {
            continue;
        }

        if (CAS(&prev->next, STD_LOCK_FREE_LIST_STRIP_MARK(cur), STD_LOCK_FREE_LIST_STRIP_MARK(next))) {
            //del cur
            if (head->free_enable) {
                std_hazard_defer_free((std_hazard_head_t *) head->hazard_head, (std_uchar_t *) cur - head->offset);
            }
        } else {
            std_lock_free_list_inline_find(head, u64_key, head->offset, &prev, &cur, &next);
        }

        result = STD_RV_SUC;
    }

    std_hazard_set_ptr(hp0, NULL);
    std_hazard_set_ptr(hp1, NULL);
    std_hazard_set_ptr(hp2, NULL);

    return result;
}

/**
 * std_lock_free_list_del_flag
 * @brief   
 * @param   head
 * @param   u64_key
 * @param   del_flag
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t std_lock_free_list_del_flag(IN std_lock_free_list_head_t *head, IN std_u64_t u64_key, IN std_bool_t del_flag)
{
    std_lock_free_list_head_t *prev = NULL;
    std_lock_free_list_head_t *cur = NULL;
    std_lock_free_list_head_t *next = NULL;
    std_hazard_ptr_t *hp0 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 0);
    std_hazard_ptr_t *hp1 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 1);
    std_hazard_ptr_t *hp2 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 2);
    std_rv_t result = STD_RV_ERR_FAIL;

    STD_ASSERT_RV(head != NULL, STD_RV_ERR_INVALIDARG);

    while (result != STD_RV_SUC) {
        if (std_lock_free_list_inline_find(head, u64_key, head->offset, &prev, &cur, &next) == STD_BOOL_FALSE) {
            result = STD_RV_ERR_FAIL;
            break;
        }

        if (CAS(&cur->next, STD_LOCK_FREE_LIST_STRIP_MARK(next), STD_LOCK_FREE_LIST_MARK(next)) != STD_BOOL_TRUE) {
            continue;
        }

        if (CAS(&prev->next, STD_LOCK_FREE_LIST_STRIP_MARK(cur), STD_LOCK_FREE_LIST_STRIP_MARK(next))) {
            //del cur
            if (del_flag) {
                std_hazard_defer_free((std_hazard_head_t *) head->hazard_head, (std_uchar_t *) cur - head->offset);
            }
        } else {
            std_lock_free_list_inline_find(head, u64_key, head->offset, &prev, &cur, &next);
        }

        result = STD_RV_SUC;
    }

    std_hazard_set_ptr(hp0, NULL);
    std_hazard_set_ptr(hp1, NULL);
    std_hazard_set_ptr(hp2, NULL);

    return result;
}

/**
 * std_lock_free_list_find
 * @brief   
 * @param   head
 * @param   u64_key
 * @return  std_lock_free_list_head_t *
 */
std_lock_free_list_head_t *std_lock_free_list_find(IN std_lock_free_list_head_t *head, IN std_u64_t u64_key)
{
    std_lock_free_list_head_t *prev = NULL;
    std_lock_free_list_head_t *cur = NULL;
    std_lock_free_list_head_t *next = NULL;
    std_hazard_ptr_t *hp0 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 0);
    std_hazard_ptr_t *hp1 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 1);
    std_hazard_ptr_t *hp2 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 2);
    std_bool_t result;

    STD_ASSERT_RV(head != NULL, NULL);

    result = std_lock_free_list_inline_find(head, u64_key, head->offset, &prev, &cur, &next);

    std_hazard_set_ptr(hp0, NULL);
    std_hazard_set_ptr(hp1, NULL);
    std_hazard_set_ptr(hp2, NULL);

    if (result == STD_BOOL_TRUE) {
        return cur;
    } else {
        return NULL;
    }
}

/**
 * std_lock_free_list_count
 * @brief   
 * @param   head
 * @return  std_uint_t
 */
std_uint_t std_lock_free_list_count(IN std_lock_free_list_head_t *head)
{
    std_uint_t count = 0;
    std_lock_free_list_head_t *entry;
    std_hazard_ptr_t *hp0 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 0);

    STD_ASSERT_RV(head != NULL, 0);

    entry = STD_LOCK_FREE_LIST_STRIP_MARK(head->next);

    while (entry) {
        std_hazard_set_ptr(hp0, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(entry) - head->offset);
        if (!STD_LOCK_FREE_LIST_HAS_MARK(entry->next)) {
            count++;
        }
        entry = STD_LOCK_FREE_LIST_STRIP_MARK(entry->next);
    }

    std_hazard_set_ptr(hp0, NULL);

    return count;
}

/**
 * std_lock_free_list_cleanup
 * @brief   
 * @param   head
 * @return  std_rv_t
 */
std_rv_t std_lock_free_list_cleanup(IN std_lock_free_list_head_t *head)
{
    std_lock_free_list_head_t *entry;
    std_hazard_ptr_t *hp0 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 0);
    std_u64_t key;

    STD_ASSERT_RV(head != NULL, STD_RV_ERR_INVALIDARG);

    entry = STD_LOCK_FREE_LIST_STRIP_MARK(head->next);

    while (entry) {
        std_hazard_set_ptr(hp0, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(entry) - head->offset);
        if (!STD_LOCK_FREE_LIST_HAS_MARK(entry->next)) {
            key = entry->key;
            entry = STD_LOCK_FREE_LIST_STRIP_MARK(entry->next);
            std_lock_free_list_del(head, key);
        } else {
            entry = STD_LOCK_FREE_LIST_STRIP_MARK(entry->next);
        }
    }

    std_hazard_set_ptr(hp0, NULL);

    if (head->hazard_head) {
        std_hazard_gc((std_hazard_head_t *) head->hazard_head);
        FREE(head->hazard_head);
        head->hazard_head = NULL;
    }

    return STD_RV_SUC;
}

/**
 * std_lock_free_list_inline_find
 * @brief   
 * @param   head
 * @param   key
 * @param   offset
 * @param   prev_entry
 * @param   curr_entry
 * @param   next_entry
 * @return  std_bool_t
 */
std_bool_t std_lock_free_list_inline_find(IN std_lock_free_list_head_t *head,
                                          IN std_u64_t key,
                                          IN std_uint_t offset,
                                          INOUT std_lock_free_list_head_t **prev_entry,
                                          INOUT std_lock_free_list_head_t **curr_entry,
                                          INOUT std_lock_free_list_head_t **next_entry)
{
    std_lock_free_list_head_t *prev = NULL;
    std_lock_free_list_head_t *cur = NULL;
    std_lock_free_list_head_t *next = NULL;
    std_u64_t cur_key;
    std_hazard_ptr_t *hp0 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 0);
    std_hazard_ptr_t *hp1 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 1);
    std_hazard_ptr_t *hp2 = std_hazard_get((std_hazard_head_t *) head->hazard_head, 2);
    std_int_t cur_mark;
    std_bool_t race_condition = STD_BOOL_FALSE;

    do {
        prev = head;
        cur = prev->next;

        std_hazard_set_ptr(hp1, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(cur) - offset);
        if (prev->next != STD_LOCK_FREE_LIST_STRIP_MARK(cur)) {
            //multiple threads race condition
            continue;
        }
        race_condition = STD_BOOL_FALSE;
        while (race_condition == STD_BOOL_FALSE) {
            if (cur == NULL) {
                //list is empty or at the end of list.
                *prev_entry = prev;
                *curr_entry = cur;
                return STD_BOOL_FALSE;
            }
            next = STD_LOCK_FREE_LIST_STRIP_MARK(cur->next);
            cur_mark = STD_LOCK_FREE_LIST_HAS_MARK(next);

            std_hazard_set_ptr(hp0, (std_uchar_t *) next - offset);

            if (cur->next != next) {
                //race condition.
                race_condition = STD_BOOL_TRUE;
                continue;
            }

            cur_key = cur->key;

            if (prev->next != STD_LOCK_FREE_LIST_STRIP_MARK(cur)) {
                //race condition,
                race_condition = STD_BOOL_TRUE;
                continue;
            }

            if (cur_mark == 0) {
                if (head->key_enable) {
                    if (cur_key >= key) {
                        *prev_entry = prev;
                        *curr_entry = cur;
                        *next_entry = next;
                        return cur_key == key;
                    }
                } else {
                    if (cur_key == key) {
                        *prev_entry = prev;
                        *curr_entry = cur;
                        *next_entry = next;
                        return cur_key == key;
                    }
                }

                prev = cur;
                std_hazard_set_ptr(hp2, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(cur) - offset);
            } else {
                if (CAS(&prev->next, STD_LOCK_FREE_LIST_STRIP_MARK(cur), STD_LOCK_FREE_LIST_STRIP_MARK(next))) {
                    if (head->free_enable) {
                        std_hazard_defer_free((std_hazard_head_t *) head->hazard_head, (std_uchar_t *) cur - offset);
                    }
                } else {
                    race_condition = STD_BOOL_TRUE;
                    continue;
                }
            }
            cur = next;
            std_hazard_set_ptr(hp1, (std_uchar_t *) STD_LOCK_FREE_LIST_STRIP_MARK(next) - offset);
        }
    } while (STD_BOOL_TRUE);
}