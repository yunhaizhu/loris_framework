/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_object.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#ifndef NONCLEDB_VIRTUAL_MACHINE_OBJECT_H
#define NONCLEDB_VIRTUAL_MACHINE_OBJECT_H

#include "rsa.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "std_lock_free_list.h"

#define TOKEN_SIGNATURE_NUMBER 4
#define NAN_BOX

#ifdef NAN_BOX
typedef std_u64_t own_value_t;

typedef enum own_value_type_s {
    OWN_TYPE_NULL,
    OWN_TYPE_NUMBER,
    OWN_TYPE_DOUBLE,
    OWN_TYPE_BOOL,
    OWN_TYPE_ADDRESS,
    OWN_TYPE_OBJECT,
    OWN_TYPE_OBJECT_SYMBOL,
    OWN_TYPE_OBJECT_STRING,
} own_value_type_t;

#define NAN_BOX_MASK_SIGN 0x8000000000000000
#define NAN_BOX_MASK_EXPONENT 0x7ff0000000000000
#define NAN_BOX_MASK_QUIET 0x0008000000000000
#define NAN_BOX_MASK_TYPE 0x0007000000000000
#define NAN_BOX_MASK_SIGNATURE 0xffff000000000000
#define NAN_BOX_MASK_PAYLOAD_PTR 0x0000ffffffffffff

#define NAN_BOX_MASK_TYPE_NAN 0x0000000000000000
#define NAN_BOX_MASK_TYPE_FALSE 0x0001000000000000
#define NAN_BOX_MASK_TYPE_TRUE 0x0002000000000000
#define NAN_BOX_MASK_TYPE_NULL 0x0003000000000000
#define NAN_BOX_MASK_TYPE_ADDRESS 0x0004000000000000
#define NAN_BOX_MASK_TYPE_OBJECT 0x0005000000000000
#define NAN_BOX_MASK_TYPE_OBJECT_SYMBOL 0x0006000000000000
#define NAN_BOX_MASK_TYPE_OBJECT_STRING 0x0007000000000000

#define NAN_BOX_NaN (NAN_BOX_MASK_EXPONENT | NAN_BOX_MASK_QUIET)
#define NAN_BOX_Null (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_NULL)
#define NAN_BOX_False (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_FALSE)
#define NAN_BOX_True (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_TRUE)

#define NAN_BOX_SIGNATURE_NAN NAN_BOX_NaN
#define NAN_BOX_SIGNATURE_NULL NAN_BOX_Null
#define NAN_BOX_SIGNATURE_FALSE NAN_BOX_False
#define NAN_BOX_SIGNATURE_TRUE NAN_BOX_True

#define NAN_BOX_SIGNATURE_ADDRESS (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_ADDRESS)
#define NAN_BOX_SIGNATURE_OBJECT (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_OBJECT)
#define NAN_BOX_SIGNATURE_OBJECT_SYMBOL (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_OBJECT_SYMBOL)
#define NAN_BOX_SIGNATURE_OBJECT_STRING (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_OBJECT_STRING)

#else
typedef union own_value_union {
    std_64_t i64;
    double float64;
    std_void_t *ptr;
} own_value_union;

typedef enum own_value_tag_s {
    TAG_NUMBER,
    TAG_DOUBLE,
    TAG_BOOL,
    TAG_ADDRESS,
    TAG_SYMBOL,
    TAG_STRING,
    TAG_OBJECT
} own_value_tag_t;

typedef struct own_value {
    int64_t tag;
    own_value_union u;
} own_value_t;
#endif

typedef struct ownership_token_signature_s {
    std_u16_t token[TOKEN_SIGNATURE_NUMBER];
    std_u128_t signature[TOKEN_SIGNATURE_NUMBER];
} ownership_token_signature_t;

typedef struct ownership_object_s {
    own_value_t value;
    own_value_t own_value;
    ownership_token_signature_t owner_token_signature;
    std_lock_free_list_head_t list;
} ownership_object_t;

typedef enum {
    var_type,
    array_type,
    tuple_type,
    hash_type,
    func_type,
} symbol_type_t;

