/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_assert.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */

#ifndef SLOWLORIS_STD_ASSERT_H
#define SLOWLORIS_STD_ASSERT_H

#define STD_ASSERT_RV_LEVEL_FORMAT_MSG(cond, rv, level, format, ...)                       \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            STD_LOG(level, "ASSERT:(%s) RV:(%s) " format "\n", #cond, #rv, ##__VA_ARGS__); \
            return rv;                                                                     \
        }                                                                                  \
    } while (0)

#define STD_ASSERT_RV_LEVEL(cond, rv, level) STD_ASSERT_RV_LEVEL_FORMAT_MSG(cond, rv, level, "")
#define STD_ASSERT_RV_INFO(cond, rv) STD_ASSERT_RV_LEVEL(cond, rv, INFO)
#define STD_ASSERT_RV_DEBUG(cond, rv) STD_ASSERT_RV_LEVEL(cond, rv, DEBUG)
#define STD_ASSERT_RV_WARN(cond, rv) STD_ASSERT_RV_LEVEL(cond, rv, WARN)
#define STD_ASSERT_RV_ERR(cond, rv) STD_ASSERT_RV_LEVEL(cond, rv, ERR)
#define STD_ASSERT_RV(cond, rv) STD_ASSERT_RV_ERR(cond, rv)

#define STD_ASSERT_RV_INFO_MSG(cond, rv, format, ...) STD_ASSERT_RV_LEVEL_FORMAT_MSG(cond, rv, INFO, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_DEBUG_MSG(cond, rv, format, ...) STD_ASSERT_RV_LEVEL_FORMAT_MSG(cond, rv, DEBUG, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_WARN_MSG(cond, rv, format, ...) STD_ASSERT_RV_LEVEL_FORMAT_MSG(cond, rv, WARN, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_ERR_MSG(cond, rv, format, ...) STD_ASSERT_RV_LEVEL_FORMAT_MSG(cond, rv, ERR, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_MSG(cond, rv, format, ...) STD_ASSERT_RV_ERR_MSG(cond, rv, format, ##__VA_ARGS__)

#define STD_ASSERT_LEVEL_FORMAT_MSG(cond, level, format, ...)                 \
    do {                                                                      \
        if (!(cond)) {                                                        \
            STD_LOG(level, "ASSERT:(%s) " format "\n", #cond, ##__VA_ARGS__); \
        }                                                                     \
    } while (0)

#define STD_ASSERT_LEVEL(cond, level) STD_ASSERT_LEVEL_FORMAT_MSG(cond, level, "")
#define STD_ASSERT_INFO(cond) STD_ASSERT_LEVEL(cond, INFO)
#define STD_ASSERT_DEBUG(cond) STD_ASSERT_LEVEL(cond, DEBUG)
#define STD_ASSERT_WARN(cond) STD_ASSERT_LEVEL(cond, WARN)
#define STD_ASSERT_ERR(cond) STD_ASSERT_LEVEL(cond, ERR)
#define STD_ASSERT(cond) STD_ASSERT_ERR(cond)

#define STD_ASSERT_INFO_MSG(cond, format, ...) STD_ASSERT_LEVEL_FORMAT_MSG(cond, INFO, format, ##__VA_ARGS__)
#define STD_ASSERT_DEBUG_MSG(cond, format, ...) STD_ASSERT_LEVEL_FORMAT_MSG(cond, DEBUG, format, ##__VA_ARGS__)
#define STD_ASSERT_WARN_MSG(cond, format, ...) STD_ASSERT_LEVEL_FORMAT_MSG(cond, WARN, format, ##__VA_ARGS__)
#define STD_ASSERT_ERR_MSG(cond, format, ...) STD_ASSERT_LEVEL_FORMAT_MSG(cond, ERR, format, ##__VA_ARGS__)
#define STD_ASSERT_MSG(cond, format, ...) STD_ASSERT_ERR_MSG(cond, format, ##__VA_ARGS__)


#define STD_ASSERT_RV_LEVEL_ACTION_FORMAT_MSG(cond, rv, level, action, format, ...)        \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            STD_LOG(level, "ASSERT:(%s) RV:(%s) " format "\n", #cond, #rv, ##__VA_ARGS__); \
            {                                                                              \
                action;                                                                    \
            }                                                                              \
            return rv;                                                                     \
        }                                                                                  \
    } while (0)

