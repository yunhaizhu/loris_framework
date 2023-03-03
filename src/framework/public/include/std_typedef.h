/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_typedef.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */
#ifndef SLOWLORIS_STD_TYPEDEF_H
#define SLOWLORIS_STD_TYPEDEF_H

#define forced_inline inline __attribute__((always_inline))
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#define fastcall __attribute__((regparm(3)))
#define attr_unused __attribute__((unused))

attr_unused typedef uint8_t std_u8_t;
attr_unused typedef uint16_t std_u16_t;
attr_unused typedef uint32_t std_u32_t;
attr_unused typedef uint64_t std_u64_t;
attr_unused typedef __uint128_t std_u128_t;

attr_unused typedef int8_t std_8_t;
attr_unused typedef int16_t std_16_t;
attr_unused typedef int32_t std_32_t;
attr_unused typedef int64_t std_64_t;
attr_unused typedef __int128_t std_128_t;

attr_unused typedef int32_t std_int_t;
attr_unused typedef uint32_t std_uint_t;
attr_unused typedef long std_long_t;
attr_unused typedef unsigned long std_ulong_t;

attr_unused typedef float std_float_t;
attr_unused typedef uint8_t std_byte_t;
attr_unused typedef char std_char_t;
attr_unused typedef unsigned char std_uchar_t;

attr_unused typedef void std_void_t;

attr_unused typedef size_t std_size_t;
attr_unused typedef int std_status_t;
attr_unused typedef bool std_bool_t;
attr_unused typedef int std_rv_t;

#endif
