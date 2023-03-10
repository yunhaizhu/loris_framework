/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_ast.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */

#include "lang_ast.h"
#include "std_lock_free_key_hash.h"

std_lock_free_key_hash_t *global_parse_error_need_clean_lang_ast_hash = NULL;
std_lock_free_key_hash_t *global_symbol_hash = NULL;
std_bool_t create_id = STD_BOOL_FALSE;
std_bool_t create_func = STD_BOOL_FALSE;
std_char_t *function_name = NULL;
#define FUNC_EXTERN_LEN 22
#define FUNC_EXTERN_EXTERN_LEN 100
std_bool_t parse_no_error = STD_BOOL_FALSE;


std_char_t *global_func_extern[FUNC_EXTERN_EXTERN_LEN] = {"print",
                                                          "install",
                                                          "uninstall",
                                                          "start",
                                                          "stop",
                                                          "show",
                                                          "help",
                                                          "grace_exit",
                                                          "exit",
                                                          "run",
                                                          "ps",
                                                          "assert",
                                                          "random_number",
                                                          "random_address",
                                                          "random_string",
                                                          "make_json",
                                                          "parse_json",
                                                          "create_instance",
                                                          "delete_instance",
                                                          "debug",
                                                          "convert",
                                                          "check_type"};
std_int_t global_func_extern_idx = FUNC_EXTERN_LEN;

