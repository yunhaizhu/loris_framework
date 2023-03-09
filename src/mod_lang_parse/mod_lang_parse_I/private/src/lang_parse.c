/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_parse.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */

#include "lang_parse.h"
#include "lang_ast.h"
#include "lang_lex.h"
#include "mod_lang_compile.h"

extern std_lock_free_key_hash_t *global_parse_error_need_clean_lang_ast_hash;
extern std_lock_free_key_hash_t *global_symbol_hash;
extern std_bool_t parse_no_error;

extern mod_lang_compile_t *p_global_mod_compile;

extern std_bool_t create_id;
extern std_char_t *function_name;

lang_ast_t *primary_expr(lang_state_t *state);
lang_ast_t *expression(lang_state_t *state);
lang_ast_t *arg_list(lang_state_t *state);
lang_ast_t *vals_list(lang_state_t *state);
lang_ast_t *expr_key(lang_state_t *state);
lang_ast_t *statement(lang_state_t *state);
lang_ast_t *block(lang_state_t *state, std_bool_t nl_skip);
lang_ast_t *command_statement(lang_state_t *state);
lang_ast_t *command_statements(lang_state_t *state);
lang_ast_t *global_definition(lang_state_t *state);
lang_ast_t *global_definitions(lang_state_t *state);

/**
 * identifier
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *identifier(lang_state_t *state)
{
    lang_ast_t *ast;

    if (state->lex_lookahead == TOKEN_ID) {
        ast = make_lang_ast_symbol(state->value.string, state->source_name, state->source_line);
        lang_next(state);
        return ast;
    }
    lang_lex_error(state, "expect ID, unexpect %c %s\n", state->lex_lookahead, get_token_name(state->lex_lookahead));
}

/**
 * fake_type_check_ast
 * @brief   
 * @param   func_name
 * @param   check_name
 * @return  lang_ast_t *
 */
lang_ast_t *fake_type_check_ast(std_char_t *func_name, std_char_t *check_name)
{
    std_int_t jmp_ret;
    lang_state_t State;
    lang_state_t *state = &State;
    std_char_t source_template[BUF_SIZE_512] = " {\n"
                                               "    var %s__parameter_check_ret\n"
                                               "    check_type(%s, %s__type, %s__parameter_check_ret)\n"
                                               "    if (%s__parameter_check_ret == false) {\n"
                                               "        print(\"%s:%s check_type_failed\")\n"
                                               "        return\n"
                                               "    } else {\n"
                                               "        print(\"%s:%s check_type_success\")\n"
                                               "    }\n"
                                               "} \n";
    std_char_t source_buffer[BUF_SIZE_1024];
    lang_ast_t *block_stms;

    snprintf(source_buffer, sizeof(source_buffer), source_template,
             check_name,
             check_name, check_name, check_name,
             check_name,
             func_name, check_name,
             func_name, check_name);

    STD_LOG(DISPLAY, "%s\n", source_buffer);
    lang_lex_init(state, "fake_type_check_ast", source_buffer, sizeof(source_buffer));

    jmp_ret = setjmp(state->error_jump_buf);
    if (jmp_ret) {
        //        parse_error();
        lang_lex_cleanup(state);
        return NULL;
    }

    lang_next(state);

    block_stms = block(state, STD_BOOL_FALSE);

    lang_lex_cleanup(state);
    return block_stms;
}

