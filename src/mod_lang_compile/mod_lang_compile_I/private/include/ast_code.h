/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    ast_code.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-21
 *
 */
#ifndef NONCLEDB_AST_CODE_H
#define NONCLEDB_AST_CODE_H

#include "lang_ast.h"

typedef enum gen_opcode_s {
    POP = 1,

    VAR_A,
    VAR_L,
    VAR_A_CLEAN,
    VAR_L_CLEAN,
    SYM_A,
    SYM_L,

    PUSHI,
    PUSHU,
    PUSHA,
    PUSHS,

    ADD,
    Inp_ADD,
    Inp_ADDI,
    SUB,
    Inp_SUB,
    MUL,
    Inp_MUL,
    DIV,
    Inp_DIV,
    MOD,
    Inp_MOD,

    GT,
    LT,
    EQ,
    NEQ,
    LGE,
    BGE,
    BEQ0,
    AND,
    OR,

    LOADA,
    LOADL,
    STOREA,
    STOREL,

    JUMP,
    CALL,
    RET,
    POPR,
    FRAME,

    ENTRY,
    LABEL,
    CUSTOM,

    NEW_ARRAY,
    DEL_ARRAY,

    NEW_LIST,
    DEL_LIST,

    NEW_KEY_HASH,
    DEL_KEY_HASH,

    ADD_ITEM,
    ADD_KEY_ITEM,
    DEL_ITEM,
    GET_ITEM,
    SET_ITEM,
    FIND_ITEM,
    COUNT_ITEM,
    RESIZE_ARRAY,

    LOAD_LIB
} gen_opcode_t;

typedef struct gen_buffer_s {
    std_char_t *buf;
    std_int_t buf_len;
} gen_buffer_t;

typedef struct gen_code_s {
    std_int_t opcode;
    std_64_t operand;
    std_u64_t u_operand;
    const std_char_t *soperand;
    std_int_t ex_operand;
    std_int_t line;
} gen_code_t;

typedef struct gen_code_table_s {
    char *name;
    int opcode;
} gen_code_table_t;

#define MAX_GEN_CODES 4000
#define MAX_GEN_BUFFER 64

/**
 * gen_code_init
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_code_init();

/**
 * get_opcode_name
 * @brief   
 * @param   opcode
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_opcode_name(IN std_int_t opcode);

/**
 * get_opcode
 * @brief   
 * @param   name
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_opcode(IN std_char_t *name);

/**
 * gen_code
 * @brief   
 * @param   opcode
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_code(IN std_int_t opcode, IN std_int_t line);

/**
 * gen_codeI
 * @brief   
 * @param   opcode
 * @param   i
 * @param   ex
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_codeI(IN std_int_t opcode, IN std_64_t i, IN std_int_t ex, IN std_int_t line);

/**
 * gen_codeU
 * @brief   
 * @param   opcode
 * @param   i
 * @param   ex
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_codeU(IN std_int_t opcode, IN std_u64_t i, IN std_int_t ex, IN std_int_t line);
/**
 * gen_codeS
 * @brief   
 * @param   opcode
 * @param   s
 * @param   ex
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_codeS(IN std_int_t opcode, IN const std_char_t *s, IN std_int_t ex, IN std_int_t line);

/**
 * gen_code_func
 * @brief   
 * @param   entry_name
 * @param   n_local
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *gen_code_func(IN std_char_t *entry_name, IN std_int_t n_local, IN std_int_t params_count);

/**
 * gen_code_expr
 * @brief   
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *gen_code_expr();
/**
 * gen_buffer_reset
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_buffer_reset();

/**
 * gen_buffer_output
 * @brief   
 * @param   output_buffer
 * @param   output_buffer_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t gen_buffer_output(IN std_char_t *output_buffer, IN std_int_t output_buffer_len);

#endif
