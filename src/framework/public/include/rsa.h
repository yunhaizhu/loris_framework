/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    rsa.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#ifndef __RSA_H__
#define __RSA_H__

#include <stdint.h>

// This is the header file for the library librsaencrypt.a

// Change this line to the file you'd like to use as a source of primes.
// The format of the file should be one prime per line.
#define PRIME_SOURCE_FILE "primes.txt"

struct public_key_class {
    long long modulus;
    long long exponent;
};

struct private_key_class {
    long long modulus;
    long long exponent;
};

// This function generates include and private keys, then stores them in the structures you
// provide pointers to. The 3rd argument should be the text PRIME_SOURCE_FILE to have it use
// the location specified above in this header.
/**
 * rsa_gen_keys
 * @brief   
 * @param   pub
 * @param   priv
 * @param   PRIME_SOURCE
 */
void rsa_gen_keys(struct public_key_class *pub, struct private_key_class *priv, const char *PRIME_SOURCE);

// This function will encrypt the data pointed to by message. It returns a pointer to a heap
// array containing the encrypted data, or NULL upon failure. This pointer should be freed when
// you are finished. The encrypted data will be 8 times as large as the original data.
/**
 * rsa_encrypt
 * @brief   
 * @param   message
 * @param   message_size
 * @param   pub
 * @return  long long *
 */
long long *rsa_encrypt(const char *message, const unsigned long message_size, const struct public_key_class *pub);

// This function will decrypt the data pointed to by message. It returns a pointer to a heap
// array containing the decrypted data, or NULL upon failure. This pointer should be freed when
// you are finished. The variable message_size is the size in bytes of the encrypted message.
// The decrypted data will be 1/8th the size of the encrypted data.
/**
 * rsa_decrypt
 * @brief   
 * @param   message
 * @param   message_size
 * @param   pub
 * @return  char *
 */
char *rsa_decrypt(const long long *message, const unsigned long message_size, const struct private_key_class *pub);

/**
 * rsa_ptr_encrypt
 * @brief   
 * @param   ptr
 * @param   pub
 * @return  __uint128_t
 */
__uint128_t rsa_ptr_encrypt(const uint16_t ptr, const struct public_key_class *pub);
/**
 * rsa_ptr_decrypt
 * @brief   
 * @param   ptr
 * @param   priv
 * @return  uint16_t
 */
uint16_t rsa_ptr_decrypt(const __uint128_t ptr, const struct private_key_class *priv);
#endif
