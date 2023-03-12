/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_READ.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-04
 *
 */
#include "virtual_machine_READ.h"
#include <dlfcn.h>

extern environment_vm_t vm[128];

/**
 * read_code_PUSHS
 * @brief
 * @param   Codes
 * @param   n_codes
 * @param   line
 * @param   buf
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_PUSHS(IN code_st *Codes, IN const std_int_t *n_codes, IN const std_char_t *line)
{
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_char_t string[BUF_SIZE_128] = "\0";
    std_int_t source_line = 0;
    own_value_t value;

    STD_ASSERT_RV(sscanf(line, "%s [%[^]]] [%d]", opcode_name, string, &source_line) == 3, STD_RV_ERR_FAIL);

    std_safe_strip_chars(string, '"');
    value = make_own_value_object_string(string);
    Codes[*n_codes].i_operand = value;
    Codes[*n_codes].line = source_line;

    create_ownership_signature(&vm[get_std_thread_id()].global_system_object_symbol, get_own_value_object(value));
    return STD_RV_SUC;
}


/**
 * read_code_PUSHI
 * @brief   
 * @param   Codes
 * @param   n_codes
 * @param   opcode
 * @param   line
 * @param   entry
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_PUSHI(IN code_st *Codes, IN const std_int_t *n_codes, IN const std_char_t *line, IN const std_char_t *entry)
{
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_64_t i = 0;
    std_64_t ex = 0;
    std_int_t source_line = 0;
    own_value_t value;

    if (sscanf(line, "%s %ld %ld [%d]", opcode_name, &i, &ex, &source_line) != 4) {
        STD_LOG(ERR, "code read error\n");
        return STD_RV_ERR_POINTER;
    }

    if (ex) {
        value = make_own_value_bool(i);
    } else {
        value = make_own_value_number(i);
    }

    Codes[*n_codes].i_operand = value;
    Codes[*n_codes].line = source_line;

    return STD_RV_SUC;
}

/**
 * read_code_PUSHU
 * @brief   
 * @param   Codes
 * @param   n_codes
 * @param   line
 * @param   entry
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_PUSHU(IN code_st *Codes, IN const std_int_t *n_codes, IN const std_char_t *line, IN const std_char_t *entry)
{
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_u64_t i = 0;
    std_int_t source_line = 0;
    own_value_t value;

    if (sscanf(line, "%s %lu [%d]", opcode_name, &i, &source_line) != 3) {
        STD_LOG(ERR, "code read error\n");
        return STD_RV_ERR_POINTER;
    }

    value = make_own_value_number(i);
    Codes[*n_codes].i_operand = value;
    Codes[*n_codes].line = source_line;

    return STD_RV_SUC;
}

/**
 * read_code_PUSHA
 * @brief   
 * @param   Codes
 * @param   n_codes
 * @param   line
 * @param   entry
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_PUSHA(IN code_st *Codes, IN const std_int_t *n_codes, IN const std_char_t *line, IN const std_char_t *entry)
{
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_64_t i = 0;
    std_int_t source_line = 0;
    own_value_t value;

    if (sscanf(line, "%s %lu [%d]", opcode_name, &i, &source_line) != 3) {
        STD_LOG(ERR, "code read error\n");
        return STD_RV_ERR_POINTER;
    }

    value = make_own_value_address((std_void_t *) i);
    Codes[*n_codes].i_operand = value;
    Codes[*n_codes].line = source_line;

    return STD_RV_SUC;
}

/**
 * read_code_VAR_CUSTOM
 * @brief
 * @param   Codes
 * @param   n_codes
 * @param   opcode
 * @param   line
 * @param   entry
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_VAR_SYM(code_st *Codes, const std_int_t *n_codes, std_int_t opcode, const std_char_t *line, std_char_t *entry)
{
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_char_t key[BUF_SIZE_128] = "\0";
    std_64_t i = 0;
    own_value_t value;

    if (sscanf(line, "%s %lu", opcode_name, &i) != 2) {
        STD_LOG(ERR, "code read error\n");
        return STD_RV_ERR_POINTER;
    }
    Codes[*n_codes].i_operand = i;

    if (opcode == VAR_A) {
        value = make_own_value_object_symbol();

        Codes[*n_codes].i_operand = value;
        Codes[*n_codes].i_operand_ex = i;

        snprintf(key, sizeof(key), "arg_%s_%ld", entry, i);
        std_lock_free_key_hash_add(vm[get_std_thread_id()].symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) value);
    } else if (opcode == VAR_L) {
        value = make_own_value_object_symbol();

        Codes[*n_codes].i_operand = value;
        Codes[*n_codes].i_operand_ex = i;

        snprintf(key, sizeof(key), "local_%s_%ld", entry, i);
        std_lock_free_key_hash_add(vm[get_std_thread_id()].symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) value);
    } else if (opcode == VAR_A_CLEAN || opcode == SYM_A || opcode == LOADA) {
        snprintf(key, sizeof(key), "arg_%s_%ld", entry, i);
        value = (own_value_t) std_lock_free_key_hash_find(vm[get_std_thread_id()].symbol_hash, key, std_safe_strlen(key, sizeof(key)));
        Codes[*n_codes].i_operand = (intptr_t) value;
        Codes[*n_codes].i_operand_ex = i;
    } else if (opcode == VAR_L_CLEAN || opcode == SYM_L|| opcode == LOADL) {
        snprintf(key, sizeof(key), "local_%s_%ld", entry, i);
        value = (own_value_t) std_lock_free_key_hash_find(vm[get_std_thread_id()].symbol_hash, key, std_safe_strlen(key, sizeof(key)));
        Codes[*n_codes].i_operand = (intptr_t) value;
        Codes[*n_codes].i_operand_ex = i;
    }

    return STD_RV_SUC;
}

/**
 * read_code_EX
 * @brief
 * @param   Codes
 * @param   n_codes
 * @param   line
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_EX(IN code_st *Codes, IN const std_int_t *n_codes, IN const std_char_t *line)
{
    std_int_t i = 0;
    std_int_t ex = 0;
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_int_t source_line = 0;

    if (sscanf(line, "%s %d %d [%d]", opcode_name, &i, &ex, &source_line) != 4) {
        STD_LOG(ERR, "code read error\n");
        return STD_RV_ERR_POINTER;
    }

    Codes[*n_codes].i_operand = i;
    Codes[*n_codes].i_operand_ex = ex;
    Codes[*n_codes].line = source_line;

    return STD_RV_SUC;
}

/**
 * read_code_CALL
 * @brief
 * @param   Codes
 * @param   n_codes
 * @param   line
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_CALL(IN code_st *Codes, IN const std_int_t *n_codes, IN const std_char_t *line)
{
    std_char_t buf[BUF_SIZE_128];
    std_int_t idx = 0;
    std_int_t source_line = 0;
    std_char_t alter_name[BUF_SIZE_256] = "\0";
    std_char_t opcode_name[BUF_SIZE_32] = "\0";

    if (sscanf(line, "%s %s %d [%d]", opcode_name, buf, &idx, &source_line) != 4) {
        STD_LOG(ERR, "code read error\n");
        return STD_RV_ERR_POINTER;
    }

    if ((!strcmp(buf, "print") && std_safe_strlen(buf, BUF_SIZE_32) == std_safe_strlen("print", BUF_SIZE_32)) ||
        (!strcmp(buf, "assert") && std_safe_strlen(buf, BUF_SIZE_32) == std_safe_strlen("assert", BUF_SIZE_32)) ||
        (!strcmp(buf, "start") && std_safe_strlen(buf, BUF_SIZE_32) == std_safe_strlen("start", BUF_SIZE_32))) {
        snprintf(alter_name, sizeof(alter_name), "%s_%d", buf, idx);
    } else if (strstr(buf, "--")) {
        snprintf(alter_name, sizeof(alter_name), "%s", strstr(buf, "--") + 2);
    } else {
        snprintf(alter_name, sizeof(alter_name), "%s", buf);
    }

    Codes[*n_codes].s_operand = strdup(alter_name);
    Codes[*n_codes].i_operand_ex = idx;
    Codes[*n_codes].line = source_line;

    return STD_RV_SUC;
}

/**
 * read_code_ENTRY
 * @brief
 * @param   Codes
 * @param   n_codes
 * @param   Labels
 * @param   n_labels
 * @param   opcode
 * @param   line
 * @param   entry
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_ENTRY(IN code_st *Codes, IN const std_int_t *n_codes, IN label_t *Labels, IN const std_int_t *n_labels, IN std_int_t opcode, IN const std_char_t *line, IN std_char_t **entry)
{
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_char_t entry_name[BUF_SIZE_128] = "\0";
    std_int_t args_count = 0;

    if (sscanf(line, "%s %s", opcode_name, entry_name) != 2) {
        STD_LOG(ERR, "code read error\n");
        return STD_RV_ERR_POINTER;
    }

    if (opcode == LABEL || opcode == ENTRY) {
        Labels[*n_labels].name = strdup(entry_name);
        Labels[*n_labels].addr = *n_codes;
        if (opcode == ENTRY) {
            sscanf(line, "%s %s %d", opcode_name, entry_name, &args_count);
            Labels[*n_labels].args_count = args_count;
            *entry = Labels[*n_labels].name;
        }
    } else {
        Codes[*n_codes].s_operand = strdup(entry_name);
    }

    return STD_RV_SUC;
}

/**
 * read_code_LOAD_LIB
 * @brief   
 * @param   Codes
 * @param   n_codes
 * @param   line
 * @return  STD_CALL static std_rv_t
 */
