/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_safe.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#include "std_common.h"

/**
 * std_safe_strlen
 * @brief   
 * @param   str
 * @param   n
 * @return  std_size_t
 */
STD_CALL std_size_t std_safe_strlen(IN const std_char_t *str, IN std_size_t n)
{
    const std_char_t *found = memchr(str, '\0', n);
    return found ? (std_size_t) (found - str) : n;
}

/**
 * std_safe_atoi
 * @brief   
 * @param   str
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t std_safe_atoi(IN const std_char_t *str)
{
    std_char_t *ptr;
    std_int_t ret;

    ret = (std_int_t) strtol(str, &ptr, 10);

    return ret;
}

/**
 * std_safe_strip_chars
 * @brief
 * @param   str
 * @param   strip
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t std_safe_strip_chars(IN std_char_t *str, IN std_char_t strip)
{
    const std_char_t *p;
    std_char_t *q;
    std_int_t len = 0;

    p = str;
    q = str;

    for (; *p; p++) {
        if (*p != strip) {
            len += 1;
            *q++ = *p;
        }
    }
    *q = '\0';

    return len;
}


/**
 * std_safe_replace_chars
 * @brief   
 * @param   str
 * @param   strip
 * @param   replace
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t std_safe_replace_chars(IN std_char_t *str, IN std_char_t strip, IN std_char_t replace)
{
    const std_char_t *p;
    std_char_t *q;
    std_int_t len = 0;

    p = str;
    q = str;

    for (; *p; p++) {
        if (*p != strip) {
            len += 1;
            *q++ = *p;
        } else {
            len += 1;
            *q++ = replace;
        }
    }
    *q = '\0';

    return len;
}