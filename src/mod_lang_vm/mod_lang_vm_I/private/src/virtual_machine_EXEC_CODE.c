/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virutal_machine_EXEC_CODE.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-01
 *
 */
#include "virtual_machine_EXEC.c"
#include "virtual_machine_EXEC.h"


#define DISPATCH() \
    (*Pc)++;       \
    goto *jump_table[Codes[*Pc].opcode];


/**
* execute_code
* @brief
* @param   start_pc
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t execute_code(IN std_int_t start_pc, IN std_bool_t reset)
{
    std_int_t thread_id = get_std_thread_id();
    code_st *Codes = vm[thread_id].Codes;
    std_int_t *Sp = &vm[thread_id].Sp;
    std_int_t *Fp = &vm[thread_id].Fp;
    std_int_t *Pc = &vm[thread_id].Pc;
    std_u64_t *Stack = vm[thread_id].Stack;
    std_u64_t x = 0;

    if (reset) {
        *Sp = *Fp = MAX_STACK - 1;
    }

    *Pc = start_pc;

    void *jump_table[] = {
            NULL,
            &&OP_CODE_POP,
            &&OP_CODE_VAR_A,
            &&OP_CODE_VAR_L,
            &&OP_CODE_VAR_A_CLEAN,
            &&OP_CODE_VAR_L_CLEAN,
            &&OP_CODE_SYM_A,
            &&OP_CODE_SYM_L,
            &&OP_CODE_PUSHI,
            &&OP_CODE_PUSHU,
            &&OP_CODE_PUSHA,
            &&OP_CODE_PUSHS,
            &&OP_CODE_ADD,
            &&OP_CODE_Inp_ADD,
            &&OP_CODE_Inp_ADDI,
            &&OP_CODE_SUB,
            &&OP_CODE_Inp_SUB,
            &&OP_CODE_MUL,
            &&OP_CODE_Inp_MUL,
            &&OP_CODE_DIV,
            &&OP_CODE_Inp_DIV,
            &&OP_CODE_MOD,
            &&OP_CODE_Inp_MOD,
            &&OP_CODE_GT,
            &&OP_CODE_LT,
            &&OP_CODE_EQ,
            &&OP_CODE_NEQ,
            &&OP_CODE_LGE,
            &&OP_CODE_BGE,
            &&OP_CODE_BEQ0,
            &&OP_CODE_AND,
            &&OP_CODE_OR,
            &&OP_CODE_LOADA,
            &&OP_CODE_LOADL,
            &&OP_CODE_STOREA,
            &&OP_CODE_STOREL,
            &&OP_CODE_STOREL_C,
            &&OP_CODE_STOREL_NC,
            &&OP_CODE_JUMP,
            &&OP_CODE_CALL,
            &&OP_CODE_RET,
            &&OP_CODE_POPR,
            &&OP_CODE_FRAME,

            &&OP_CODE_ENTRY,
            &&OP_CODE_LABEL,
            &&OP_CODE_CUSTOM,

            &&OP_CODE_NEW_ARRAY,
            &&OP_CODE_DEL_ARRAY,

            &&OP_CODE_NEW_LIST,
            &&OP_CODE_DEL_LIST,

            &&OP_CODE_NEW_KEY_HASH,
            &&OP_CODE_DEL_KEY_HASH,

            &&OP_CODE_ADD_ITEM,
            &&OP_CODE_ADD_KEY_ITEM,
            &&OP_CODE_DEL_ITEM,
            &&OP_CODE_GET_ITEM,
            &&OP_CODE_SET_ITEM,
            &&OP_CODE_FIND_ITEM,
            &&OP_CODE_COUNT_ITEM,

            &&OP_CODE_LOAD_LIB};
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_POP:
    inline_execute_code_POP(thread_id, &x);
    DISPATCH()

OP_CODE_VAR_A:
    inline_execute_code_VAR_A(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_VAR_L:
    inline_execute_code_VAR_L(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_VAR_A_CLEAN:
    inline_execute_code_VAR_A_CLEAN(thread_id, Codes, Pc);
    DISPATCH()

OP_CODE_VAR_L_CLEAN:
    inline_execute_code_VAR_L_CLEAN(thread_id, Codes, Pc);
    DISPATCH()

OP_CODE_SYM_A:
    inline_execute_code_SYM_A(thread_id, Codes, Pc);
    DISPATCH()

OP_CODE_SYM_L:
    inline_execute_code_SYM_L(thread_id, Codes, Pc, Sp);
    DISPATCH()

OP_CODE_PUSHI:
OP_CODE_PUSHU:
OP_CODE_PUSHA:
OP_CODE_PUSHS:
    inline_execute_code_PUSHIUAS(thread_id, Codes, Pc);
    DISPATCH()

OP_CODE_ADD:
    inline_execute_code_ADD(thread_id);
    DISPATCH()

OP_CODE_Inp_ADD:
    inline_execute_code_Inp_ADD(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_Inp_ADDI:
    inline_execute_code_Inp_ADDI(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_SUB:
    inline_execute_code_SUB(thread_id);
    DISPATCH()

OP_CODE_Inp_SUB:
    inline_execute_code_Inp_SUB(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_MUL:
    inline_execute_code_MUL(thread_id);
    DISPATCH()

OP_CODE_Inp_MUL:
    inline_execute_code_Inp_MUL(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DIV:
    inline_execute_code_DIV(thread_id);
    DISPATCH()

OP_CODE_Inp_DIV:
    inline_execute_code_Inp_DIV(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_MOD:
    inline_execute_code_MOD(thread_id);
    DISPATCH()

OP_CODE_Inp_MOD:
    inline_execute_code_Inp_MOD(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_GT:
    inline_execute_code_GT(thread_id);
    DISPATCH()

OP_CODE_LT:
    inline_execute_code_LT(thread_id);
    DISPATCH()

OP_CODE_EQ:
    inline_execute_code_EQ(thread_id);
    DISPATCH()

OP_CODE_NEQ:
    inline_execute_code_NEQ(thread_id);
    DISPATCH()

OP_CODE_LGE:
    inline_execute_code_LGE(thread_id);
    DISPATCH()

OP_CODE_BGE:
    inline_execute_code_BGE(thread_id);
    DISPATCH()

OP_CODE_BEQ0:
    if (inline_execute_code_BEQ0(thread_id, Codes, Pc) == STD_BOOL_TRUE) {
        goto *jump_table[Codes[*Pc].opcode];
    } else {
        DISPATCH()
    }

OP_CODE_AND:
    inline_execute_code_AND(thread_id);
    DISPATCH()

OP_CODE_OR:
    inline_execute_code_OR(thread_id);
    DISPATCH()

OP_CODE_LOADA:
    inline_execute_code_LOADA(thread_id, Codes, Pc);
    DISPATCH()

OP_CODE_LOADL:
    inline_execute_code_LOADL(thread_id, Codes, Pc);
    DISPATCH()

OP_CODE_STOREA:
    inline_execute_code_STOREA(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_STOREL:
    inline_execute_code_STOREL(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_STOREL_C:
    inline_execute_code_STOREL_C(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_STOREL_NC:
    inline_execute_code_STOREL_NC(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_JUMP:
    inline_execute_code_JUMP(thread_id, Codes, Pc);
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_CALL:
    inline_execute_code_CALL(thread_id, Codes, Pc);
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_RET:
    if (inline_execute_code_RET(thread_id, Pc, Fp, Sp, &x) == 1) {
        return;
    }
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_POPR:
    inline_execute_code_POPR(thread_id, Codes, Pc, Sp, &x);
    DISPATCH()

OP_CODE_FRAME:
    inline_execute_code_FRAME(thread_id, Codes, Pc, Fp, Sp);
    DISPATCH()

OP_CODE_ENTRY:
    return;

OP_CODE_LABEL:
    return;

OP_CODE_CUSTOM:
    inline_execute_code_CUSTOM(thread_id, Codes, Pc);
    DISPATCH()

OP_CODE_NEW_ARRAY:
    inline_execute_code_NEW_ARRAY(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DEL_ARRAY:
    return;

OP_CODE_NEW_LIST:
    inline_execute_code_NEW_LIST(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DEL_LIST:
    return;

OP_CODE_NEW_KEY_HASH:
    inline_execute_code_NEW_KEY_HASH(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DEL_KEY_HASH:
    return;

OP_CODE_ADD_ITEM:
    inline_execute_code_ADD_ITEM(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_ADD_KEY_ITEM:
    inline_execute_code_ADD_KEY_ITEM(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DEL_ITEM:
    inline_execute_code_DEL_ITEM(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_GET_ITEM:
    inline_execute_code_GET_ITEM(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_SET_ITEM:
    inline_execute_code_SET_ITEM(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_FIND_ITEM:
    inline_execute_code_FIND_ITEM(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_COUNT_ITEM:
    inline_execute_code_COUNT_ITEM(thread_id, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_LOAD_LIB:
    return;
    DISPATCH()
}