STD_CALL static std_rv_t read_code_LOAD_LIB(IN const code_st *Codes, IN const std_int_t *n_codes, IN const std_char_t *line)
{
    std_char_t opcode_name[BUF_SIZE_32] = "\0";
    std_char_t string[BUF_SIZE_64] = "\0";
    std_int_t source_line = 0;

    STD_ASSERT_RV(sscanf(line, "%s \"%[^\"] \" [%d]", opcode_name, string, &source_line) == 3, STD_RV_ERR_FAIL);

    typedef std_void_t (*library_init_t)();

    std_void_t *dl_handle;
    library_init_t library_init;
    std_char_t dl_name[BUF_SIZE_128] = "\0";

    snprintf(dl_name, sizeof(dl_name), "script/lib%s.so", string);

    dl_handle = dlopen(dl_name, RTLD_LAZY | RTLD_LOCAL);
    if (dl_handle != NULL) {
        library_init = dlsym(dl_handle, "library_init");

        if (library_init) {
            library_init();
        }
        STD_LOG(DISPLAY, "%s library_init SUCCESS\n", dl_name);
    }
    return STD_RV_SUC;
}

/**
 * read_code
 * @brief
 * @param   buffer
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t read_code(IN const std_char_t *buffer)
{
    std_char_t opcode_name[BUF_SIZE_32];
    std_int_t opcode;
    const std_char_t *eol = buffer;
    const std_char_t *eol2;
    std_char_t line[BUF_SIZE_128];

    code_st *Codes = vm[get_std_thread_id()].Codes;
    label_t *Labels = vm[get_std_thread_id()].Labels;
    std_int_t *n_codes = &vm[get_std_thread_id()].n_codes;
    std_int_t *n_labels = &vm[get_std_thread_id()].n_labels;
    std_char_t *entry = NULL;

    do {
        eol2 = strchr(eol + 1, '\n');
        if (eol2 == NULL) {
            break;
        }
        memset(line, 0, sizeof(line));

        STD_ASSERT_RV(eol2 - eol <= BUF_SIZE_128, STD_RV_ERR_UNEXPECTED);
        strncpy(line, eol, eol2 - eol);
        eol = eol2 + 1;

        if (sscanf(line, "%s", opcode_name) != 1) {
            STD_LOG(ERR, "code read error\n");
            return STD_RV_ERR_POINTER;
        }

        opcode = get_opcode(opcode_name);
        Codes[*n_codes].opcode = opcode;
        switch (opcode) {
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

            case FRAME:
            case POPR:
            case CUSTOM:
                STD_ASSERT_RV(read_code_VAR_SYM(Codes, n_codes, opcode, line, entry) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case Inp_ADD:
            case Inp_SUB:
            case Inp_MUL:
            case Inp_DIV:
            case Inp_MOD:
            case Inp_ADDI:
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
            case SET_ITEM:
            case FIND_ITEM:
            case COUNT_ITEM:
                STD_ASSERT_RV(read_code_EX(Codes, n_codes, line) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case CALL:
                STD_ASSERT_RV(read_code_CALL(Codes, n_codes, line) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case BEQ0:
            case LABEL:
            case JUMP:
            case ENTRY:
                STD_ASSERT_RV(read_code_ENTRY(Codes, n_codes, Labels, n_labels, opcode, line, &entry) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case PUSHS:
                STD_ASSERT_RV(read_code_PUSHS(Codes, n_codes, line) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case PUSHI:
                STD_ASSERT_RV(read_code_PUSHI(Codes, n_codes, line, entry) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case PUSHU:
                STD_ASSERT_RV(read_code_PUSHU(Codes, n_codes, line, entry) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case PUSHA:
                STD_ASSERT_RV(read_code_PUSHA(Codes, n_codes, line, entry) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            case LOAD_LIB:
                STD_ASSERT_RV(read_code_LOAD_LIB(Codes, n_codes, line) == STD_RV_SUC, STD_RV_ERR_POINTER);
                break;

            default:
                break;
        }

        if (opcode == LABEL || opcode == ENTRY) {
            (*n_labels)++;
        } else if (opcode != LOAD_LIB) {
            (*n_codes)++;
        }
    } while (eol != NULL);

    for (std_int_t i = 0; i < *n_codes; i++) {
        switch (Codes[i].opcode) {
            case BEQ0:
            case JUMP:
                Codes[i].i_operand = find_label(Codes[i].s_operand, Codes[i].line);
                STD_ASSERT_RV(Codes[i].i_operand >= 0, STD_RV_ERR_FAIL);
                break;
            case CALL:
                Codes[i].i_operand = find_label_ex(Codes[i].s_operand, (std_int_t) Codes[i].i_operand_ex, Codes[i].line);
                STD_ASSERT_RV(Codes[i].i_operand >= 0, STD_RV_ERR_FAIL);
                break;
            default:
                break;
        }
    }

#if 0
    for (int i = 0; i < *n_codes; i++) {
        STD_LOG(DISPLAY, "READ %d: %s %ld [%d]\n", i, get_opcode_name(Codes[i].opcode),
                Codes[i].i_operand, Codes[i].line);
    }
#endif

    return STD_RV_SUC;
}

/**
 * clean_codes
 * @brief
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t clean_codes()
{
    code_st *Codes = vm[get_std_thread_id()].Codes;
    const std_int_t *n_codes = &vm[get_std_thread_id()].n_codes;
    label_t *Labels = vm[get_std_thread_id()].Labels;
    const std_int_t *n_labels = &vm[get_std_thread_id()].n_labels;

    for (int i = 0; i < *n_codes; i++) {
        STD_LOG(INFO, "%d: %s %ld [%d]\n", i, get_opcode_name(Codes[i].opcode),
                Codes[i].i_operand, Codes[i].line);

        switch (Codes[i].opcode) {
            case PUSHI:
            case PUSHU:
            case PUSHA:
                break;
            case PUSHS:
                free_ownership_ownvalue(&vm[get_std_thread_id()].global_system_object_symbol, Codes[i].i_operand);
                break;

            case BEQ0:
            case JUMP:
            case CALL:
                FREE(Codes[i].s_operand);
                break;

            default:
                break;
        }
    }

    for (std_int_t i = 0; i < *n_labels; i++) {
        if (Labels[i].name) {
            FREE(Labels[i].name);
        }
    }
}

/**
 * find_label
 * @brief
 * @param   name
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t find_label(IN std_char_t *name, IN std_int_t line)
{
    const label_t *Labels = vm[get_std_thread_id()].Labels;
    std_int_t n_labels = vm[get_std_thread_id()].n_labels;

    for (std_int_t j = 0; j < n_labels; j++) {
        if (strcmp(Labels[j].name, name) == 0)
            return Labels[j].addr;
    }

    STD_LOG(ERR, "label '%s' is undefined, CHECK line:[%d]\n", name, line);

    return STD_RV_ERR_UNEXPECTED;
}

/**
 * find_label_ex
 * @brief   
 * @param   name
 * @param   args_count
 * @param   line
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t find_label_ex(IN std_char_t *name, IN std_int_t args_count, IN std_int_t line)
{
    const label_t *Labels = vm[get_std_thread_id()].Labels;
    std_int_t n_labels = vm[get_std_thread_id()].n_labels;

    for (std_int_t j = 0; j < n_labels; j++) {
        if (strcmp(Labels[j].name, name) == 0) {
            if (Labels[j].args_count != args_count) {
                STD_LOG(ERR, "Please check line:[%d], CALL '%s' is not correct!\n", line, name);
                return STD_RV_ERR_UNEXPECTED;
            }
            return Labels[j].addr;
        }
    }

    STD_LOG(ERR, "Please check line:[%d], the label '%s' is undefined.\n", line, name);

    return STD_RV_ERR_UNEXPECTED;
}