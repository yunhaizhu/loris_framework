/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_lock.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */

#ifndef SLOWLORIS_STD_LOCK_H
#define SLOWLORIS_STD_LOCK_H

#define CAS_V(addr, old, x) __sync_val_compare_and_swap(addr, old, x)
//#define CAS(addr, old, x) (__sync_bool_compare_and_swap(addr, old, x) == TRUE)
#define CAS(addr, old, x) (__sync_bool_compare_and_swap(addr, old, x))
#define ATOMIC_INC(addr) __sync_add_and_fetch(addr, 1)
#define ATOMIC_DEC(addr) __sync_add_and_fetch(addr, -1)
#define ATOMIC_ADD(addr, n) __sync_add_and_fetch(addr, n)

#define INC_ATOMIC(addr) __sync_add_and_fetch(addr, 1)
#define DEC_ATOMIC(addr) __sync_add_and_fetch(addr, -1)

typedef sem_t std_mutex_t;
typedef sem_t std_sem_t;
typedef pthread_rwlock_t std_rwlock_t;
typedef volatile int std_cas_t;
typedef pthread_mutex_t std_pthread_mutex_t;

#define std_mutex_init(sem) sem_init(sem, 0, 1)
#define std_mutex_lock sem_wait
#define std_mutex_unlock sem_post

#define std_sem_init(sem, val) sem_init(sem, 0, val)
#define std_sem_lock sem_wait
#define std_sem_unlock sem_post
#define std_sem_getvalue(sem, val) sem_getvalue(sem, val)

#define std_rwlock_init(rwlock) pthread_rwlock_init(rwlock, NULL)
#define std_rwlock_wrlock pthread_rwlock_wrlock
#define std_rwlock_rdlock pthread_rwlock_rdlock
#define std_rwlock_unlock pthread_rwlock_unlock
#define std_rwlock_trywrlock pthread_rwlock_trywrlock
#define std_rwlock_tryrdlock pthread_rwlock_tryrdlock

#define std_cas_init(caslock) \
    {                         \
        *(caslock) = 0;       \
    }
#define std_cas_lock(caslock)                              \
    while (!(__sync_bool_compare_and_swap(caslock, 0, 1))) \
        usleep(1);
#define std_cas_unlock(caslock) __sync_bool_compare_and_swap(caslock, 1, 0);

#define std_pthread_mutex_init(pthread_mutex) \
    pthread_mutex_init(pthread_mutex, NULL)
#define std_pthread_mutex_lock pthread_mutex_lock
#define std_pthread_mutex_unlock pthread_mutex_unlock

#endif