typedef struct var_s {
    own_value_t value;
} val_t;

typedef struct array_s {
    std_int_t array_length;
    own_value_t *array;
} array_t;

typedef struct tuple_s {
    std_lock_free_list_head_t *tuples;
} tuple_t;

typedef struct hash_s {
    std_lock_free_key_hash_t *hash_head;
} hash_t;

typedef struct func_s {

} func_t;

typedef struct source_data_s {
    std_int_t line;
} source_data_t;

typedef struct env_value_s {
    symbol_type_t symbol_type;
    union {
        val_t val;
        array_t array;
        tuple_t tuple;
        hash_t hash;
        func_t func;
    } data;
} env_value_t;

typedef struct ownership_object_symbol_s {
    env_value_t env_value;

    struct public_key_class pub;
    struct private_key_class pri;
} ownership_object_symbol_t;

/**
 * make_own_value_number
 * @brief   
 * @param   num
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_number(IN const std_64_t num)
{
#ifdef NAN_BOX
    return *(own_value_t *) (&num);
#else
    own_value_t value;
    value.tag = TAG_NUMBER;
    value.u.i64 = num;

    return value;
#endif
}

/**
 * make_own_value_bool
 * @brief   
 * @param   val
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_bool(IN const std_bool_t val)
{
#ifdef NAN_BOX
    return val ? NAN_BOX_True : NAN_BOX_False;
#else
    own_value_t value;
    value.tag = TAG_BOOL;
    value.u.i64 = val;

    return value;
#endif
}

/**
 * make_own_value_float
 * @brief   
 * @param   val
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_float(IN const double val)
{
#ifdef NAN_BOX
    union {
        double d;
        uint64_t u64;
    } u;
    own_value_t v;
    u.d = val;

    v = u.u64;
    return v;
#else
    own_value_t value;
    value.tag = TAG_DOUBLE;
    value.u.float64 = val;

    return value;
#endif
}


/**
 * make_own_value_object_string
 * @brief   
 * @param   str
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_object_string(IN const std_char_t *str)
{
#ifdef NAN_BOX
    ownership_object_t *object = (ownership_object_t *) CALLOC(sizeof(ownership_object_t), 1);

    object->value = (uint64_t) strdup(str) | NAN_BOX_SIGNATURE_ADDRESS;
    return NAN_BOX_SIGNATURE_OBJECT_STRING | (uint64_t) object;
#else
    own_value_t value;
    own_object_t *object = (own_object_t *) CALLOC(sizeof(own_object_t), 1);

    object->value.tag = TAG_STRING;
    object->value.u.ptr = strdup(str);

    value.tag = TAG_OBJ;
    value.u.ptr = object;

    return value;
#endif
}

/**
 * make_own_value_string
 * @brief   
 * @param   str
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_string(IN const std_char_t *str)
{
#ifdef NAN_BOX
    std_char_t *value = strdup(str);
    return NAN_BOX_SIGNATURE_ADDRESS | (uint64_t) value;
#else
    own_value_t value;
    own_object_t *object = (own_object_t *) CALLOC(sizeof(own_object_t), 1);

    object->value.tag = TAG_STRING;
    object->value.u.ptr = strdup(str);

    value.tag = TAG_OBJ;
    value.u.ptr = object;

    return value;
#endif
}


/**
 * make_own_value_address
 * @brief   
 * @param   ptr
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_address(IN std_void_t *ptr)
{
#ifdef NAN_BOX
    return NAN_BOX_SIGNATURE_ADDRESS | (uint64_t) ptr;
#else
    own_value_t value;
    value.tag = TAG_ADDRESS;
    value.u.ptr = ptr;

    return value;
#endif
}

/**
 * make_own_value_object_symbol
 * @brief   
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_object_symbol()
{
#ifdef NAN_BOX
    ownership_object_t *object = (ownership_object_t *) CALLOC(sizeof(ownership_object_t), 1);
    ownership_object_symbol_t *symbol = (ownership_object_symbol_t *) CALLOC(sizeof(ownership_object_symbol_t), 1);
    rsa_gen_keys(&symbol->pub, &symbol->pri, PRIME_SOURCE_FILE);

    object->value = NAN_BOX_SIGNATURE_ADDRESS | (uint64_t) symbol;

    return NAN_BOX_SIGNATURE_OBJECT_SYMBOL | (uint64_t) object;
#else
    own_value_t value;
    own_object_t *object = (own_object_t *) CALLOC(sizeof(own_object_t), 1);
    own_object_symbol_t *symbol = (own_object_symbol_t *) CALLOC(sizeof(own_object_symbol_t), 1);
    rsa_gen_keys(&symbol->pub, &symbol->pri, PRIME_SOURCE_FILE);

    object->value.tag = TAG_SYMBOL;
    object->value.u.ptr = symbol;

    value.tag = TAG_OBJ;
    value.u.ptr = object;

    return value;
#endif
}

/**
 * make_own_value_object
 * @brief   
 * @param   val
 * @return  STD_CALL static inline own_value_t
 */
