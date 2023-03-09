/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    ast_code.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-21
 *
 */
#include "ast_code.h"
#include "std_common.h"

gen_code_t gen_codes[MAX_GEN_CODES];
std_int_t n_code;
static gen_buffer_t gen_buffer[MAX_GEN_BUFFER];
static std_int_t gen_buffer_idx = 0;

gen_code_table_t gen_code_table[] = {
        {"POP", POP},
        {"PUSHI", PUSHI},
        {"PUSHU", PUSHU},
        {"PUSHA", PUSHA},
        {"PUSHS", PUSHS},
        {"ADD", ADD},
        {"Inp_ADD", Inp_ADD},
        {"Inp_ADDI", Inp_ADDI},
        {"SUB", SUB},
        {"Inp_SUB", Inp_SUB},
        {"MUL", MUL},
        {"Inp_MUL", Inp_MUL},
        {"DIV", DIV},
        {"Inp_DIV", Inp_DIV},
        {"MOD", MOD},
        {"Inp_MOD", Inp_MOD},
        {"GT", GT},
        {"LT", LT},
        {"EQ", EQ},
        {"NEQ", NEQ},
        {"LGE", LGE},
        {"BGE", BGE},
        {"AND", AND},
        {"OR", OR},
        {"BEQ0", BEQ0},
        {"LOADA", LOADA},
        {"LOADL", LOADL},
        {"STOREA", STOREA},
        {"STOREL", STOREL},

        {"JUMP", JUMP},
        {"CALL", CALL},
        {"RET", RET},
        {"POPR", POPR},
        {"FRAME", FRAME},
        {"ENTRY", ENTRY},
        {"LABEL", LABEL},
        {"CUSTOM", CUSTOM},
        {"VAR_A", VAR_A},
        {"VAR_L", VAR_L},
        {"VAR_A_CLEAN", VAR_A_CLEAN},
        {"VAR_L_CLEAN", VAR_L_CLEAN},
        {"SYM_A", SYM_A},
        {"SYM_L", SYM_L},

        {"NEW_ARRAY", NEW_ARRAY},
        {"DEL_ARRAY", DEL_ARRAY},
        {"NEW_LIST", NEW_LIST},
        {"DEL_LIST", DEL_LIST},
        {"NEW_KEY_HASH", NEW_KEY_HASH},
        {"DEL_KEY_HASH", DEL_KEY_HASH},
        {"ADD_ITEM", ADD_ITEM},
        {"ADD_KEY_ITEM", ADD_KEY_ITEM},
        {"DEL_ITEM", DEL_ITEM},
        {"GET_ITEM", GET_ITEM},
        {"SET_ITEM", SET_ITEM},
        {"FIND_ITEM", FIND_ITEM},
        {"COUNT_ITEM", COUNT_ITEM},
        {"LOAD_LIB", LOAD_LIB},

        {0, 0}};