/**
 * parse_error_need_auto_free
 * @brief   
 * @param   ast
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t parse_error_need_auto_free(lang_ast_t *ast)
{
    parse_no_error = STD_BOOL_TRUE;

    if (NULL == global_parse_error_need_clean_lang_ast_hash) {
        global_parse_error_need_clean_lang_ast_hash =
                std_lock_free_key_hash_create(BUF_SIZE_128);
    }
    std_char_t new_name[BUF_SIZE_128];
    snprintf(new_name, sizeof(new_name), "ast_%p", ast);
    std_lock_free_key_hash_add(global_parse_error_need_clean_lang_ast_hash, new_name, std_safe_strlen(new_name, sizeof(new_name)), ast);
}

/**
 * make_lang_ast_symbol
 * @brief   
 * @param   name
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_symbol(IN std_char_t *name, IN std_char_t *source_file, IN std_int_t line)
{
    lang_ast_t *ast = NULL;

    ast = (lang_ast_t *) CALLOC(sizeof(lang_ast_t), 1);
    ast->op = SYMBOL_OP;
    ast->symbol = lookup_lang_ast_symbol(name, STD_BOOL_TRUE);
    ast->debug_info.source_file = source_file;
    ast->debug_info.line = line;

    FREE(name);

    STD_LOG(DEBUG, "NEW ast:%p symbol:%p name:%s line:%d\n", ast, ast->symbol, get_lang_ast_symbol_name(ast), ast->debug_info.line);
    if (ast->symbol == NULL) {
        FREE(ast);

        return NULL;
    }

    parse_error_need_auto_free(ast);
    return ast;
}


/**
 * make_lang_ast
 * @brief
 * @param   op
 * @param   left
 * @param   right
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast(IN lang_ast_code_t op, IN lang_ast_t *left, IN lang_ast_t *right, IN std_char_t *source_file, IN std_int_t line)
{
    lang_ast_t *ast = NULL;

    ast = (lang_ast_t *) CALLOC(sizeof(lang_ast_t), 1);
    ast->op = op;
    ast->left = left;
    ast->right = right;

    ast->debug_info.source_file = source_file;
    ast->debug_info.line = line;

    STD_LOG(DEBUG, "NEW %s %p %d\n", __FUNCTION__, ast, ast->debug_info.line);
    print_lang_ast(ast);
    STD_LOG(DEBUG, "\n");

    parse_error_need_auto_free(ast);
    return ast;
}

/**
 * make_lang_ast_number
 * @brief   
 * @param   number
 * @param   number_type
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_number(IN std_64_t number, IN std_char_t *source_file, IN std_int_t line)
{
    lang_ast_t *ast = NULL;

    ast = (lang_ast_t *) CALLOC(sizeof(lang_ast_t), 1);
    ast->op = NUMBER_OP;
    ast->number_type = NUMBER_TYPE_I64;
    ast->debug_info.source_file = source_file;
    ast->debug_info.line = line;
    ast->number.i64 = number;

    STD_LOG(DEBUG, "NEW make_lang_ast_number:name:%ld %p\n", number, ast);

    parse_error_need_auto_free(ast);
    return ast;
}

/**
 * make_lang_ast_u_number
 * @brief   
 * @param   number
 * @param   number_type
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_u_number(IN std_u64_t number, IN std_char_t *source_file, IN std_int_t line)
{
    lang_ast_t *ast = NULL;

    ast = (lang_ast_t *) CALLOC(sizeof(lang_ast_t), 1);
    ast->op = NUMBER_OP;
    ast->number_type = NUMBER_TYPE_U64;
    ast->debug_info.source_file = source_file;
    ast->debug_info.line = line;
    ast->number.u64 = number;

    STD_LOG(DEBUG, "NEW :name:%ld %p\n", number, ast);

    parse_error_need_auto_free(ast);
    return ast;
}

/**
 * make_lang_ast_bool
 * @brief   
 * @param   bool_value
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_bool(IN std_bool_t bool_value, IN std_char_t *source_file, IN std_int_t line)
{
    lang_ast_t *ast = NULL;

    ast = (lang_ast_t *) CALLOC(sizeof(lang_ast_t), 1);
    ast->op = BOOL_OP;
    ast->bool_value = bool_value;
    ast->debug_info.source_file = source_file;
    ast->debug_info.line = line;

    STD_LOG(DEBUG, "NEW make_lang_ast_bool: %p\n", ast);

    parse_error_need_auto_free(ast);
    return ast;
}


/**
 * make_lang_ast_string
 * @brief   
 * @param   string
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_string(IN std_char_t *string, IN std_char_t *source_file, IN std_int_t line)
{
    lang_ast_t *ast = NULL;

    ast = (lang_ast_t *) CALLOC(sizeof(lang_ast_t), 1);
    ast->op = STRING_OP;
    ast->string = string;
    ast->debug_info.source_file = source_file;
    ast->debug_info.line = line;

    STD_LOG(DEBUG, "NEW make_lang_ast_string:name:%s %p\n", string, ast);

    parse_error_need_auto_free(ast);
    return ast;
}


/**
 * make_lang_ast_address
 * @brief   
 * @param   address
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_address(IN std_void_t *address, IN std_char_t *source_file, IN std_int_t line)
{
    lang_ast_t *ast = NULL;

    ast = (lang_ast_t *) CALLOC(sizeof(lang_ast_t), 1);
    ast->op = ADDRESS_OP;
    ast->address = address;
    ast->debug_info.line = line;

    STD_LOG(DEBUG, "NEW make_lang_ast_address:name: %p %p\n", ast, address);
    return ast;
}

/**
 * lookup_lang_ast_symbol
 * @brief
 * @param   name
 * @return  STD_CALL symbol_t *
 */