STD_CALL static inline own_value_t make_own_value_object(IN const own_value_t val)
{
#ifdef NAN_BOX
    ownership_object_t *object = (ownership_object_t *) CALLOC(sizeof(ownership_object_t), 1);
    object->value = val;

    return NAN_BOX_SIGNATURE_OBJECT | (uint64_t) object;
#else
    own_value_t value;
    own_object_t *object = (own_object_t *) CALLOC(sizeof(own_object_t), 1);

    object->value = val;

    value.tag = TAG_OBJ;
    value.u.ptr = object;

    return value;
#endif
}

/**
 * get_own_value_number
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_64_t
 */
STD_CALL static inline std_64_t get_own_value_number(IN const own_value_t value)
{
#ifdef NAN_BOX
    std_u64_t isNaN = NAN_BOX_SIGNATURE_NAN & value;

//    assert(isNaN != NAN_BOX_SIGNATURE_NAN);
//DEBUG HIGH
    return *(std_64_t *) (&value);
#else
    return value.u.i64;
#endif
}

/**
 * get_own_value_bool
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_bool_t
 */
STD_CALL static inline std_bool_t get_own_value_bool(IN const own_value_t value)
{
#ifdef NAN_BOX
    assert(value == NAN_BOX_True || value == NAN_BOX_False);
    return value == NAN_BOX_True ? STD_BOOL_TRUE : STD_BOOL_FALSE;
#else
    return value.u.i64;
#endif
}

/**
 * get_own_value_float
 * @brief   
 * @param   value
 * @return  STD_CALL static inline double
 */
STD_CALL static inline double get_own_value_float(IN const own_value_t value)
{
#ifdef NAN_BOX
    std_u64_t isNaN = NAN_BOX_SIGNATURE_NAN & value;

    assert(isNaN != NAN_BOX_SIGNATURE_NAN);

    union {
        own_value_t v;
        double d;
    } u;
    u.v = value;

    return u.d;
#else
    return value.u.float64;
#endif
}

/**
 * get_own_value_object_string
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_char_t *
 */
STD_CALL static inline std_char_t *get_own_value_object_string(IN const own_value_t value)
{
#ifdef NAN_BOX
    assert(NAN_BOX_SIGNATURE_OBJECT_STRING == (value & NAN_BOX_MASK_SIGNATURE));
    ownership_object_t *object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);

    assert(NAN_BOX_SIGNATURE_ADDRESS == (object->value & NAN_BOX_MASK_SIGNATURE));

    return (std_char_t *) (object->value & NAN_BOX_MASK_PAYLOAD_PTR);
#else
    own_object_t *object = (own_object_t *) value.u.ptr;

    assert(object->value.tag == TAG_STRING);
    return object->value.u.ptr;
#endif
}

/**
 * get_own_value_string
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_char_t *
 */
STD_CALL static inline std_char_t *get_own_value_string(IN const own_value_t value)
{
#ifdef NAN_BOX
    std_u64_t signature = value & NAN_BOX_MASK_SIGNATURE;
    assert(NAN_BOX_SIGNATURE_ADDRESS == signature);

    return (std_char_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);
#else
    own_object_t *object = (own_object_t *) value.u.ptr;

    assert(object->value.tag == TAG_STRING);
    return object->value.u.ptr;
#endif
}

