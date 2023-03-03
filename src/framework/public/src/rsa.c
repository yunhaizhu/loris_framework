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
* @file    rsa.c
* @brief   implement functions
* @version 1.0
* @author  Yunhai Zhu
* @date    2021-10-19
*
*/
#include <gsl/gsl_rng.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int MAX_DIGITS = 50;

struct public_key_class {
    long long modulus;
    long long exponent;
};

struct private_key_class {
    long long modulus;
    long long exponent;
};


// This should totally be in the math library.
/**
* gcd
* @brief
* @param   a
* @param   b
* @return  long long
*/
long long gcd(long long a, long long b)
{
    long long c;
    while (a != 0) {
        c = a;
        a = b % a;
        b = c;
    }
    return b;
}


/**
* ExtEuclid
* @brief
* @param   a
* @param   b
* @return  long long
*/
long long ExtEuclid(long long a, long long b)
{
    long long x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
    while (a != 0) {
        q = gcd / a;
        r = gcd % a;
        m = x - u * q;
        n = y - v * q;
        gcd = a;
        a = r;
        x = u;
        y = v;
        u = m;
        v = n;
    }
    return y;
}

/**
* modmult
* @brief
* @param   a
* @param   b
* @param   mod
* @return  static inline long long
*/
static inline long long modmult(long long a, long long b, long long mod)
{
    // this is necessary since we will be dividing by a
    if (a == 0) {
        return 0;
    }
    register long long product = a * b;
    //if multiplication does not overflow, we can use it
    if (product / a == b) {
        return product % mod;
    }
    // if a % 2 == 1 i. e. a >> 1 is not a / 2
    if (a & 1) {
        product = modmult((a >> 1), b, mod);
        if ((product << 1) > product) {
            return (((product << 1) % mod) + b) % mod;
        }
    }
    //implicit else
    product = modmult((a >> 1), b, mod);
    if ((product << 1) > product) {
        return (product << 1) % mod;
    }
    //implicit else: this is about 10x slower than the code above, but it will not overflow
    long long sum;
    sum = 0;
    while (b > 0) {
        if (b & 1)
            sum = (sum + a) % mod;
        a = (2 * a) % mod;
        b >>= 1;
    }
    return sum;
}

/**
* rsa_modExp
* @brief
* @param   b
* @param   e
* @param   m
* @return  long long
*/
long long rsa_modExp(long long b, long long e, long long m)
{
    long long product;
    product = 1;
    if (b < 0 || e < 0 || m <= 0) {
        return -1;
    }
    b = b % m;
    while (e > 0) {
        if (e & 1) {
            product = modmult(product, b, m);
        }
        b = modmult(b, b, m);
        e >>= 1;
    }
    return product;
}

// Calling this function will generate a include and private key and store them in the pointers
// it is given.
/**
* rsa_gen_keys
* @brief
* @param   pub
* @param   priv
* @param   PRIME_SOURCE_FILE
*/
void rsa_gen_keys(struct public_key_class *pub, struct private_key_class *priv, char *PRIME_SOURCE_FILE)
{
    static int prime_table[10240];
    static long long prime_count = 0;
    FILE *primes_list;
    char prime_buffer[MAX_DIGITS];

    if (prime_count == 0) {
        if (!(primes_list = fopen(PRIME_SOURCE_FILE, "r"))) {
            fprintf(stderr, "Problem reading %s\n", PRIME_SOURCE_FILE);
            exit(1);
        }
        while (1) {
            if (fgets(prime_buffer, (int) sizeof(prime_buffer), primes_list) == NULL) {
                break;
            }
            prime_table[prime_count++] = atol(prime_buffer);
        }
        fclose(primes_list);
    }

    // choose random primes from the list, store them as p,q

    long long p = 0;
    long long q = 0;

    //values of e should be sufficiently large to protect against naive attacks
    long long e = (2 << 16) + 1;
    long long d = 0;

    long long max = 0;
    long long phi_max = 0;

    static gsl_rng *r = NULL;

    if (r == NULL) {
        r = gsl_rng_alloc(gsl_rng_default);
        gsl_rng_set(r, 32 * 3233323443);
    }

    do {
        // a and b are the positions of p and q in the list
        unsigned long long a = (gsl_rng_get(r) % (prime_count + 1));
        unsigned long long b = (gsl_rng_get(r) % (prime_count + 1));

        p = prime_table[a];
        q = prime_table[b];

        max = p * q;
        phi_max = (p - 1) * (q - 1);
    } while (!(p && q) || (p == q) || (gcd(phi_max, e) != 1));

    // Next, we need to choose a,b, so that a*max+b*e = gcd(max,e). We actually only need b
    // here, and in keeping with the usual notation of RSA we'll call it d. We'd also like
    // to make sure we get a representation of d as positive, hence the while loop.
    d = ExtEuclid(phi_max, e);
    while (d < 0) {
        d = d + phi_max;
    }

    // We now store the include / private keys in the appropriate structs
    pub->modulus = max;
    pub->exponent = e;

    priv->modulus = max;
    priv->exponent = d;
}