STD_CALL symbol_t *lookup_lang_ast_symbol(IN std_char_t *name, IN std_bool_t check_error)
{
    symbol_t *symbol = NULL;
    std_bool_t find = STD_BOOL_FALSE;
    std_bool_t extend_func = STD_BOOL_TRUE;
    std_char_t new_name[BUF_SIZE_128];

    for (std_int_t i = 0; i < global_func_extern_idx; ++i) {
        if (0 == strcmp(name, global_func_extern[i])) {
            extend_func = STD_BOOL_FALSE;
            create_id = STD_BOOL_TRUE;
            break;
        }
    }

    if (NULL == global_symbol_hash) {
        global_symbol_hash =
                std_lock_free_key_hash_create(BUF_SIZE_128);
    }

    if (function_name != NULL && extend_func) {
        snprintf(new_name, sizeof(new_name), "%s--%s", function_name, name);
    } else {
        if (create_func == STD_BOOL_TRUE) {
            create_func = STD_BOOL_FALSE;
            snprintf(new_name, sizeof(new_name), "function__%s", name);
        } else {
            snprintf(new_name, sizeof(new_name), "%s", name);
        }
    }

    symbol = std_lock_free_key_hash_find(global_symbol_hash, new_name, std_safe_strlen(new_name, sizeof(new_name)));
    if (symbol != NULL) {
        find = STD_BOOL_TRUE;
    }

    if (STD_BOOL_FALSE == find) {
        if (STD_BOOL_TRUE == create_id) {
            create_id = STD_BOOL_FALSE;

            symbol = (symbol_t *) CALLOC(sizeof(symbol_t), 1);
            symbol->name = strdup(new_name);
            std_lock_free_key_hash_add(global_symbol_hash, new_name, std_safe_strlen(new_name, sizeof(new_name)), symbol);
            STD_LOG(DEBUG, "new symbol name:%s %p\n", new_name, symbol);
        } else {
            snprintf(new_name, sizeof(new_name), "function__%s", name);
            symbol = std_lock_free_key_hash_find(global_symbol_hash, new_name, std_safe_strlen(new_name, sizeof(new_name)));
            if (symbol == NULL && check_error) {
                STD_LOG(ERR,
                        "PARSE FAILED! You should declare '%s' first, please check source codes line[].\n",
                        name);
            }
        }
    }

    create_id = STD_BOOL_FALSE;

    return symbol;
}


/**
 * get_lang_ast_symbol
 * @brief
 * @param   ast
 * @return  STD_CALL symbol_t *
 */
STD_CALL symbol_t *get_lang_ast_symbol(lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == SYMBOL_OP, NULL);

    return ast->symbol;
}

/**
 * get_lang_ast_bool
 * @brief   
 * @param   ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t get_lang_ast_bool(IN const lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == BOOL_OP, 0);

    return ast->bool_value;
}

/**
 * get_lang_ast_number
 * @brief   
 * @param   ast
 * @return  STD_CALL std_64_t
 */
STD_CALL std_64_t get_lang_ast_number(IN const lang_ast_t *ast)
{
    std_64_t number = 0;

    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == NUMBER_OP, 0);

    if (ast->number_type == NUMBER_TYPE_I64) {
        number = ast->number.i64;
    } else {
        STD_LOG(ERR, "incorrect number_type %d, need call get_lang_ast_u_number() instead.\n",
                ast->number_type);
    }

    return number;
}

/**
 * get_lang_ast_u_number
 * @brief   
 * @param   ast
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t get_lang_ast_u_number(IN const lang_ast_t *ast)
{
    std_u64_t number = 0;

    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == NUMBER_OP, 0);

    if (ast->number_type == NUMBER_TYPE_U64) {
        number = ast->number.u64;
    } else {
        STD_LOG(ERR, "incorrect number_type %d, need call get_lang_ast_number() instead.\n",
                ast->number_type);
    }

    return number;
}

/**
 * get_lang_ast_string
 * @brief   
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_string(IN lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == STRING_OP, NULL);

    return ast->string;
}

/**
 * get_lang_ast_address
 * @brief   
 * @param   ast
 * @return  STD_CALL std_void_t *
 */
STD_CALL std_void_t *get_lang_ast_address(IN lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == ADDRESS_OP, NULL);

    return ast->address;
}