#define STD_ASSERT_RV_LEVEL_ACTION(cond, rv, level, action) STD_ASSERT_RV_LEVEL_ACTION_FORMAT_MSG(cond, rv, level, action, "")
#define STD_ASSERT_RV_INFO_ACTION(cond, rv, action) STD_ASSERT_RV_LEVEL_ACTION(cond, rv, INFO, action)
#define STD_ASSERT_RV_DEBUG_ACTION(cond, rv, action) STD_ASSERT_RV_LEVEL_ACTION(cond, rv, DEBUG, action)
#define STD_ASSERT_RV_WARN_ACTION(cond, rv, action) STD_ASSERT_RV_LEVEL_ACTION(cond, rv, WARN, action)
#define STD_ASSERT_RV_ERR_ACTION(cond, rv, action) STD_ASSERT_RV_LEVEL_ACTION(cond, rv, ERR, action)
#define STD_ASSERT_RV_ACTION(cond, rv, action) STD_ASSERT_RV_ERR_ACTION(cond, rv, action)

#define STD_ASSERT_RV_INFO_ACTION_MSG(cond, rv, action, format, ...) STD_ASSERT_RV_LEVEL_ACTION_FORMAT_MSG(cond, rv, INFO, action, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_DEBUG_ACTION_MSG(cond, rv, action, format, ...) STD_ASSERT_RV_LEVEL_ACTION_FORMAT_MSG(cond, rv, DEBUG, action, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_WARN_ACTION_MSG(cond, rv, action, format, ...) STD_ASSERT_RV_LEVEL_ACTION_FORMAT_MSG(cond, rv, WARN, action, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_ERR_ACTION_MSG(cond, rv, action, format, ...) STD_ASSERT_RV_LEVEL_ACTION_FORMAT_MSG(cond, rv, ERR, action, format, ##__VA_ARGS__)
#define STD_ASSERT_RV_ACTION_MSG(cond, rv, action, format, ...) STD_ASSERT_RV_ERR_ACTION_MSG(cond, rv, format, action, ##__VA_ARGS__)

#define STD_ASSERT_LEVEL_ACTION_FORMAT_MSG(cond, level, action, format, ...)  \
    do {                                                                      \
        if (!(cond)) {                                                        \
            STD_LOG(level, "ASSERT:(%s) " format "\n", #cond, ##__VA_ARGS__); \
            {                                                                 \
                action;                                                       \
            }                                                                 \
        }                                                                     \
    } while (0)

#define STD_ASSERT_LEVEL_ACTION(cond, level, action) STD_ASSERT_LEVEL_ACTION_FORMAT_MSG(cond, level, action, "")
#define STD_ASSERT_INFO_ACTION(cond, action) STD_ASSERT_LEVEL_ACTION(cond, INFO, action)
#define STD_ASSERT_DEBUG_ACTION(cond, action) STD_ASSERT_LEVEL_ACTION(cond, DEBUG, action)
#define STD_ASSERT_WARN_ACTION(cond, action) STD_ASSERT_LEVEL_ACTION(cond, WARN, action)
#define STD_ASSERT_ERR_ACTION(cond, action) STD_ASSERT_LEVEL_ACTION(cond, ERR, action)
#define STD_ASSERT_ACTION(cond, action) STD_ASSERT_ERR_ACTION(cond, action)

#define STD_ASSERT_INFO_ACTION_MSG(cond, action, format, ...) STD_ASSERT_LEVEL_ACTION_FORMAT_MSG(cond, INFO, action, format, ##__VA_ARGS__)
#define STD_ASSERT_DEBUG_ACTION_MSG(cond, action, format, ...) STD_ASSERT_LEVEL_ACTION_FORMAT_MSG(cond, DEBUG, action, format, ##__VA_ARGS__)
#define STD_ASSERT_WARN_ACTION_MSG(cond, action, format, ...) STD_ASSERT_LEVEL_ACTION_FORMAT_MSG(cond, WARN, action, format, ##__VA_ARGS__)
#define STD_ASSERT_ERR_ACTION_MSG(cond, action, format, ...) STD_ASSERT_LEVEL_ACTION_FORMAT_MSG(cond, ERR, action, format, ##__VA_ARGS__)
#define STD_ASSERT_ACTION_MSG(cond, action, format, ...) STD_ASSERT_ERR_ACTION_MSG(cond, action, format, ##__VA_ARGS__)

#endif