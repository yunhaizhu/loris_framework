/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_EXEC.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-04
 *
 */
#include "virtual_machine_EXEC.h"
#include "virtual_machine_object.h"
#include "virtual_machine_safe_var.h"


extern environment_vm_t vm[128];

/**
 * execute_code_POP
 * @brief
 * @param   x
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_POP(IN std_int_t thread_id, std_u64_t *x)
{
    *x = Pop(thread_id);
}

/**
 * execute_code_PUSHI
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_PUSHIUAS(IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc)
{
    Push(thread_id, Codes[*Pc].i_operand);
}

/**
 * inline_execute_code_ADD
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD(IN std_int_t thread_id)
{
    own_value_t obj_y = Pop(thread_id);
    own_value_t obj_x = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x) + get_own_value_number(obj_y));
    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_SUB
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SUB(IN std_int_t thread_id)
{
    own_value_t obj_y = Pop(thread_id);
    own_value_t obj_x = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x) - get_own_value_number(obj_y));
    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_MUL
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_MUL(IN std_int_t thread_id)
{
    own_value_t obj_y = Pop(thread_id);
    own_value_t obj_x = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x) * get_own_value_number(obj_y));
    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_DIV
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_DIV(IN std_int_t thread_id)
{
    own_value_t obj_y = Pop(thread_id);
    own_value_t obj_x = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x) / get_own_value_number(obj_y));
    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_MOD
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_MOD(IN std_int_t thread_id)
{
    own_value_t obj_y = Pop(thread_id);
    own_value_t obj_x = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x) % get_own_value_number(obj_y));
    Push(thread_id, (intptr_t) ret);
}

#if 0
#include "sljitLir.h"

#include <stdio.h>
#include <stdlib.h>

typedef long (SLJIT_FUNC *func2_t)(long a, long b);

/**
 * dump_code
 * @brief   
 * @param   code
 * @param   len
 */
void dump_code(void *code, sljit_uw len)
{
    FILE *fp = fopen("/tmp/slj_dump", "wb");
    if (!fp)
        return;
    fwrite(code, len, 1, fp);
    fclose(fp);
#if defined(SLJIT_CONFIG_X86_64)
    system("objdump -b binary -m l1om -D /tmp/slj_dump");
#elif defined(SLJIT_CONFIG_X86_32)
    system("objdump -b binary -m i386 -D /tmp/slj_dump");
#endif
}

/**
 * add2
 * @brief   
 * @param   a
 * @param   b
 * @return  static int
 */
static int add2(long a, long b)
{
    void *code = NAN_BOX_Null;
    int ret;
    static func2_t func;

    if (func != NAN_BOX_Null){
        ret = func(a, b);
        return ret;
    }

    /* Create a SLJIT compiler */
    struct sljit_compiler *C = sljit_create_compiler(NAN_BOX_Null, NAN_BOX_Null);

    /* Start a context(function entry), have 3 arguments, discuss later */
    sljit_emit_enter(C, 0,  SLJIT_ARG1(SW)|SLJIT_ARG2(SW),  2, 2, 0, 0, 0);

    /* The first arguments of function is register SLJIT_S0, 2nd, SLJIT_S1, etc.  */
    /* R0 = first */
    sljit_emit_op1(C, SLJIT_MOV, SLJIT_R0, 0, SLJIT_S0, 0);

    /* R0 = R0 + second */
    sljit_emit_op2(C, SLJIT_ADD, SLJIT_R0, 0, SLJIT_R0, 0, SLJIT_S1, 0);

    /* This statement mov R0 to RETURN REG and return */
    /* in fact, R0 is RETURN REG itself */
    sljit_emit_return(C, SLJIT_MOV, SLJIT_R0, 0);

    /* Generate machine code */
    code = sljit_generate_code(C);

    /* Execute code */
    func = (func2_t)code;
    ret = func(a, b);

    //    dump_code(code, len);

    /* Clean up */
    sljit_free_compiler(C);
//    sljit_free_code(code, NAN_BOX_Null);
    return ret;
}
#endif

/**
 * inline_set_obj_x_value
 * @brief   
 * @param   ret
 * @param   thread_id
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static forced_inline std_void_t
 */
