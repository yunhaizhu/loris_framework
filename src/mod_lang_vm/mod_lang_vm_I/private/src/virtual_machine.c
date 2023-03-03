/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */


#include "virtual_machine.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "virtual_machine_EXEC.h"
#include "virtual_machine_READ.h"
#include "virtual_machine_library.h"
#include <dlfcn.h>

environment_vm_t vm[128];

/**
 * vm_init
 * @brief   
 * @param   buffer
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_init(IN const std_char_t *name, IN const std_char_t *buffer)
{
    std_int_t thread_it = get_std_thread_id();
    memset((std_void_t *) &vm[thread_it], 0, sizeof(environment_vm_t));

    vm[thread_it].custom_func_hash = std_lock_free_key_hash_create(128);
    vm[thread_it].symbol_hash = std_lock_free_key_hash_create(128);
    vm[thread_it].auto_free_hash = std_lock_free_key_hash_create(128);
    library_register();
    rsa_gen_keys(&vm[thread_it].global_system_object_symbol.pub, &vm[thread_it].global_system_object_symbol.pri, PRIME_SOURCE_FILE);

    return read_code(buffer);
}

STD_CALL std_void_t dump_codes(IN const std_char_t *name, IN std_int_t start_pc);
STD_CALL std_void_t DISPLAY_CODE();
/**
 * vm_execute
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_execute(IN const std_char_t *name, IN std_u64_t u64_key)
{
    std_int_t pc;
    std_u64_t tick;

    pc = find_label("function__main", 0);
    if (pc >= 0) {
#if EXEC_COMPILED_CODE
        typedef std_rv_t (*execute_compiled_code_t)(IN std_int_t start_pc);

        std_void_t *dl_handle;
        execute_compiled_code_t execute_compiled_code;
        std_char_t dl_name[BUF_SIZE_128] = "\0";

        snprintf(dl_name, sizeof(dl_name), "dynamic_exec/obj/lib_%s-%lu.so", name, u64_key);

        dl_handle = dlopen(dl_name, RTLD_LAZY | RTLD_LOCAL);
        if (dl_handle != NULL) {
            execute_compiled_code = dlsym(dl_handle, "execute_compiled_code");

            if (execute_compiled_code) {
                TICK(tick);
                execute_compiled_code(pc);
                TOCK(tick);
                STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));
            }
            STD_LOG(DISPLAY, "executed_compiled_code\n");
        } else {
#if DUMP_EXEC_CODE
            DISPLAY_CODE();
#endif
            TICK(tick);
            execute_code(pc, STD_BOOL_TRUE);
            TOCK(tick);
            STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));

            dump_codes(name, pc);
            std_char_t cmd[BUF_SIZE_128] = "\0";
            snprintf(cmd, sizeof(cmd), "make -C dynamic_exec all arg1=%s arg2=%lu", name, u64_key);
            std_int_t ret = system(cmd);
            STD_LOG(DISPLAY, "compile %s %s\n", cmd, ret == 0 ? "SUCCESS" : "FAIL");
        }
#else
#if DUMP_EXEC_CODE
        DISPLAY_CODE();
#endif
        TICK(tick);
        execute_code(pc, STD_BOOL_TRUE);
        TOCK(tick);
        STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));
#endif
    }
    return STD_RV_SUC;
}

/**
 * vm_call_prepare_stack
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t vm_call_prepare_stack()
{
    std_int_t thread_id = get_std_thread_id();
    code_st *Codes = vm[thread_id].Codes;
    std_int_t const *nCodes = &vm[thread_id].n_codes;
    std_int_t *Sp = &vm[thread_id].Sp;
    std_int_t *Fp = &vm[thread_id].Fp;

    *Sp = *Fp = MAX_STACK - 1;

    Push(thread_id, *Fp);
    *Fp = *Sp;
    *Sp -= 1;

    Codes[*nCodes].opcode = RET;
}

/**
 * vm_call_prepare_args
 * @brief   
 * @return  STD_CALL object_t *
 */
