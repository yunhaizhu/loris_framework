/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_def.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */

#ifndef SLOWLORIS_STD_DEF_H
#define SLOWLORIS_STD_DEF_H

#define IN attr_unused
#define OUT attr_unused
#define INOUT attr_unused

#define STD_CALL attr_unused

#define STD_BOOL_TRUE true
#define STD_BOOL_FALSE false

#define TRUE true
#define FALSE false

#define BUF_SIZE_8 8
#define BUF_SIZE_16 16
#define BUF_SIZE_32 32
#define BUF_SIZE_64 64
#define BUF_SIZE_128 128
#define BUF_SIZE_256 256
#define BUF_SIZE_512 512
#define BUF_SIZE_1024 1024
#define BUF_SIZE_2048 2048
#define BUF_SIZE_4096 4096
#define BUF_SIZE_8192 8192

typedef enum std_rv_error_code_e {
    STD_RV_SUC = 0,
    STD_RV_ERR_UNEXPECTED = -1,   /* Unexpected error */
    STD_RV_ERR_NOTIMPL = -2,      /* Not implemented */
    STD_RV_ERR_NOINTERFACE = -3,  /* Interface not supported */
    STD_RV_ERR_POINTER = -3,      /* Bad pointer */
    STD_RV_ERR_ABART = -5,        /* Operation aborted */
    STD_RV_ERR_FAIL = -6,         /* Operation failed */
    STD_RV_ERR_ACCESSDENIED = -7, /* Access denied */
    STD_RV_ERR_OUTOFMEMORY = -8,  /* Out of memory */
    STD_RV_ERR_INVALIDARG = -9    /* Invalid argument */
} std_rv_error_code_t;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define MALLOC(size) malloc(size)
#define CALLOC(size, size_t) calloc(size, size_t)
#define FREE(ptr) free(ptr)

#define TICK(t)                                         \
    do {                                                \
        struct timespec x;                              \
        clock_gettime(CLOCK_REALTIME, &x);              \
        t = x.tv_nsec + 1000000000 * (x.tv_sec & 0xff); \
    } while (0)

#define TOCK(t)         \
    do {                \
        std_u64_t t1;   \
        TICK(t1);       \
        (t) = t1 - (t); \
    } while (0)

/* Extract bits from a 32-bit values */
static inline std_int_t bits(std_u32_t val, std_int_t start, std_int_t end)
{
    std_u32_t rval = val >> start;
    std_u32_t mask = (1 << (end - start + 1)) - 1;

    return (std_int_t)(rval & mask);
}


/* Sign-extension */
static forced_inline std_64_t sign_extend(std_64_t x,int len)
{
    len = 64 - len;
    return (x << len) >> len;
}

/* Sign-extension (32-bit) */
static forced_inline std_32_t sign_extend_32(std_32_t x,int len)
{
    len = 32 - len;
    return (x << len) >> len;
}

/* Endianness */
#define ARCH_BIG_ENDIAN     0x4321
#define ARCH_LITTLE_ENDIAN  0x1234

#if defined(__i386) || defined(__i386__) || defined(i386)
#define HOST_BYTE_ORDER ARCH_LITTLE_ENDIAN
#elif defined(__x86_64__)
#define HOST_BYTE_ORDER ARCH_LITTLE_ENDIAN
#elif defined(__mips__)
#define HOST_BYTE_ORDER ARCH_BIG_ENDIAN
#endif

/*Guest endian*/
#define GUEST_BYTE_ORDER  ARCH_LITTLE_ENDIAN

/* Host to VM conversion functions */
#if HOST_BYTE_ORDER == GUEST_BYTE_ORDER
#define htovm16(x) (x)
#define htovm32(x) (x)
#define htovm64(x) (x)

#define vmtoh16(x) (x)
#define vmtoh32(x) (x)
#define vmtoh64(x) (x)
#elif HOST_BYTE_ORDER==ARCH_LITTLE_ENDIAN

//host:little guest:big
#define htovm16(x) (htons(x))
#define htovm32(x) (htonl(x))
#define htovm64(x) (swap64(x))

#define vmtoh16(x) (ntohs(x))
#define vmtoh32(x) (ntohl(x))
#define vmtoh64(x) (swap64(x))
#else
//host:big guest:little

#define htovm16(x) (ntohs(x))
#define htovm32(x) (ntohl(x))
#define htovm64(x) (swap64(x))

#define vmtoh16(x) (htons(x))
#define vmtoh32(x) (htonl(x))
#define vmtoh64(x) (swap64(x))
#endif

#endif