/**
 * var_identifier
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *var_identifier(lang_state_t *state)
{
    lang_ast_t *ast = NULL;
    lang_ast_t *lang_ast_type_symbol = NULL;
    std_char_t check_symbol_name[BUF_SIZE_128] = "\0";
    std_char_t type_symbol_name[BUF_SIZE_128] = "\0";
    lang_ast_t *expr2 = NULL;

    if (state->lex_lookahead == TOKEN_VAR) {
        lang_next(state);
        if (lang_accept(state, TOKEN_ID)) {
            snprintf(check_symbol_name, sizeof(type_symbol_name), "%s", state->value.string);
            snprintf(type_symbol_name, sizeof(type_symbol_name), "%s__type", state->value.string);
            ast = make_lang_ast_symbol(state->value.string, state->source_name, state->source_line);

            if (lang_accept(state, ':')) {
                create_id = STD_BOOL_TRUE;
                lang_ast_type_symbol = make_lang_ast_symbol(strdup(type_symbol_name), state->source_name, state->source_line);
                lang_expect(state, '{');
                expr2 = vals_list(state);
                lang_expect(state, '}');
                ast->symbol->type_symbol = make_lang_ast(DECLARE_TUPLE_OP, lang_ast_type_symbol, expr2, state->source_name, state->source_line);

                ast->symbol->check_block = fake_type_check_ast(function_name, check_symbol_name);
            }
            return ast;
        } else {
            lang_lex_error(state, "expect VAR ID, unexpect %c %s\n", state->lex_lookahead, get_token_name(state->lex_lookahead));
        }
    }
    lang_lex_error(state, "expect VAR ID, unexpect %c %s\n", state->lex_lookahead, get_token_name(state->lex_lookahead));
}

/**
 * vals_list
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *vals_list(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *expr1 = NULL;

    expr1 = primary_expr(state);
    if (expr1) {
        head = make_lang_ast_list1(expr1, state->source_name, state->source_line);
    }

    while (lang_accept(state, ',')) {
        last = primary_expr(state);
        head = add_lang_ast_last(head, last);
    }

    return head;
}

/**
 * parameters
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *parameters(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;

    if (state->lex_lookahead == ')') {
        return NULL;
    }
    head = make_lang_ast_list1(var_identifier(state), state->source_name, state->source_line);

    while (lang_accept(state, ',')) {
        last = var_identifier(state);
        head = add_lang_ast_last(head, last);
    }

    return head;
}


/**
 * arg_list
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *arg_list(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *expr1 = NULL;

    expr1 = expression(state);
    if (expr1) {
        head = make_lang_ast_list1(expr1, state->source_name, state->source_line);
    }

    while (lang_accept(state, ',')) {
        last = expression(state);
        head = add_lang_ast_last(head, last);
    }

    return head;
}

/**
 * expr_key
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *expr_key(lang_state_t *state)
{
    lang_ast_t *expr1 = NULL;
    lang_ast_t *expr2 = NULL;

    expr1 = primary_expr(state);
    if (expr1) {
        lang_expect(state, ':');
        expr2 = primary_expr(state);
    }

    return make_lang_ast_list2(expr1, expr2, state->source_name, state->source_line);
}

/**
 * key_list
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *key_list(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *expr1 = NULL;

    expr1 = expr_key(state);
    head = make_lang_ast_list1(expr1, state->source_name, state->source_line);

    while (lang_accept(state, ',')) {
        last = expr_key(state);
        head = add_lang_ast_last(head, last);
    }

    return head;
}

/**
 * local_var
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *local_var(lang_state_t *state)
{
    lang_ast_t *var = NULL;
    lang_ast_t *symbol = NULL;
    lang_ast_t *expr1;
    lang_ast_t *expr2;

    if (lang_accept(state, TOKEN_VAR)) {
        symbol = identifier(state);

        if (lang_accept(state, TOKEN_lang)) {
            var = make_lang_ast(DECLARE_VAR_OP, symbol, NULL, state->source_name, state->source_line);
        } else if (lang_accept(state, '=')) {
            expr1 = expression(state);
            lang_expect(state, TOKEN_lang);
            return make_lang_ast(DECLARE_VAR_OP, symbol, expr1, state->source_name, state->source_line);
        } else if (lang_accept(state, '[')) {
            expr1 = expression(state);
            lang_expect(state, ']');
            if (lang_accept(state, TOKEN_lang)) {
                return make_lang_ast(DECLARE_ARRAY_OP, symbol, make_lang_ast_list2(expr1, NULL, state->source_name, state->source_line), state->source_name, state->source_line);
            } else if (lang_accept(state, '=')) {
                lang_expect(state, '[');
                expr2 = vals_list(state);
                lang_expect(state, ']');
                lang_expect(state, TOKEN_lang);
                return make_lang_ast(DECLARE_ARRAY_OP, symbol, make_lang_ast_list2(expr1, expr2, state->source_name, state->source_line), state->source_name, state->source_line);
            }
        } else if (lang_accept(state, '{')) {
            lang_expect(state, '}');
            if (lang_accept(state, TOKEN_lang)) {
                return make_lang_ast(DECLARE_TUPLE_OP, symbol, NULL, state->source_name, state->source_line);
            } else if (lang_accept(state, '=')) {
                lang_expect(state, '{');
                expr2 = vals_list(state);
                lang_expect(state, '}');
                lang_expect(state, TOKEN_lang);
                return make_lang_ast(DECLARE_TUPLE_OP, symbol, expr2, state->source_name, state->source_line);
            }
        } else if (lang_accept(state, '<')) {
            lang_expect(state, '>');
            if (lang_accept(state, TOKEN_lang)) {
                return make_lang_ast(DECLARE_HASH_OP, symbol, NULL, state->source_name, state->source_line);
            } else if (lang_accept(state, '=')) {
                lang_expect(state, '<');
                expr2 = key_list(state);
                lang_expect(state, '>');
                lang_expect(state, TOKEN_lang);
                return make_lang_ast(DECLARE_HASH_OP, symbol, expr2, state->source_name, state->source_line);
            }
        }
    } else {
        return NULL;
    }

    return var;
}

/**
 * local_vars
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *local_vars(lang_state_t *state)
{
    lang_ast_t *head;
    lang_ast_t *last;

    head = make_lang_ast_list1(local_var(state), state->source_name, state->source_line);

    while (true) {
        last = local_var(state);
        if (last == NULL) {
            break;
        }
        head = add_lang_ast_last(head, last);
    }

    return head;
}

/**
 * primary_expr
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *primary_expr(lang_state_t *state)
{
    lang_ast_t *ast = NULL;

    if (lang_accept(state, TOKEN_ID)) {
        ast = make_lang_ast_symbol(state->value.string, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_NULL)) {
        ast = make_lang_ast_number(0x7ff0000000000000|0x0008000000000000|0x0003000000000000, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_NUM)) {
        ast = make_lang_ast_number(state->value.i64, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_U_NUM)) {
        ast = make_lang_ast_u_number(state->value.u64, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_TRUE)) {
        ast = make_lang_ast_bool(STD_BOOL_TRUE, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_FALSE)) {
        ast = make_lang_ast_bool(STD_BOOL_FALSE, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_ADDRESS)) {
        ast = make_lang_ast_address(state->value.address, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_STR)) {
        ast = make_lang_ast_string(state->value.string, state->source_name, state->source_line);
    } else if (lang_accept(state, '(')) {
        ast = expression(state);
        lang_expect(state, ')');
    }

    return ast;
}

/**
 * call_expr
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *call_expr(lang_state_t *state)
{
    lang_ast_t *expr1 = primary_expr(state);
    lang_ast_t *expr2 = NULL;
    lang_ast_t *expr3 = NULL;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '(')) {
            expr2 = arg_list(state);
            lang_expect(state, ')');
            ret = make_lang_ast(CALL_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '[')) {
            expr2 = expression(state);
            lang_expect(state, ']');

            if (lang_accept(state, '=')) {
                expr3 = expression(state);
                ret = make_lang_ast(SET_ARRAY_OP, make_lang_ast_list2(ret, expr2, state->source_name, state->source_line), expr3, state->source_name, state->source_line);
            } else {
                ret = make_lang_ast(GET_ARRAY_OP, ret, expr2, state->source_name, state->source_line);
            }
        } else if (lang_accept(state, '{')) {
            expr2 = expression(state);
            lang_expect(state, '}');
            ret = make_lang_ast(GET_TUPLE_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '.')) {
            if (lang_accept(state, TOKEN_ADD_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(ADD_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_ADD_KEY_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(ADD_KEY_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_DEL_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(DEL_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_FIND_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(FIND_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_COUNT_ITEM)) {
                lang_expect(state, '(');
                lang_expect(state, ')');
                ret = make_lang_ast(COUNT_TUPLE_OP, ret, NULL, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_GET)) {
                lang_expect(state, '(');
                lang_expect(state, ')');
                ret = make_lang_ast(SAFE_GET_OP, ret, NULL, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_SET)) {
                lang_expect(state, '(');
                expr2 = expression(state);
                lang_expect(state, ')');
                ret = make_lang_ast(SAFE_SET_OP, ret, expr2, state->source_name, state->source_line);
            }
        } else {
            break;
        }
    }
    return ret;
}

/**
 * multiplicative
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *multiplicative(lang_state_t *state)
{
    lang_ast_t *expr1 = call_expr(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '*')) {
            expr2 = call_expr(state);
            ret = make_lang_ast(MUL_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '/')) {
            expr2 = call_expr(state);
            ret = make_lang_ast(DIV_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '%')) {
            expr2 = call_expr(state);
            ret = make_lang_ast(MOD_OP, ret, expr2, state->source_name, state->source_line);
        } else {
            break;
        }
    }

    return ret;
}

/**
 * additive
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *additive(lang_state_t *state)
{
    lang_ast_t *expr1 = multiplicative(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '+')) {
            expr2 = multiplicative(state);
            ret = make_lang_ast(PLUS_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '-')) {
            expr2 = multiplicative(state);
            ret = make_lang_ast(MINUS_OP, ret, expr2, state->source_name, state->source_line);
        } else {
            break;
        }
    }
    return ret;
}

/**
 * relational
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *relational(lang_state_t *state)
{
    lang_ast_t *expr1 = additive(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '<')) {
            expr2 = additive(state);
            ret = make_lang_ast(LT_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '>')) {
            expr2 = additive(state);
            ret = make_lang_ast(GT_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, TOKEN_LGE)) {
            expr2 = additive(state);
            ret = make_lang_ast(LGE_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, TOKEN_BGE)) {
            expr2 = additive(state);
            ret = make_lang_ast(BGE_OP, ret, expr2, state->source_name, state->source_line);
        } else {
            break;
        }
    }
    return ret;
}

/**
 * equality
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *equality(lang_state_t *state)
{
    lang_ast_t *expr1 = relational(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, TOKEN_EQ)) {
            expr2 = relational(state);
            ret = make_lang_ast(EQUAL_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, TOKEN_NEQ)) {
            expr2 = relational(state);
            ret = make_lang_ast(NON_EQUAL_OP, ret, expr2, state->source_name, state->source_line);
        } else {
            break;
        }
    }
    return ret;
}


/**
 * logic_and
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *logic_and(lang_state_t *state)
{
    lang_ast_t *expr1 = equality(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    if (lang_accept(state, TOKEN_AND)) {
        expr2 = logic_and(state);
        ret = make_lang_ast(AND_OP, ret, expr2, state->source_name, state->source_line);
    }
    return ret;
}

/**
 * logic_or
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *logic_or(lang_state_t *state)
{
    lang_ast_t *expr1 = logic_and(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    if (lang_accept(state, TOKEN_OR)) {
        expr2 = logic_or(state);
        ret = make_lang_ast(OR_OP, expr1, expr2, state->source_name, state->source_line);
    }
    return ret;
}

/**
 * assignment
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *assignment(lang_state_t *state)
{
    lang_ast_t *symbol = logic_or(state);
    lang_ast_t *value;
    lang_ast_t *ret = symbol;

    if (lang_accept(state, '=')) {
        value = assignment(state);
        ret = make_lang_ast(EQ_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_MUL)) {
        value = assignment(state);
        ret = make_lang_ast(Inp_MUL_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_DIV)) {
        value = assignment(state);
        ret = make_lang_ast(Inp_DIV_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_MOD)) {
        value = assignment(state);
        ret = make_lang_ast(Inp_MOD_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_ADD)) {
        value = assignment(state);
        ret = make_lang_ast(Inp_PLUS_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_SUB)) {
        value = assignment(state);
        ret = make_lang_ast(Inp_MINUS_OP, symbol, value, state->source_name, state->source_line);
    }

    return ret;
}

/**
 * expression
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *expression(lang_state_t *state)
{
    lang_ast_t *assn = assignment(state);

    return assn;
}

/**
 * statement
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *statement(lang_state_t *state)
{
    lang_ast_t *stm = NULL;
    lang_ast_t *blk;
    lang_ast_t *exp;
    lang_ast_t *stm_else;
    lang_ast_t *exp2;
    lang_ast_t *exp3;

    if (lang_accept(state, TOKEN_lang)) {
        return NULL;
    } else if (state->lex_lookahead == '{') {
        blk = block(state, STD_BOOL_FALSE);
        return blk;
    } else if ((stm = expression(state))) {
        lang_expect(state, TOKEN_lang);
        return stm;
    } else if (lang_accept(state, TOKEN_IF)) {
        lang_expect(state, '(');
        exp = expression(state);
        lang_expect(state, ')');

        stm = block(state, STD_BOOL_TRUE);
        if (lang_accept(state, TOKEN_ELSE)) {
            stm_else = block(state, STD_BOOL_FALSE);
            return make_lang_ast(IF_STATEMENT, exp, make_lang_ast_list2(stm, stm_else, state->source_name, state->source_line), state->source_name, state->source_line);
        } else {
            return make_lang_ast(IF_STATEMENT, exp, make_lang_ast_list2(stm, NULL, state->source_name, state->source_line), state->source_name, state->source_line);
        }

    } else if (lang_accept(state, TOKEN_RETURN)) {
        exp = expression(state);
        lang_expect(state, TOKEN_lang);
        return make_lang_ast(RETURN_STATEMENT, exp, NULL, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_BREAK)) {
        lang_expect(state, TOKEN_lang);
        return make_lang_ast(BREAK_STATEMENT, NULL, NULL, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_WHILE)) {
        lang_expect(state, '(');
        exp = expression(state);
        lang_expect(state, ')');

        stm = statement(state);
        return make_lang_ast(WHILE_STATEMENT, exp, stm, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_FOR)) {
        lang_expect(state, '(');
        exp = expression(state);
        lang_expect(state, ',');
        exp2 = expression(state);
        lang_expect(state, ',');
        exp3 = expression(state);
        lang_expect(state, ')');
        stm = statement(state);
        return make_lang_ast(FOR_STATEMENT, make_lang_ast_list3(exp, exp2, exp3, state->source_name, state->source_line), stm, state->source_name, state->source_line);
    }
    return NULL;
}

/**
 * statements
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *statements(lang_state_t *state)
{
    lang_ast_t *head;
    lang_ast_t *last;

    head = make_lang_ast_list1(statement(state), state->source_name, state->source_line);

    while (true) {
        last = statement(state);
        head = add_lang_ast_last(head, last);
        if (state->lex_lookahead == '}') {
            break;
        }
    }

    return head;
}

/**
 * block
 * @brief   
 * @param   state
 * @param   nl_skip
 * @return  lang_ast_t *
 */