/**
 * get_own_value_address
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_void_t *
 */
STD_CALL static inline std_void_t *get_own_value_address(IN const own_value_t value)
{
#ifdef NAN_BOX
    std_u64_t signature = value & NAN_BOX_MASK_SIGNATURE;

    assert(NAN_BOX_SIGNATURE_ADDRESS == signature);

    return (std_void_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);
#else
    return value.u.ptr;
#endif
}

/**
 * get_own_value_object_symbol
 * @brief   
 * @param   value
 * @return  STD_CALL static inline ownership_object_symbol_t *
 */
STD_CALL static inline ownership_object_symbol_t *get_own_value_object_symbol(IN const own_value_t value)
{
#ifdef NAN_BOX
    assert(NAN_BOX_SIGNATURE_OBJECT_SYMBOL == (value & NAN_BOX_MASK_SIGNATURE));
    ownership_object_t *object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);

    assert(NAN_BOX_SIGNATURE_ADDRESS == (object->value & NAN_BOX_MASK_SIGNATURE));
    return (ownership_object_symbol_t *) (object->value & NAN_BOX_MASK_PAYLOAD_PTR);
#else
    own_object_t *object = (own_object_t *) value.u.ptr;

    assert(object->value.tag == TAG_SYMBOL);
    return object->value.u.ptr;
#endif
}

/**
 * get_own_value_symbol
 * @brief   
 * @param   value
 * @return  STD_CALL static inline ownership_object_symbol_t *
 */
STD_CALL static inline ownership_object_symbol_t *get_own_value_symbol(IN const own_value_t value)
{
#ifdef NAN_BOX
    assert(NAN_BOX_SIGNATURE_ADDRESS == (value & NAN_BOX_MASK_SIGNATURE));
    return (ownership_object_symbol_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);
#else
    own_object_t *object = (own_object_t *) value.u.ptr;

    assert(object->value.tag == TAG_SYMBOL);
    return object->value.u.ptr;
#endif
}

/**
 * get_own_value_object
 * @brief   
 * @param   value
 * @return  STD_CALL static inline ownership_object_t *
 */
STD_CALL static inline ownership_object_t *get_own_value_object(IN const own_value_t value)
{
#ifdef NAN_BOX
    ownership_object_t *object;

    assert(NAN_BOX_SIGNATURE_OBJECT == (value & NAN_BOX_MASK_SIGNATURE) ||
           NAN_BOX_SIGNATURE_OBJECT_SYMBOL == (value & NAN_BOX_MASK_SIGNATURE) ||
           NAN_BOX_SIGNATURE_OBJECT_STRING == (value & NAN_BOX_MASK_SIGNATURE));

    object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);

    return object;
#else
    return value.u.ptr;
#endif
}

/**
 * get_own_value_type
 * @brief   
 * @param   value
 * @return  STD_CALL static inline own_value_type_t
 */