//STD_CALL own_object_t *vm_call_prepare_args()
//{
//    std_int_t thread_id = get_std_thread_id();
//    std_int_t const *nCodes = &vm[thread_id].n_codes;
//    own_object_t *arg_x = make_object_number(1000);
//    own_object_t *arg_y = make_object_number(300);
//    own_object_t *ret = make_object_symbol();
//
//    std_char_t key[BUF_SIZE_128] = "\0";
//    snprintf(key, sizeof(key), "local_%s_%d", "call_entry", 1);
//    std_lock_free_key_hash_add(vm[thread_id].symbol_hash, key, std_safe_strlen(key, sizeof(key)), ret);
//    declare_VAR(ret->value.symbol, var_type, 0, NULL, STD_BOOL_TRUE);
//
//    Push(thread_id, (intptr_t)ret);
//    Push(thread_id, (intptr_t)arg_y);
//    Push(thread_id, (intptr_t)arg_x);
//    Push(thread_id, *nCodes);
//
//    return ret;
//}

/**
 * vm_call
 * @brief   
 * @param   name
 * @param   u64_key
 * @param   func_name
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_call(IN const std_char_t *name, IN std_u64_t u64_key, IN std_char_t *func_name)
{
    //    std_int_t pc;
    //    std_u64_t tick;
    //    std_char_t func[BUF_SIZE_128] = "\0";
    //    own_object_t *ret;
    //
    //    snprintf(func, sizeof(func), "function__%s", func_name);
    //
    //    pc = find_label(func, 0);
    //    if (pc >= 0) {
    //        TICK(tick);
    //        vm_call_prepare_stack();
    //        ret = vm_call_prepare_args();
    //
    //        execute_code(pc, STD_BOOL_FALSE);
    //
    //        STD_LOG(DISPLAY, "ret:%d\n", get_object_number(get_VAR(ret, NULL, STD_BOOL_FALSE)));
    //        set_VAR(ret, NULL, STD_BOOL_FALSE);
    //
    //        TOCK(tick);
    //        STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));
    //    }
    return STD_RV_SUC;
}


/**
 * cleanup_symbol_object_callback
 * @brief   
 * @param   value
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t cleanup_symbol_object_callback(IN std_void_t *value, IN const std_void_t *callback_arg)
{
    ownership_object_t *object = (ownership_object_t *) ((std_u64_t)value & NAN_BOX_MASK_PAYLOAD_PTR);
    ownership_object_symbol_t *symbol = (ownership_object_symbol_t *) (object->value & NAN_BOX_MASK_PAYLOAD_PTR);

    FREE(symbol);
    FREE(object);
}

/**
 * cleanup_auto_free_object_callback
 * @brief   
 * @param   value
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t cleanup_auto_free_object_callback(IN std_void_t *value, IN const std_void_t *callback_arg)
{
    ownership_object_t *object = (ownership_object_t *) value;
//    ownership_object_symbol_t const *symbol = (const ownership_object_symbol_t *) callback_arg;

//    free_ownership_object(symbol, object);

    STD_LOG(DEBUG, "FREE %p %p\n", object);
}


/**
 * vm_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_cleanup(IN const std_char_t *name)
{
    clean_codes();

    vm[get_std_thread_id()].n_codes = 0;

    std_lock_free_key_hash_value_destroy(vm[get_std_thread_id()].custom_func_hash);
    std_lock_free_key_hash_value_callback_destroy(vm[get_std_thread_id()].symbol_hash, cleanup_symbol_object_callback, NULL);
    std_lock_free_key_hash_value_callback_destroy(vm[get_std_thread_id()].auto_free_hash, cleanup_auto_free_object_callback, &vm[get_std_thread_id()].global_system_object_symbol);

    vm[get_std_thread_id()].custom_func_hash = NULL;
    vm[get_std_thread_id()].symbol_hash = NULL;
    vm[get_std_thread_id()].auto_free_hash = NULL;

    memset((std_void_t *) &vm[get_std_thread_id()], 0, sizeof(environment_vm_t));

    return STD_RV_SUC;
}

typedef struct gen_code_table_s {
    char *name;
    int opcode;
} gen_code_table_t;

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
        {"STOREL_C", STOREL_C},
        {"STOREL_NC", STOREL_NC},
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