/**
 * get_lang_ast_symbol_name
 * @brief   
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_symbol_name(IN lang_ast_t *ast)
{
    STD_ASSERT_RV_WARN(ast != NULL, NULL);
    STD_ASSERT_RV_WARN(ast->op == SYMBOL_OP, NULL);
    STD_ASSERT_RV_WARN(ast->symbol != NULL, NULL);

    return ast->symbol->name;
}

/**
 * get_lang_ast_nth
 * @brief   
 * @param   ast
 * @param   nth
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *get_lang_ast_nth(IN lang_ast_t *ast, IN std_int_t nth)
{
    if ((NULL == ast) || (ast->op != LIST_OP)) {
        return NULL;
    }
    if (nth > 0) {
        return get_lang_ast_nth(ast->right, nth - 1);
    } else {
        return ast->left;
    }
}

/**
 * get_lang_ast_next
 * @brief   
 * @param   ast
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *get_lang_ast_next(IN lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == LIST_OP, NULL);

    return ast->right;
}

/**
 * add_lang_ast_last
 * @brief   
 * @param   last
 * @param   ast
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *add_lang_ast_last(IN lang_ast_t *last, IN lang_ast_t *ast)
{
    lang_ast_t *queue = NULL;

    if (NULL == last) {
        return make_lang_ast(LIST_OP, ast, NULL, NULL, 0);
    }
    queue = last;
    while (queue->right != NULL) {
        queue = queue->right;
    }
    queue->right = make_lang_ast(LIST_OP, ast, NULL, NULL, 0);

    return last;
}

/**
 * code_name
 * @brief   
 * @param   op
 * @return  STD_CALL static char *
 */
STD_CALL static char *code_name(IN enum lang_ast_code op)
{
    switch (op) {
        case LIST_OP:
            return "LIST";
        case NUMBER_OP:
            return "NUMBER";
        case ADDRESS_OP:
            return "ADDRESS";
        case STRING_OP:
            return "STRING";
        case SYMBOL_OP:
            return "SYMBOL";
        case EQ_OP:
            return "EQ_OP";
        case PLUS_OP:
            return "PLUS_OP";
        case MINUS_OP:
            return "MINUS_OP";
        case MUL_OP:
            return "MUL_OP";
        case LT_OP:
            return "LT_OP";
        case GT_OP:
            return "GT_OP";
        case GET_ARRAY_OP:
            return "GET_ARRAY_OP";
        case SET_ARRAY_OP:
            return "SET_ARRAY_OP";
        case CALL_OP:
            return "CALL_OP";
        case IF_STATEMENT:
            return "IF_STATEMENT";
        case BLOCK_STATEMENT:
            return "BLOCK_STATEMENT";
        case RETURN_STATEMENT:
            return "RETURN_STATEMENT";
        case WHILE_STATEMENT:
            return "WHILE_STATEMENT";
        case FOR_STATEMENT:
            return "FOR_STATEMENT";
        default:
            return "???";
    }
}