/**
 * gen_code_init
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_code_init()
{
    n_code = 0;
}

/**
 * get_opcode_name
 * @brief   
 * @param   opcode
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_opcode_name(IN std_int_t opcode)
{
    for (std_int_t i = 0; gen_code_table[i].name != 0; i++) {
        if (gen_code_table[i].opcode == opcode) {
            return gen_code_table[i].name;
        }
    }

    fprintf(stderr, "unknown code = %d\n", opcode);
    exit(1);
}

/**
 * get_opcode
 * @brief   
 * @param   name
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_opcode(IN std_char_t *name)
{
    for (std_int_t i = 0; gen_code_table[i].name != 0; i++) {
        if (strcmp(gen_code_table[i].name, name) == 0) {
            return gen_code_table[i].opcode;
        }
    }

    fprintf(stderr, "unknown code name = %s\n", name);
    exit(1);
}


/**
 * gen_code
 * @brief   
 * @param   opcode
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_code(IN std_int_t opcode, IN std_int_t line)
{
    gen_codes[n_code].line = line;
    gen_codes[n_code++].opcode = opcode;
}

/**
 * gen_codeI
 * @brief   
 * @param   opcode
 * @param   i
 * @param   ex
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_codeI(IN std_int_t opcode, IN std_64_t i, IN std_int_t ex, IN std_int_t line)
{
    gen_codes[n_code].opcode = opcode;
    gen_codes[n_code].operand = i;
    gen_codes[n_code].line = line;
    gen_codes[n_code++].ex_operand = ex;
}

/**
 * gen_codeU
 * @brief   
 * @param   opcode
 * @param   i
 * @param   ex
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_codeU(IN std_int_t opcode, IN std_u64_t i, IN std_int_t ex, IN std_int_t line)
{
    gen_codes[n_code].opcode = opcode;
    gen_codes[n_code].u_operand = i;
    gen_codes[n_code].line = line;
    gen_codes[n_code++].ex_operand = ex;
}

/**
 * gen_codeS
 * @brief   
 * @param   opcode
 * @param   s
 * @param   ex
 * @param   line
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_codeS(IN std_int_t opcode, IN const std_char_t *s, IN std_int_t ex, IN std_int_t line)
{
    gen_codes[n_code].opcode = opcode;
    gen_codes[n_code].soperand = s;
    gen_codes[n_code].line = line;
    gen_codes[n_code++].ex_operand = ex;
}

/**
 * gen_code_func
 * @brief   
 * @param   entry_name
 * @param   n_local
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *gen_code_func(IN std_char_t *entry_name, IN std_int_t n_local, IN std_int_t params_count)
{
    std_char_t *buffer;
    std_size_t buffer_index = 0;

    buffer = (std_char_t *) CALLOC(sizeof(std_char_t), BUF_SIZE_4096);

    snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "ENTRY %s %d\n", entry_name, params_count);
    buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);

    snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "FRAME %d\n", n_local);
    buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);

    for (std_int_t i = 0; i < n_code; i++) {
        snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%s ", get_opcode_name(gen_codes[i].opcode));
        buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);

        switch (gen_codes[i].opcode) {
            case VAR_A:
            case VAR_L:
            case VAR_A_CLEAN:
            case VAR_L_CLEAN:
            case SYM_A:
            case SYM_L:
            case LOADA:
            case LOADL:
            case STOREA:
            case STOREL:

            case POPR:

            case PUSHA:
                snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%ld [%d]", gen_codes[i].operand, gen_codes[i].line);
                buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
                break;

            case Inp_ADD:
            case Inp_SUB:
            case Inp_MUL:
            case Inp_DIV:
            case Inp_MOD:
            case Inp_ADDI:
            case PUSHI:
            case NEW_ARRAY:
            case DEL_ARRAY:
            case NEW_LIST:
            case DEL_LIST:
            case NEW_KEY_HASH:
            case DEL_KEY_HASH:
            case ADD_ITEM:
            case ADD_KEY_ITEM:
            case DEL_ITEM:
            case GET_ITEM:
            case FIND_ITEM:
            case COUNT_ITEM:
            case SET_ITEM:
                snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%ld %d [%d]", gen_codes[i].operand, gen_codes[i].ex_operand, gen_codes[i].line);
                buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
                break;

            case BEQ0:
            case LABEL:
            case JUMP:
                snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "L%ld [%d]", gen_codes[i].operand, gen_codes[i].line);
                buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
                break;

            case PUSHU:
                snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%lu [%d]", gen_codes[i].u_operand, gen_codes[i].line);
                buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
                break;

            case LOAD_LIB:
            case PUSHS:
                snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%s [%d]", gen_codes[i].soperand, gen_codes[i].line);
                buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
                break;

            case CALL:
                snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%s %d [%d]", gen_codes[i].soperand, gen_codes[i].ex_operand, gen_codes[i].line);
                buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
                break;

            default:
                break;
        }
        snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "\n");
        buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
    }
    snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "RET\n");

    STD_LOG(INFO, "%s %s", __FUNCTION__, buffer);

    gen_buffer[gen_buffer_idx++].buf = buffer;

    return buffer;
}


/**
 * gen_code_expr
 * @brief   
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *gen_code_expr()
{
    std_char_t *buffer;
    std_size_t buffer_index = 0;

    buffer = (std_char_t *) CALLOC(sizeof(std_char_t), BUF_SIZE_4096);

    for (std_int_t i = 0; i < n_code; i++) {
        snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%s ", get_opcode_name(gen_codes[i].opcode));
        buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);

        switch (gen_codes[i].opcode) {
            case LOAD_LIB:
                snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "%s [%d]", gen_codes[i].soperand, gen_codes[i].line);
                buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
                break;

            default:
                break;
        }
        snprintf(buffer + buffer_index, BUF_SIZE_4096 - buffer_index, "\n");
        buffer_index = std_safe_strlen(buffer, BUF_SIZE_4096);
    }

    STD_LOG(INFO, "%s %s", __FUNCTION__, buffer);

    gen_buffer[gen_buffer_idx++].buf = buffer;

    return buffer;
}

/**
 * gen_buffer_reset
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t gen_buffer_reset()
{
    memset(gen_buffer, 0, sizeof(gen_buffer));
    gen_buffer_idx = 0;
}

/**
 * gen_buffer_output
 * @brief   
 * @param   output_buffer
 * @param   output_buffer_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t gen_buffer_output(IN std_char_t *output_buffer, IN std_int_t output_buffer_len)
{
    std_size_t out_buf_len = 0;
    std_rv_t ret = STD_RV_SUC;

    memset(output_buffer, 0, output_buffer_len);
    for (int i = 0; i < gen_buffer_idx; ++i) {
        out_buf_len += std_safe_strlen(gen_buffer[i].buf, BUF_SIZE_4096);
        if (out_buf_len < output_buffer_len) {
            strcat(output_buffer, gen_buffer[i].buf);
            FREE(gen_buffer[i].buf);
        } else {
            ret = STD_RV_ERR_POINTER;
            break;
        }
    }

    STD_LOG(DEBUG, "%s: %s\n", __FUNCTION__, output_buffer);
    return ret;
}
