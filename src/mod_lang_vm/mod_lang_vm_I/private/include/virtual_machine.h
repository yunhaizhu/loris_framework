/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#ifndef NONCLEDB_VIRTUAL_MACHINE_H
#define NONCLEDB_VIRTUAL_MACHINE_H

#include "std_common.h"
#include "virtual_machine_object.h"
#include "virtual_machine_safe_var.h"

#define EXEC_COMPILED_CODE 0
#define DUMP_EXEC_CODE 0

#define MAX_CODE 2000 * 3
#define MAX_LABEL 200 * 2
#define MAX_STACK 4000 * 1

typedef enum vm_opcode_s {
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
    STOREL_C,
    STOREL_NC,
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

    LOAD_LIB
} vm_opcode_t;

typedef struct code {
    std_int_t opcode;
    std_64_t i_operand;
    std_64_t i_operand_ex;
    std_char_t *s_operand;
    std_int_t gpr_idx;
    std_int_t line;
} code_st;

typedef struct label {
    std_char_t *name;
    std_int_t addr;
    std_int_t args_count;
} label_t;

typedef struct environment_vm_s {
    std_int_t Sp;
    std_int_t Fp;
    std_int_t Pc;
    std_u64_t Stack[MAX_STACK];

    code_st Codes[MAX_CODE];
    label_t Labels[MAX_LABEL];

    std_int_t n_codes;
    std_int_t n_labels;

    std_lock_free_key_hash_t *custom_func_hash;
    std_lock_free_key_hash_t *symbol_hash;
    std_lock_free_key_hash_t *auto_free_hash;
    ownership_object_symbol_t global_system_object_symbol;

    std_u64_t gpr[32];
} environment_vm_t;

#define Push(t, x) vm[t].Stack[vm[t].Sp--] = (x)
#define Pop(t) vm[t].Stack[++vm[t].Sp]
#define Top(t) vm[t].Stack[vm[t].Sp + 1]

/**
 * vm_init
 * @brief   
 * @param   buffer
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_init(IN const std_char_t *name, IN const std_char_t *buffer);
/**
 * vm_execute
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_execute(IN const std_char_t *name, IN std_u64_t u64_key);
/**
 * vm_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_cleanup(IN const std_char_t *name);

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
#endif