STD_CALL static forced_inline std_void_t inline_set_obj_x_value(IN own_value_t ret, IN std_int_t thread_id,  IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t reg_id;
    std_int_t fp_index;
    own_value_t obj_x;
    ownership_object_symbol_t *symbol;

    reg_id = (std_int_t)Codes[*Pc].i_operand_ex;
    vm[thread_id].gpr[reg_id] = ret;
    fp_index = reg_id >= 11 ? (*Fp - (reg_id - 11)): (*Fp - (reg_id - 3));
    obj_x = Stack[fp_index];
    symbol = get_own_value_object_symbol(obj_x);

    if (symbol->env_value.symbol_type == var_type) {
        symbol->env_value.data.val.value = ret;
    }else {
        set_VAR(obj_x, NAN_BOX_Null, ret);
    }
}

/**
 * inline_execute_code_ADD_Inplace
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_Inp_ADD(IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x_value) + get_own_value_number(obj_y_value));

    inline_set_obj_x_value(ret, thread_id, Codes, Stack, Pc, Fp);
}


/**
 * inline_execute_code_Inp_ADDI
 * @brief   
 * @param   thread_id
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static forced_inline std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_Inp_ADDI(IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;
    std_64_t y_value = Codes[*Pc].i_operand;

    ret = make_own_value_number(get_own_value_number(obj_x_value) + y_value);

    inline_set_obj_x_value(ret, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_SUB
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_SUB(IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x_value) - get_own_value_number(obj_y_value));

    inline_set_obj_x_value(ret, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_MUL
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_MUL(IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x_value) * get_own_value_number(obj_y_value));

    inline_set_obj_x_value(ret, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_DIV
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_DIV(IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x_value) / get_own_value_number(obj_y_value));

    inline_set_obj_x_value(ret, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_MOD
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_MOD(IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x_value) % get_own_value_number(obj_y_value));

    inline_set_obj_x_value(ret, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * execute_code_GT
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_GT(IN std_int_t thread_id)
{
    own_value_t obj_y = Pop(thread_id);
    own_value_t obj_x = Pop(thread_id);
    own_value_t ret;
    own_value_t obj_y_value;
    own_value_t obj_x_value;

    obj_x_value = get_VAR(obj_x, NAN_BOX_Null, STD_BOOL_FALSE);
    obj_y_value = get_VAR(obj_y, NAN_BOX_Null, STD_BOOL_FALSE);
    ret = make_own_value_bool(get_own_value_number(obj_x_value) > get_own_value_number(obj_y_value));

    Push(thread_id, (intptr_t) ret);
}


/**
 * execute_code_LT
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_LT(IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) < get_own_value_number(obj_y_value));

    Push(thread_id, ret);
}


/**
 * execute_code_EQ
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_EQ(IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_bool(is_own_value_equal(obj_x_value, obj_y_value));

    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_NEQ
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEQ(IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_bool(!is_own_value_equal(obj_x_value, obj_y_value));

    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_LGE
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LGE(IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) >= get_own_value_number(obj_y_value));

    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_BGE
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_BGE(IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) <= get_own_value_number(obj_y_value));

    Push(thread_id, (intptr_t) ret);
}

/**
 * inline_execute_code_AND
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_AND(IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) && get_own_value_number(obj_y_value));

    Push(thread_id, (intptr_t) ret);
}


/**
 * inline_execute_code_OR
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_OR(IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(thread_id);
    own_value_t obj_x_value = Pop(thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) || get_own_value_number(obj_y_value));

    Push(thread_id, (intptr_t) ret);
}

/**
 * execute_code_BEQ0
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_int_t
 */
STD_CALL static forced_inline std_bool_t inline_execute_code_BEQ0(IN std_int_t thread_id, const code_st *Codes, std_int_t *Pc)
{
    const own_value_t obj_x = Pop(thread_id);

    if (get_own_value_bool(obj_x) == STD_BOOL_FALSE) {
        *Pc = (std_int_t) Codes[*Pc].i_operand;
        return STD_BOOL_TRUE;
    }

    return STD_BOOL_FALSE;
}


