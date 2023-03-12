/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_library.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#include "virtual_machine_library.h"
#include "json-maker.h"
#include "shell.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "tiny-json.h"
#include "virtual_machine.h"
#include "virtual_machine_safe_var.h"

std_int_t register_id[128];
extern environment_vm_t vm[128];

STD_CALL std_void_t read_code(IN std_char_t *buffer, IN std_char_t *entry);


/**
 * library_func_register
 * @brief   
 * @param   func_name
 * @param   arg_counts
 * @param   func
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_func_register(IN std_int_t *reg_id, IN std_char_t *func_name, IN std_int_t arg_counts, IN register_func func)
{
    std_char_t buffer[BUF_SIZE_512] = "\0";
    std_size_t buffer_index = 0;
    std_char_t key[BUF_SIZE_32] = "\0";
    func_entry_t *func_entry;

    *reg_id = *reg_id + 1;

    snprintf(buffer + buffer_index, sizeof(buffer) - buffer_index, "ENTRY %s %d\n", func_name, arg_counts);
    buffer_index = std_safe_strlen(buffer, sizeof(buffer));

    snprintf(buffer + buffer_index, sizeof(buffer) - buffer_index, "FRAME %d\n", 0);
    buffer_index = std_safe_strlen(buffer, sizeof(buffer));

    for (std_int_t i = 0; i < arg_counts; ++i) {
        snprintf(buffer + buffer_index, sizeof(buffer) - buffer_index, "VAR_A %d\n", i);
        buffer_index = std_safe_strlen(buffer, sizeof(buffer));
    }

    for (std_int_t i = 0; i < arg_counts; ++i) {
        snprintf(buffer + buffer_index, sizeof(buffer) - buffer_index, "SYM_A %d\n", i);
        buffer_index = std_safe_strlen(buffer, sizeof(buffer));
    }

    snprintf(buffer + buffer_index, sizeof(buffer) - buffer_index, "CUSTOM %d\n", *reg_id);
    buffer_index = std_safe_strlen(buffer, sizeof(buffer));

    for (std_int_t i = 0; i < arg_counts; ++i) {
        snprintf(buffer + buffer_index, sizeof(buffer) - buffer_index, "VAR_A_CLEAN %d\n", i);
        buffer_index = std_safe_strlen(buffer, sizeof(buffer));
    }

    snprintf(buffer + buffer_index, sizeof(buffer) - buffer_index, "RET \n");

    snprintf(key, sizeof(key), "%d", *reg_id);

    func_entry = (func_entry_t *) CALLOC(sizeof(func_entry_t), 1);
    func_entry->arg_counts = arg_counts;
    func_entry->reg_func = func;

    std_lock_free_key_hash_add(vm[get_std_thread_id()].custom_func_hash, key, std_safe_strlen(key, sizeof(key)), func_entry);
    read_code(buffer, func_name);
}


/**
 * json_create_walk_callback
 * @brief   
 * @param   key
 * @param   data
 * @param   callback_arg
 * @return  std_void_t
 */
std_void_t json_create_walk_callback(const std_char_t *key, std_void_t **data, std_void_t *callback_arg)
{
    const std_char_t *arg_name = key;
    own_value_t value = (own_value_t) *data;
    std_char_t **dest = (std_char_t **) callback_arg;

    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
    own_value_type_t value_type = get_own_value_type(value);

    switch (value_type) {

        case OWN_TYPE_NULL:
            break;
        case OWN_TYPE_NUMBER:
            STD_LOG(INFO, "%s NUM value:%ld\n",
                    arg_name, get_own_value_number(value));
            *dest = json_verylong(*dest, arg_name,
                                  get_own_value_number(value));
            break;
        case OWN_TYPE_DOUBLE:
            break;
        case OWN_TYPE_BOOL:
            STD_LOG(INFO, "%s BOOL value:%ld\n",
                    arg_name, get_own_value_bool(value));
            *dest = json_verylong(*dest, arg_name,
                                  get_own_value_bool(value));
            break;
        case OWN_TYPE_ADDRESS:
            STD_LOG(INFO, "%s ADDRESS value:%p\n",
                    arg_name, get_own_value_address(value));
            *dest = json_verylong(*dest, arg_name,
                                  (intptr_t) get_own_value_address(value));
            break;
        case OWN_TYPE_OBJECT:
            break;
        case OWN_TYPE_OBJECT_SYMBOL:
            break;
        case OWN_TYPE_OBJECT_STRING:
            STD_LOG(INFO, "%s STRING value:%s\n",
                    arg_name, get_own_value_object_string(value));
            *dest = json_str(*dest, arg_name, get_own_value_object_string(value));
            break;
        default:
            break;
    }
}

