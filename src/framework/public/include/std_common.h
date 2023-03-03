/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_common.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */

#ifndef SLOWLORIS_STD_COMMON_H
#define SLOWLORIS_STD_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pthread.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "std_typedef.h"
#include "std_assert.h"
#include "std_def.h"
#include "std_lock.h"
#include "std_log.h"
#include "std_random.h"
#include "std_safe.h"

#define VERSION_NUMBER 20230302

#ifdef __cplusplus
}
#endif
#endif