/**
 * execute_code_LOADA
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LOADA(IN std_int_t thread_id, IN code_st *Codes, IN const std_int_t *Pc)
{
    own_value_t obj_value;

//    if (likely(Codes[*Pc].gpr_idx != 0 && vm[thread_id].gpr[Codes[*Pc].gpr_idx] != NAN_BOX_Null)) {
//        obj_value = vm[thread_id].gpr[Codes[*Pc].gpr_idx];
//    } else

    //performance issue.
    {
        own_value_t object = Codes[*Pc].i_operand;
//        Codes[*Pc].gpr_idx = (std_int_t)(3 + Codes[*Pc].i_operand_ex);
        obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
//        vm[thread_id].gpr[Codes[*Pc].gpr_idx] = obj_value;
    }

    Push(thread_id, obj_value);
}


/**
 * execute_code_LOADL
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LOADL(IN std_int_t thread_id, IN code_st *Codes, IN const std_int_t *Pc)
{
    own_value_t obj_value;

//    if (likely(Codes[*Pc].gpr_idx != 0 && vm[thread_id].gpr[Codes[*Pc].gpr_idx] != NAN_BOX_Null)) {
//        obj_value = vm[thread_id].gpr[Codes[*Pc].gpr_idx];
//    } else
    {
        own_value_t object = Codes[*Pc].i_operand;
//        Codes[*Pc].gpr_idx = (std_int_t)(11 + Codes[*Pc].i_operand_ex);
        obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
//        vm[thread_id].gpr[Codes[*Pc].gpr_idx] = obj_value;
    }

    Push(thread_id, obj_value);
}


/**
 * execute_code_STOREA
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_STOREA(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp + Codes[*Pc].i_operand + 3];
    set_VAR(object, NAN_BOX_Null, Top(thread_id));
    vm[thread_id].gpr[3 + Codes[*Pc].i_operand] = Top(thread_id);
}

/**
 * execute_code_STOREL
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_STOREL(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    set_VAR(object, NAN_BOX_Null, Top(thread_id));
    vm[thread_id].gpr[11 + Codes[*Pc].i_operand] = Top(thread_id);
}

/**
 * execute_code_JUMP
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_JUMP(IN std_int_t thread_id, const code_st *Codes, std_int_t *Pc)
{
    *Pc = (std_int_t) Codes[*Pc].i_operand;
}

/**
 * execute_code_CALL
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_CALL(IN std_int_t thread_id, const code_st *Codes, std_int_t *Pc)
{
    Push(thread_id, *Pc + 1);
    *Pc = (std_int_t) Codes[*Pc].i_operand;
}

/**
 * execute_code_RET
 * @brief
 * @param   Pc
 * @param   Fp
 * @param   Sp
 * @param   x
 * @return  STD_CALL static std_int_t
 */
STD_CALL static inline std_int_t inline_execute_code_RET(IN std_int_t thread_id, std_int_t *Pc, std_int_t *Fp, std_int_t *Sp, std_u64_t *x)
{
    //    *x = Pop(thread_id);
    *Sp = *Fp;
    *Fp = (std_int_t) Pop(thread_id);
    if (*Sp >= MAX_STACK - 1 || *Sp < 0) {
#if DUMP_EXEC_CODE
        const code_st *Codes = vm[thread_id].Codes;
        STD_LOG(DISPLAY, "%d: %s 0x%lx ex:0x%lx [%d] Sp:%d Fp:%d Pc:%d\n", *Pc, get_opcode_name(Codes[*Pc].opcode),
                Codes[*Pc].i_operand, Codes[*Pc].i_operand_ex, Codes[*Pc].line, *Sp, *Fp, *Pc);
#endif
        return 1;
    }
    *Pc = (std_int_t) Pop(thread_id);
    return 0;
}