/**
 * library_make_json
 * @brief   
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_make_json(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_name_value_hash;
    std_char_t request_string[BUF_SIZE_4096] = "\0";
    std_char_t *dest;
    own_value_t ret_obj;

    ret_obj = Pop(thread_id);
    obj_name_value_hash = Pop(thread_id);
    obj_name_value_hash = get_VAR(obj_name_value_hash, NAN_BOX_Null, STD_BOOL_FALSE);

    memset(request_string, 0, sizeof(request_string));
    dest = json_objOpen(request_string, NULL);

    walk_VAR_with_hash_type(get_own_value_object_symbol(obj_name_value_hash), json_create_walk_callback, &dest);

    dest = json_objClose(dest);
    json_end(dest);

    STD_LOG(INFO, "request_string:%s\n", request_string);
    set_VAR(ret_obj, NAN_BOX_Null, make_own_value_object_string(request_string));
}


/**
 * json_parse_walk_callback
 * @brief   
 * @param   key
 * @param   data
 * @param   callback_arg
 * @return  std_void_t
 */
std_void_t json_parse_walk_callback(const std_char_t *key, std_void_t **data, std_void_t *callback_arg)
{
    const std_char_t *arg_name = key;
    own_value_t *pp_value = (own_value_t *) data;
    own_value_t value = (own_value_t) *data;
    json_t const *json = (json_t *) callback_arg;
    json_t const *json_value;
    std_64_t ret_number;
    const std_char_t *ret_string = NULL;
    ownership_object_t *object;
    ownership_object_t *new_object;
    std_char_t *string;

    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
    own_value_type_t value_type = get_own_value_type(value);

    switch (value_type) {
        case OWN_TYPE_NULL:
            break;
        case OWN_TYPE_NUMBER:
            json_value = json_getProperty(json, arg_name);
            ret_number = json_getInteger(json_value);
            *pp_value = make_own_value_number(ret_number);
            break;
        case OWN_TYPE_DOUBLE:
            break;
        case OWN_TYPE_BOOL:
            break;
        case OWN_TYPE_ADDRESS:
            json_value = json_getProperty(json, arg_name);
            ret_number = json_getInteger(json_value);
            *pp_value = make_own_value_address((std_void_t *) ret_number);
            break;
        case OWN_TYPE_OBJECT:
            break;
        case OWN_TYPE_OBJECT_SYMBOL:
            break;
        case OWN_TYPE_OBJECT_STRING:
            json_value = json_getProperty(json, arg_name);
            ret_string = json_getValue(json_value);
            object = get_own_value_object(value);
            string = get_own_value_string(object->value);
            *pp_value = make_own_value_object_string(ret_string);
            new_object = get_own_value_object(*pp_value);
            memcpy(&new_object->owner_token_signature, &object->owner_token_signature, sizeof(object->owner_token_signature));

            FREE(string);
            FREE(object);
            break;
        default:
            break;
    }
}