long long *rsa_encrypt(const char *message, const unsigned long message_size,
                       const struct public_key_class *pub)
{
    long long *encrypted = malloc(sizeof(long long) * message_size);
    if (encrypted == NULL) {
        fprintf(stderr,
                "Error: Heap allocation failed.\n");
        return NULL;
    }
    long long i = 0;
    for (i = 0; i < message_size; i++) {
        if ((encrypted[i] = rsa_modExp(message[i], pub->exponent, pub->modulus)) == -1)
            return NULL;
    }
    return encrypted;
}


char *rsa_decrypt(const long long *message,
                  const unsigned long message_size,
                  const struct private_key_class *priv)
{
    if (message_size % sizeof(long long) != 0) {
        fprintf(stderr,
                "Error: message_size is not divisible by %d, so cannot be output of rsa_encrypt\n",
                (int) sizeof(long long));
        return NULL;
    }
    // We allocate space to do the decryption (temp) and space for the output as a char array
    // (decrypted)
    char *decrypted = malloc(message_size / sizeof(long long));
    char *temp = malloc(message_size);
    if ((decrypted == NULL) || (temp == NULL)) {
        if (decrypted) {
            free(decrypted);
        }

        if (temp) {
            free(temp);
        }
        fprintf(stderr,
                "Error: Heap allocation failed.\n");
        return NULL;
    }
    // Now we go through each 8-byte chunk and decrypt it.
    long long i = 0;
    for (i = 0; i < message_size / 8; i++) {
        if ((temp[i] = rsa_modExp(message[i], priv->exponent, priv->modulus)) == -1) {
            free(decrypted);
            free(temp);
            return NULL;
        }
    }
    // The result should be a number in the char range, which gives back the original byte.
    // We put that into decrypted, then return.
    for (i = 0; i < message_size / 8; i++) {
        decrypted[i] = temp[i];
    }
    free(temp);
    return decrypted;
}


/**
* rsa_ptr_encrypt
* @brief
* @param   ptr
* @param   pub
* @return  __uint128_t
*/
__uint128_t rsa_ptr_encrypt(const uint16_t ptr, const struct public_key_class *pub)
{
    __uint128_t encrypted = 0;
    __uint128_t temp2 = 0;

    for (int k = 1; k >= 0; k--) {
        uint64_t temp = 0;
        if ((temp = rsa_modExp((ptr >> 8 * k) & 0xFF, pub->exponent, pub->modulus)) == -1) {
            return 0;
        }

        temp2 = temp;
        encrypted = encrypted | temp2 << 64 * k;
    }

    return encrypted;
}


/**
* rsa_ptr_decrypt
* @brief
* @param   ptr
* @param   priv
* @return  uint16_t
*/
uint16_t rsa_ptr_decrypt(const __uint128_t ptr, const struct private_key_class *priv)
{
    uint32_t decrypted = 0;

    for (int k = 1; k >= 0; k--) {
        uint64_t temp = 0;
        if ((temp = rsa_modExp((uint64_t) ((ptr >> 64 * k) & 0xFFFFFFFFFFFFFFFF), priv->exponent, priv->modulus)) ==
            -1) {
            return 0;
        }
        decrypted = temp << 8 * k | decrypted;
    }

    return decrypted;
}