/**
 * execute_code_POPR
 * @brief
 * @param   Codes
 * @param   Pc
 * @param   Sp
 * @param   x
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_POPR(IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc, std_int_t *Sp, const std_u64_t *x)
{
    *Sp = (std_int_t) (*Sp + Codes[*Pc].i_operand);
    //    Push(thread_id, *x);
}

/**
 * execute_code_FRAME
 * @brief
 * @param   Codes
 * @param   Pc
 * @param   Fp
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_FRAME(IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc, std_int_t *Fp, std_int_t *Sp)
{
    Push(thread_id, *Fp);
    *Fp = *Sp;
    *Sp -= (std_int_t) Codes[*Pc].i_operand;
}

/**
 * execute_code_CUSTOM
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_CUSTOM(IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc)
{
    std_char_t key[BUF_SIZE_32] = "\0";
    const func_entry_t *func_entry;

    snprintf(key, sizeof(key), "%ld", Codes[*Pc].i_operand);
    func_entry = (func_entry_t *) std_lock_free_key_hash_find(vm[get_std_thread_id()].custom_func_hash, key, std_safe_strlen(key, sizeof(key)));

    STD_ASSERT_RV(func_entry != NULL, );
    func_entry->reg_func(thread_id, func_entry->arg_counts);
}

/**
 * execute_code_VAR_A
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_A(IN std_int_t thread_id, const code_st *Codes, std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Codes[*Pc].i_operand;
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);
    std_int_t fp_index;

    fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + 3);
    own_value_t init_value = Stack[fp_index];
    declare_VAR(symbol, var_type, 0, init_value, STD_BOOL_FALSE);

    Stack[fp_index] = object;
}

/**
 * execute_code_VAR_L
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_L(IN std_int_t thread_id, const code_st *Codes, std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Codes[*Pc].i_operand;
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);
    std_int_t fp_index;

    fp_index = (std_int_t) (*Fp - Codes[*Pc].i_operand_ex);
    Stack[fp_index] = object;
    declare_VAR(symbol, var_type, 0, NAN_BOX_Null, STD_BOOL_TRUE);
}

/**
 * execute_code_VAR_A_CLEAN
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_A_CLEAN(IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc)
{
    own_value_t object = Codes[*Pc].i_operand;

    del_VARS(object, STD_BOOL_TRUE);
#if 1
    vm[thread_id].gpr[3 + Codes[*Pc].i_operand_ex] = NAN_BOX_Null;
#endif
}

/**
 * execute_code_VAR_L_CLEAN
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_L_CLEAN(IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc)
{
    own_value_t object = Codes[*Pc].i_operand;

    del_VARS(object, STD_BOOL_TRUE);
#if 1
    vm[thread_id].gpr[11 + Codes[*Pc].i_operand_ex] = NAN_BOX_Null;
#endif
}

/**
 * execute_code_SYM_A
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SYM_A(IN std_int_t thread_id, code_st *Codes, const std_int_t *Pc)
{
    own_value_t obj = Codes[*Pc].i_operand;

    Push(thread_id, obj);
}


/**
 * execute_code_SYM_L
 * @brief
 * @param   Codes
 * @param   Pc
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SYM_L(IN std_int_t thread_id, IN code_st *Codes, IN const std_int_t *Pc, IN const std_int_t *Sp)
{
    own_value_t obj = Codes[*Pc].i_operand;

    Push(thread_id, obj);
}

/**
 * execute_code_NEW_ARRAY
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEW_ARRAY(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);
    std_int_t count = (std_int_t) Codes[*Pc].i_operand_ex;

    declare_VAR(symbol, array_type, count, NAN_BOX_Null, STD_BOOL_TRUE);
}

/**
 * execute_code_NEW_LIST
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEW_LIST(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);

    declare_VAR(symbol, tuple_type, 0, NAN_BOX_Null, STD_BOOL_TRUE);
}

/**
 * execute_code_NEW_KEY_HASH
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEW_KEY_HASH(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);

    declare_VAR(symbol, hash_type, 0, NAN_BOX_Null, STD_BOOL_TRUE);
}

/**
 * execute_code_SET_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SET_ITEM(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t obj_value = Pop(thread_id);
    own_value_t obj_index = Pop(thread_id);

    if (Codes[*Pc].i_operand_ex) {
        object = Stack[*Fp + Codes[*Pc].i_operand + 3];//LOAD A
    } else {
        object = Stack[*Fp - Codes[*Pc].i_operand];
    }

    set_VAR(object, obj_index, obj_value);
}

/**
 * execute_code_GET_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_GET_ITEM(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t value;
    own_value_t obj_index = Pop(thread_id);

    if (Codes[*Pc].i_operand_ex) {
        object = Stack[*Fp + Codes[*Pc].i_operand + 3];//LOAD A
    } else {
        object = Stack[*Fp - Codes[*Pc].i_operand];
    }

    value = get_VAR(object, obj_index, STD_BOOL_FALSE);

    Push(thread_id, (intptr_t) value);
}

/**
 * execute_code_ADD_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD_ITEM(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t object_item;

    if (Codes[*Pc].i_operand_ex) {
        object = Stack[*Fp + Codes[*Pc].i_operand + 3];//LOAD A
    } else {
        object = Stack[*Fp - Codes[*Pc].i_operand];
    }

    if (get_own_value_object_symbol(object)->env_value.symbol_type == var_type) {
        object_item = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
        STD_ASSERT_RV(object_item != NAN_BOX_Null, );
    } else {
        object_item = object;
    }
    STD_ASSERT_RV(get_own_value_object_symbol(object_item)->env_value.symbol_type == tuple_type, );

    own_value_t obj_value = Pop(thread_id);
    set_VAR(object_item, NAN_BOX_Null, obj_value);
}

/**
 * execute_code_ADD_KEY_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD_KEY_ITEM(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t object_item;

    if (Codes[*Pc].i_operand_ex) {
        object = Stack[*Fp + Codes[*Pc].i_operand + 3];//LOAD A
    } else {
        object = Stack[*Fp - Codes[*Pc].i_operand];
    }

    if (get_own_value_object_symbol(object)->env_value.symbol_type == var_type) {
        object_item = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
        STD_ASSERT_RV(object_item != NAN_BOX_Null, );
    } else {
        object_item = object;
    }

    STD_ASSERT_RV(get_own_value_object_symbol(object_item)->env_value.symbol_type == hash_type, );

    own_value_t obj_value = Pop(thread_id);
    own_value_t obj_key = Pop(thread_id);
    set_VAR(object_item, obj_key, obj_value);
}

/**
 * execute_code_DEL_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_DEL_ITEM(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t object_item;
    own_value_t ret;

    if (Codes[*Pc].i_operand_ex) {
        object = Stack[*Fp + Codes[*Pc].i_operand + 3];//LOAD A
    } else {
        object = Stack[*Fp - Codes[*Pc].i_operand];
    }

    if (get_own_value_object_symbol(object)->env_value.symbol_type == var_type) {
        object_item = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
        STD_ASSERT_RV(object_item != NAN_BOX_Null, );
    } else {
        object_item = object;
    }

    if (get_own_value_object_symbol(object_item)->env_value.symbol_type == tuple_type) {
        own_value_t obj_value = Pop(thread_id);
        ret = del_VAR(object_item, obj_value);

        Push(thread_id, ret);
    } else if (get_own_value_object_symbol(object_item)->env_value.symbol_type == hash_type) {
        own_value_t obj_key = Pop(thread_id);
        ret = del_VAR(object_item, obj_key);
        Push(thread_id, ret);
    }
}

/**
 * execute_code_FIND_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_FIND_ITEM(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t object_item;

    if (Codes[*Pc].i_operand_ex) {
        object = Stack[*Fp + Codes[*Pc].i_operand + 3];//LOAD A
    } else {
        object = Stack[*Fp - Codes[*Pc].i_operand];
    }

    if (get_own_value_object_symbol(object)->env_value.symbol_type == var_type) {
        object_item = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
        STD_ASSERT_RV(object_item != NAN_BOX_Null, );
    } else {
        object_item = object;
    }

    if (get_own_value_object_symbol(object_item)->env_value.symbol_type == hash_type) {
        own_value_t obj_key = Pop(thread_id);
        own_value_t ret = get_VAR(object_item, obj_key, STD_BOOL_FALSE);

        Push(thread_id, ret);
    }
}

/**
 * execute_code_COUNT_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_COUNT_ITEM(IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t object_item;
    own_value_t ret = NAN_BOX_Null;

    if (Codes[*Pc].i_operand_ex) {
        object = Stack[*Fp + Codes[*Pc].i_operand + 3];//LOAD A
    } else {
        object = Stack[*Fp - Codes[*Pc].i_operand];
    }

    if (get_own_value_object_symbol(object)->env_value.symbol_type == var_type) {
        own_value_type_t value_type;

        object_item = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
        STD_ASSERT_RV(object_item != NAN_BOX_Null, );

        value_type = get_own_value_type(object_item);
        if (value_type == OWN_TYPE_OBJECT_STRING) {
            ret = make_own_value_number(get_VAR_total_with_var_type(get_own_value_object_symbol(object)));
            Push(thread_id, ret);
            return;
        }
    } else {
        object_item = object;
    }

    std_int_t obj_item_type = get_own_value_object_symbol(object_item)->env_value.symbol_type;
    if ( obj_item_type == array_type) {
        ret = make_own_value_number(get_VAR_total_with_array_type(get_own_value_object_symbol(object_item)));
    }else if (obj_item_type == tuple_type){
        ret = make_own_value_number(get_VAR_total_with_tuple_type(get_own_value_object_symbol(object_item)));
    }

    Push(thread_id, ret);
}