/**
 * library_parse_json
 * @brief   
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_parse_json(IN std_int_t thread_id, IN std_int_t args)
{
    json_t const *json = NULL;
    own_value_t obj_json_string;
    own_value_t obj_name_value_hash;
    json_t mem[32];
    std_char_t *json_string;

    obj_name_value_hash = Pop(thread_id);
    obj_json_string = Pop(thread_id);

    obj_name_value_hash = get_VAR(obj_name_value_hash, NAN_BOX_Null, STD_BOOL_FALSE);
    obj_json_string = get_VAR(obj_json_string, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(obj_json_string != NAN_BOX_Null, );
    STD_ASSERT_RV(obj_name_value_hash != NAN_BOX_Null, );

    STD_ASSERT_RV(get_own_value_type(obj_json_string) == OWN_TYPE_OBJECT_STRING, );

    json_string = strdup(get_own_value_object_string(obj_json_string));

    json = json_create(json_string, mem, sizeof mem / sizeof *mem);
    STD_ASSERT_RV(json != NULL, );

    walk_VAR_with_hash_type(get_own_value_object_symbol(obj_name_value_hash), json_parse_walk_callback, (std_void_t *) json);

    FREE(json_string);
}


/**
 * library_print
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_print(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj[32];

    STD_ASSERT_RV(args <= 31, );

    for (std_int_t i = 0; i < args; ++i) {
        obj[i] = Pop(thread_id);
    }

    for (std_int_t i = args - 1; i >= 0; --i) {
        STD_ASSERT_RV(i <= 31, );
        print_own_value(obj[i]);
    }

    STD_LOG(DISPLAY, "\n");
}

/**
 * library_assert
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_assert(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj1;
    own_value_t obj2;
    std_char_t *string;

    obj2 = Pop(thread_id);
    obj1 = Pop(thread_id);

    string = get_own_value_object_string(get_VAR(obj2, NAN_BOX_Null, STD_BOOL_FALSE));
    if (STD_BOOL_TRUE == get_own_value_bool(get_VAR(obj1, NAN_BOX_Null, STD_BOOL_FALSE))) {
        STD_LOG(INFO, "ASSERT %s OK! \n", string);
    } else {
        STD_LOG(DISPLAY, "ASSERT %s FAILED!\n", string);
    }
}

/**
 * library_convert
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_convert(IN std_int_t thread_id, IN std_int_t args)
{
    //    own_value_t obj1;
    //    own_value_t obj2;
    //    own_value_t obj_convert;
    //    const std_char_t *string;
    //    own_value_t ret_obj;
    //
    //    ret_obj =  Pop(thread_id);
    //    obj2 =  Pop(thread_id);
    //    obj1 =  Pop(thread_id);
    //
    //    string = get_own_value_string(get_VAR(obj1,
    //                                       NAN_BOX_Null,
    //                                       STD_BOOL_FALSE));
    //
    //    if (strcmp(string, "ADDRESS2OBJECT") == 0) {
    ////        obj_convert =  get_own_value_address(get_VAR(obj2,
    ////                                                              NAN_BOX_Null,
    ////                                                              STD_BOOL_FALSE));
    ////    } else if (strcmp(string, "OBJECT2ADDRESS") == 0) {
    ////        obj_convert = make_own_value_address(obj2);
    ////    } else if (strcmp(string, "STRING2ADDRESS") == 0) {
    ////        STD_ASSERT_RV(obj2->op == TYPE_STRING, );
    ////        obj_convert = make_own_value_address(obj2->value.string);
    //    } else {
    //        STD_LOG(ERR, "ADDRESS2OBJECT or OBJECT2ADDRESS\n");
    //        Push(thread_id, (std_u64_t) NAN_BOX_Null);
    //        Push(thread_id, (std_u64_t) make_own_value_bool(STD_BOOL_FALSE));
    //        return;
    //    }
    //
    //    set_VAR(ret_obj, NAN_BOX_Null, obj_convert);
}

/**
 * library_check_type
 * @brief   
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_check_type(IN std_int_t thread_id, IN std_int_t args)
{
        own_value_t obj_check;
        own_value_t obj_tuple;
        own_value_t obj_key;
        own_value_t ret_obj;
        std_char_t const *check_string;

        ret_obj =  Pop(thread_id);
        obj_tuple =  Pop(thread_id);
        obj_check =  Pop(thread_id);

        obj_check = get_VAR(obj_check, NAN_BOX_Null, STD_BOOL_FALSE);
        obj_tuple = get_VAR(obj_tuple, NAN_BOX_Null, STD_BOOL_FALSE);
        check_string = get_own_value_type_string(obj_check);
        obj_key = make_own_value_object_string(check_string);
        own_value_t check = get_VAR(obj_tuple, obj_key, STD_BOOL_FALSE);

        FREE(get_own_value_object_string(obj_key));
        FREE(get_own_value_object(obj_key));

        set_VAR(ret_obj, NAN_BOX_Null, make_own_value_bool(check != NAN_BOX_Null?STD_BOOL_TRUE:STD_BOOL_FALSE));
}

/**
 * library_random_number
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_random_number(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj1;
    own_value_t ret_obj;

    ret_obj = Pop(thread_id);
    obj1 = Pop(thread_id);
    obj1 = get_VAR(obj1, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(get_own_value_type(obj1) == OWN_TYPE_NUMBER, );

    std_int_t random_number = (std_int_t) get_own_value_number(obj1);
    std_u64_t value;

    if (32 == random_number) {
        value = (std_random_u64() % UINT32_MAX);
    } else if (16 == random_number) {
        value = (std_random_u64() % UINT16_MAX);
    } else {
        value = std_random_u64();
    }

    set_VAR(ret_obj, NAN_BOX_Null, make_own_value_number(value));
    STD_LOG(INFO, "%s:%lu\n", __FUNCTION__, value);
}

/**
 * library_random_address
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_random_address(IN std_int_t thread_id, IN std_int_t args)
{
    std_64_t value;
    own_value_t ret_obj;

    ret_obj = Pop(thread_id);

    value = (std_64_t) (std_random_u64() % INT32_MAX);

    set_VAR(ret_obj, NAN_BOX_Null, make_own_value_address((std_void_t *) value));
    STD_LOG(INFO, "%s:%lu\n", __FUNCTION__, value);
}

/**
 * library_random_string
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_random_string(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj1;
    own_value_t ret_obj;

    ret_obj = Pop(thread_id);
    obj1 = Pop(thread_id);
    obj1 = get_VAR(obj1, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(get_own_value_type(obj1) == OWN_TYPE_NUMBER, );

    std_int_t length = (std_int_t) get_own_value_number(obj1);
    std_char_t *value = NULL;
    value = std_random_string(length);

    if (value != NULL) {
        set_VAR(ret_obj, NAN_BOX_Null, make_own_value_object_string(value));

        STD_LOG(INFO, "%s:%s\n", __FUNCTION__, value);
        FREE(value);
    } else {
        set_VAR(ret_obj, NAN_BOX_Null, NAN_BOX_Null);
    }
}


/**
 * library_create_instance
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_create_instance(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_iid;
    own_value_t obj_args;
    const std_char_t *iid_string;
    const std_char_t *args_string;
    std_void_t *p_handle = NULL;
    own_value_t ret_obj;

    ret_obj = Pop(thread_id);
    obj_args = Pop(thread_id);
    obj_iid = Pop(thread_id);

    iid_string = get_own_value_object_string(get_VAR(obj_iid, NAN_BOX_Null, STD_BOOL_FALSE));
    args_string = get_own_value_object_string(get_VAR(obj_args, NAN_BOX_Null, STD_BOOL_FALSE));

    cmd_create_instance(iid_string, std_safe_strlen(iid_string, BUF_SIZE_128), &p_handle, args_string, std_safe_strlen(args_string, BUF_SIZE_128));

    set_VAR(ret_obj, NAN_BOX_Null, make_own_value_address(p_handle));
}


/**
 * library_delete_instance
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_delete_instance(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_iid;
    own_value_t obj_handle;
    const std_char_t *iid_string;
    std_void_t *p_handle = NULL;

    obj_handle = Pop(thread_id);
    obj_iid = Pop(thread_id);

    iid_string = get_own_value_object_string(get_VAR(obj_iid, NAN_BOX_Null, STD_BOOL_FALSE));
    p_handle = get_own_value_address(get_VAR(obj_handle, NAN_BOX_Null, STD_BOOL_FALSE));

    cmd_delete_instance(iid_string, std_safe_strlen(iid_string, BUF_SIZE_128), &p_handle);
    set_VAR(obj_handle, NAN_BOX_Null, NAN_BOX_Null);
}


/**
 * library_make_array
 * @brief
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_string_to_array(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_string;
    std_char_t *string_string = NULL;

    own_value_t ret_obj;
    ownership_object_symbol_t *ret_symbol;

    ret_obj = Pop(thread_id);
    ret_obj = get_VAR(ret_obj, NAN_BOX_Null, STD_BOOL_FALSE);

    obj_string = Pop(thread_id);
    obj_string = get_VAR(obj_string, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(ret_obj != NAN_BOX_Null, );
    STD_ASSERT_RV(obj_string != NAN_BOX_Null, );

    STD_ASSERT_RV(get_own_value_type(obj_string) == OWN_TYPE_OBJECT_STRING, );

    string_string = get_own_value_object_string(obj_string);
    STD_LOG(INFO, "string_string:%s\n", string_string);

    ret_symbol = get_own_value_object_symbol(ret_obj);
    del_VARS(ret_obj, STD_BOOL_TRUE);

    declare_VAR_with_array_type(ret_symbol, (std_int_t)strlen(string_string), NAN_BOX_Null);

    for (std_int_t i = 0; i < strlen(string_string); i++){
        own_value_t own_value;
        std_char_t tmp_char[BUF_SIZE_32] = "\0";

        sprintf(tmp_char, "%c", string_string[i]);
        own_value = make_own_value_object_string(tmp_char);
        set_VAR(ret_obj, i, own_value);
    }
}

/**
 * library_array_to_string
 * @brief
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_array_to_string(IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_array;
    std_char_t tmp_buffer[BUF_SIZE_1024] = "\0";
    own_value_t ret_obj;
    std_int_t array_size;

    ret_obj = Pop(thread_id);

    obj_array = Pop(thread_id);
    obj_array = get_VAR(obj_array, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(ret_obj != NAN_BOX_Null, );
    STD_ASSERT_RV(obj_array != NAN_BOX_Null, );

    STD_ASSERT_RV(get_own_value_type(obj_array) == OWN_TYPE_OBJECT_SYMBOL, );

    std_int_t obj_item_type = get_own_value_object_symbol(obj_array)->env_value.symbol_type;
    STD_ASSERT_RV(obj_item_type == array_type, );
    array_size = get_VAR_total_with_array_type(get_own_value_object_symbol(obj_array));

    for (std_int_t i = 0; i < array_size; i++){
        own_value_t item_value;
        std_char_t item_string[BUF_SIZE_128];

        item_value = get_VAR(obj_array, i, STD_BOOL_FALSE);
        print_own_value_to_buf(item_value, item_string, STD_BOOL_FALSE);
        strcat(tmp_buffer, item_string);
    }

    set_VAR(ret_obj, NAN_BOX_Null, make_own_value_object_string(tmp_buffer));
}

/**
 * library_register
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_register()
{
    std_int_t thread_id = get_std_thread_id();

    register_id[thread_id] = 33;

    for (std_int_t i = 1; i <= 10; ++i) {
        std_char_t name[BUF_SIZE_32] = "\0";
        snprintf(name, sizeof(name), "print_%d", i);
        library_func_register(&register_id[thread_id], name, i, library_print);
    }

    library_func_register(&register_id[thread_id], "assert_2", 2, library_assert);

    library_func_register(&register_id[thread_id], "random_number", 2, library_random_number);
    library_func_register(&register_id[thread_id], "random_address", 1, library_random_address);
    library_func_register(&register_id[thread_id], "random_string", 2, library_random_string);

    library_func_register(&register_id[thread_id], "make_json", 2, library_make_json);
    library_func_register(&register_id[thread_id], "parse_json", 2, library_parse_json);

    library_func_register(&register_id[thread_id], "convert", 2, library_convert);

    library_func_register(&register_id[thread_id], "check_type", 3, library_check_type);

    library_func_register(&register_id[thread_id], "create_instance", 3, library_create_instance);
    library_func_register(&register_id[thread_id], "delete_instance", 2, library_delete_instance);
    library_func_register(&register_id[thread_id], "string_to_array", 2, library_string_to_array);
    library_func_register(&register_id[thread_id], "array_to_string", 2, library_array_to_string);
}