lang_ast_t *block(lang_state_t *state, std_bool_t nl_skip)
{
    lang_ast_t *block = NULL;
    lang_ast_t *locals;
    lang_ast_t *stms = NULL;

    lang_expect(state, '{');
    lang_expect(state, TOKEN_lang);

    locals = local_vars(state);

    stms = statements(state);

    lang_expect(state, '}');
    if (!nl_skip) {
        lang_expect(state, TOKEN_lang);
    }

    block = make_lang_ast(BLOCK_STATEMENT, locals, stms, state->source_name, state->source_line);
    return block;
}


extern std_char_t *function_name;
/**
 * def_function
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *def_function(lang_state_t *state)
{
    lang_ast_t *func_symbol;
    lang_ast_t *func_parameter;
    lang_ast_t *func_body;

    func_symbol = identifier(state);

    function_name = func_symbol->symbol->name;

    lang_expect(state, '(');
    func_parameter = parameters(state);
    lang_expect(state, ')');
    lang_expect(state, TOKEN_lang);

    func_body = block(state, STD_BOOL_FALSE);

    mod_lang_compile_func(p_global_mod_compile, func_symbol, func_parameter, func_body);

    function_name = NULL;
    return func_symbol;
}

/**
 * global_definition
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *global_definition(lang_state_t *state)
{
    lang_ast_t *ret = NULL;
    lang_ast_t *cmd;

    while (true) {
        if (!lang_accept(state, TOKEN_lang)) {
            break;
        }
    }

    if (lang_accept(state, TOKEN_DEF)) {
        ret = def_function(state);
        lang_next(state);
        return ret;
    } else if (lang_accept(state, TOKEN_LOAD_LIB)) {
        lang_expect(state, TOKEN_STR);
        ret = make_lang_ast_string(state->value.string, state->source_name, state->source_line);
        lang_ast_t *expr = make_lang_ast(LOAD_LIB_OP, ret, NULL, state->source_name, state->source_line);
        mod_lang_compile_expr(p_global_mod_compile, expr);
        return ret;
    } else {
        cmd = command_statements(state);
        mod_lang_compile_cmd(p_global_mod_compile, cmd);
    }

    return NULL;
}

/**
 * global_definitions
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *global_definitions(lang_state_t *state)
{
    const lang_ast_t *var = NULL;

    while (true) {
        var = global_definition(state);
        if (var == NULL) {
            break;
        }
    }

    return NULL;
}

/**
 * program
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *program(lang_state_t *state)
{
    return global_definitions(state);
}

/**
 * command_statements
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *command_statements(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *cmd = NULL;

    cmd = command_statement(state);
    if (cmd) {
        head = make_lang_ast_list1(cmd, state->source_name, state->source_line);
    }

    while (true) {
        last = command_statement(state);
        if (last == NULL) {
            break;
        }
        head = add_lang_ast_last(head, last);
    }

    return head;
}

/**
 * command_statement
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *command_statement(lang_state_t *state)
{
    lang_ast_t *ast_func;

    if (lang_accept(state, TOKEN_lang)) {
        return NULL;
    } else {
        ast_func = call_expr(state);
        if (ast_func) {
            lang_expect(state, TOKEN_lang);
        }

        return ast_func;
    }
}

/**
 * handle_program
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *handle_program(lang_state_t *state)
{
    return program(state);
}


/**
 * cleanup_lang_ast_symbol
 * @brief   
 * @return  std_void_t
 */