/**
 * print_lang_ast_number
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_lang_ast_number(IN const lang_ast_t *p)
{
    if (p->number_type == NUMBER_TYPE_I64) {
        STD_LOG(DEBUG, "%ld", p->number.i64);
    } else if (p->number_type == NUMBER_TYPE_U64) {
        STD_LOG(DEBUG, "%lu", p->number.u64);
    }
}

/**
 * print_lang_ast_list
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_lang_ast_list(IN lang_ast_t *p)
{
    lang_ast_t *p_next = p;

    STD_LOG(DEBUG, "(LIST ");
    while (p_next != NULL) {
        print_lang_ast(p_next->left);
        p_next = p_next->right;
        if (p_next != NULL) {
            STD_LOG(DEBUG, " ");
        }
    }
    STD_LOG(DEBUG, ")");
}

/**
 * print_lang_ast
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_lang_ast(IN lang_ast_t *p)
{
    if (NULL == p) {
        STD_LOG(DEBUG, "()");
        return;
    }

    switch (p->op) {
        case NUMBER_OP:
            print_lang_ast_number(p);
            break;
        case STRING_OP:
            STD_LOG(DEBUG, "%s", p->string);
            break;
        case ADDRESS_OP:
            STD_LOG(DEBUG, "%p", p->address);
            break;
        case SYMBOL_OP:
            STD_LOG(DEBUG, "'%s'", get_lang_ast_symbol_name(p));
            break;
        case LIST_OP:
            print_lang_ast_list(p);
            break;
        default:
            STD_LOG(DEBUG, "(%s ", code_name(p->op));
            print_lang_ast(p->left);
            STD_LOG(DEBUG, " ");
            print_lang_ast(p->right);
            STD_LOG(DEBUG, ")");
    }
}

/**
 * callback_destroy_lang_symbol
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t callback_destroy_lang_symbol(IN std_void_t *data, IN const std_void_t *callback_arg)
{
    symbol_t *symbol = (symbol_t *) data;

    STD_LOG(DEBUG, "FREE callback_destroy_symbol %p %p\n", symbol->name, symbol);
    FREE(symbol->name);

    FREE(symbol);
}

/**
 * callback_destroy_lang_ast
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t callback_destroy_lang_ast(IN std_void_t *data, IN const std_void_t *callback_arg)
{
    lang_ast_t *ast = (lang_ast_t *) data;

    if (ast->op == STRING_OP) {
        FREE(ast->string);
    }
    FREE(ast);
}

/**
 * clear_lang_ast_list
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t clear_lang_ast_list(IN lang_ast_t *p)
{
    lang_ast_t *p_next = p;
    lang_ast_t *p_need_free;

    STD_LOG(DEBUG, "(LIST \n");
    while (p_next != NULL) {
        p_need_free = p_next;
        STD_LOG(DEBUG, "FREE %p\n", p_need_free);
        clear_lang_ast(p_next->left);

        p_next = p_next->right;
        if (p_next != NULL) {
            STD_LOG(DEBUG, " ");
        }

        FREE(p_need_free);
    }
    STD_LOG(DEBUG, ")\n");
}

/**
 * clear_lang_ast
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t clear_lang_ast(IN lang_ast_t *p)
{
    if (NULL == p) {
        STD_LOG(DEBUG, "()");
        return;
    }

    switch (p->op) {
        case NUMBER_OP:
            STD_LOG(DEBUG, "NUMBER FREE %p\n", p);
            FREE(p);
            break;
        case STRING_OP:
            STD_LOG(DEBUG, "%s\n", p->string);
            STD_LOG(DEBUG, "STRING FREE %p\n", p->string);
            STD_LOG(DEBUG, "STRING FREE %p\n", p);
            FREE(p->string);
            FREE(p);
            break;
        case ADDRESS_OP:
            STD_LOG(DEBUG, "ADDRESS FREE %p\n", p);
            FREE(p);
            break;
        case SYMBOL_OP:
            STD_LOG(DEBUG, "SYMBOL FREE '%s' %p\n", get_lang_ast_symbol_name(p), p);

            if (p->symbol->type_symbol) {
                lang_ast_t *need_clear_lang_ast = p->symbol->type_symbol;
                p->symbol->type_symbol = NULL;
                clear_lang_ast(need_clear_lang_ast);
            }

            if (p->symbol->check_block) {
                lang_ast_t *need_clear_lang_ast = p->symbol->check_block;
                p->symbol->check_block = NULL;
                clear_lang_ast(need_clear_lang_ast);
            }

            FREE(p);
            break;
        case LIST_OP:
            clear_lang_ast_list(p);
            break;
        default:
            STD_LOG(DEBUG, "(%s \n", code_name(p->op));
            STD_LOG(DEBUG, "FREE %p\n", p);
            clear_lang_ast(p->left);
            STD_LOG(DEBUG, " \n");
            clear_lang_ast(p->right);
            STD_LOG(DEBUG, ")\n");

            FREE(p);
    }
}