STD_CALL static inline own_value_type_t get_own_value_type(IN own_value_t value)
{
    std_u64_t signature;
    std_u64_t isNaN = NAN_BOX_SIGNATURE_NAN & value;

//    STD_LOG(ERR, ":%x %ld \n", value, value);
//    STD_LOG(ERR, "value 0x%04x %04x %04x %04x\n",
//            (unsigned)(value >> 48ULL) & 0xFFFF,
//            (unsigned)(value >> 32ULL) & 0xFFFF,
//            (unsigned)(value >> 16) & 0xFFFF,
//            (unsigned)value & 0xFFFF);
//
//    STD_LOG(ERR, "isNaN 0x%04x %04x %04x %04x\n",
//            (unsigned)(isNaN >> 48ULL) & 0xFFFF,
//            (unsigned)(isNaN >> 32ULL) & 0xFFFF,
//            (unsigned)(isNaN >> 16) & 0xFFFF,
//            (unsigned)isNaN & 0xFFFF);
//
//    STD_LOG(ERR, "NAN_BOX_SIGNATURE_NAN 0x%04x %04x %04x %04x\n",
//            (unsigned)(NAN_BOX_SIGNATURE_NAN >> 48ULL) & 0xFFFF,
//            (unsigned)(NAN_BOX_SIGNATURE_NAN >> 32ULL) & 0xFFFF,
//            (unsigned)(NAN_BOX_SIGNATURE_NAN >> 16) & 0xFFFF,
//            (unsigned)NAN_BOX_SIGNATURE_NAN & 0xFFFF);

    if (isNaN != NAN_BOX_SIGNATURE_NAN) {
        return OWN_TYPE_NUMBER;
    }

    signature = value & NAN_BOX_MASK_SIGNATURE;


//    STD_LOG(ERR, "signature 0x%04x %04x %04x %04x\n",
//            (unsigned)(signature >> 48ULL) & 0xFFFF,
//            (unsigned)(signature >> 32ULL) & 0xFFFF,
//            (unsigned)(signature >> 16) & 0xFFFF,
//            (unsigned)signature & 0xFFFF);

    switch (signature) {
        case NAN_BOX_SIGNATURE_NAN:
            return OWN_TYPE_DOUBLE;
        case NAN_BOX_SIGNATURE_NULL:
            return OWN_TYPE_NULL;
        case NAN_BOX_SIGNATURE_FALSE:
        case NAN_BOX_SIGNATURE_TRUE:
            return OWN_TYPE_BOOL;
        case NAN_BOX_SIGNATURE_ADDRESS:
            return OWN_TYPE_ADDRESS;
        case NAN_BOX_SIGNATURE_OBJECT:
            return OWN_TYPE_OBJECT;
        case NAN_BOX_SIGNATURE_OBJECT_SYMBOL:
            return OWN_TYPE_OBJECT_SYMBOL;
        case NAN_BOX_SIGNATURE_OBJECT_STRING:
            return OWN_TYPE_OBJECT_STRING;
        case NAN_BOX_MASK_SIGNATURE:
            //such as -2, < 0
            return OWN_TYPE_NUMBER;
        default:
            return OWN_TYPE_NULL;
    }
}

/**
 * get_own_value_type_string
 * @brief   
 * @param   value
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_own_value_type_string(IN own_value_t value);

/**
 * is_own_value_equal
 * @brief   
 * @param   x_value
 * @param   y_value
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t is_own_value_equal(IN own_value_t x_value, IN own_value_t y_value);

/**
 * print_own_value
 * @brief   
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_own_value(IN own_value_t value);
/**
 * create_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t create_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN ownership_object_t *need_ast);


/**
 * verify_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t verify_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN const ownership_object_t *need_ast);


/**
 * clear_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t clear_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN ownership_object_t *need_ast);


/**
 * duplicate_ownership_value
 * @brief   
 * @param   owner_symbol
 * @param   own_item
 * @return  STD_CALL own_value_t
 */
STD_CALL own_value_t duplicate_ownership_value(IN const ownership_object_symbol_t *owner_symbol, IN own_value_t own_item);

/**
 * free_ownership_ownvalue
 * @brief   
 * @param   owner_symbol
 * @param   own_item
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t free_ownership_ownvalue(IN const ownership_object_symbol_t *owner_symbol, IN own_value_t own_item);

/**
 * check_support_own_value_type
 * @brief   
 * @param   value
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t check_support_own_value_type(IN own_value_t value);


/**
 * build_u64key_with_object_value
 * @brief   
 * @param   obj
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t build_u64key_with_object_value(IN ownership_object_t *obj);

/**
 * build_u64key_with_value
 * @brief   
 * @param   value
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t build_u64key_with_value(IN own_value_t value);

/**
 * print_own_value_to_buf
 * @brief   
 * @param   value
 * @param   buf
 * @param   reenter
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_own_value_to_buf(IN own_value_t value, IN std_char_t *buf, std_bool_t reenter);

/**
 * print_object_value_to_buf
 * @brief   
 * @param   obj
 * @param   buf
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_object_value_to_buf(IN const ownership_object_t *obj, IN std_char_t *buf);

#endif