std_void_t cleanup_lang_ast_symbol()
{
    std_lock_free_key_hash_value_callback_destroy(global_symbol_hash, &callback_destroy_lang_symbol, NULL);

    if (parse_no_error == STD_BOOL_FALSE) {
        std_lock_free_key_hash_value_callback_destroy(global_parse_error_need_clean_lang_ast_hash, &callback_destroy_lang_ast, NULL);
    } else {
        std_lock_free_key_hash_destroy(global_parse_error_need_clean_lang_ast_hash);
    }

    global_symbol_hash = NULL;
    global_parse_error_need_clean_lang_ast_hash = NULL;
    parse_no_error = STD_BOOL_FALSE;
}

/**
 * lang_parse
 * @brief   
 * @param   state
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  lang_ast_t *
 */
lang_ast_t *lang_parse(lang_state_t *state, IN std_char_t *file_name, IN std_char_t *source_buffer, IN std_int_t source_buffer_len)
{
    std_int_t jmp_ret;
    lang_ast_t *ret = NULL;

    lang_lex_init(state, file_name, source_buffer, source_buffer_len);

    jmp_ret = setjmp(state->error_jump_buf);
    if (jmp_ret) {
        lang_lex_cleanup(state);
        function_name = NULL;
        cleanup_lang_ast_symbol();
        return NULL;
    }

    lang_next(state);
    ret = handle_program(state);
    lang_lex_cleanup(state);

    cleanup_lang_ast_symbol();
    return ret;
}