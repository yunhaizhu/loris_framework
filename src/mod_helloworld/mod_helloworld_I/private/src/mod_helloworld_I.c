/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_helloworld_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-03-07
 *
 */

#include "mod_helloworld_I.h"

/**
 * mod_helloworld_I_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_helloworld_I_init(IN mod_helloworld_t * p_m, IN const std_char_t * arg, IN std_int_t arg_len)
{
	mod_helloworld_imp_t *p_imp_m = (mod_helloworld_imp_t *) p_m;
	mod_iid_t mod_shell_iid = MOD_SHELL_IID;
	std_char_t key[BUF_SIZE_128] = "\0";

	mod_create_instance(&mod_shell_iid, (std_void_t **) & p_imp_m->p_mod_shell, (mod_ownership_t *) p_imp_m);

	mod_shell_init(p_imp_m->p_mod_shell, NULL, 0);

    /****shell_register******/

	snprintf(key, sizeof(key), "%lul-%s", p_imp_m->unique_id, "mod_helloworld_say_hello");
	mod_shell_register(p_imp_m->p_mod_shell, key, std_safe_strlen(key, sizeof(key)),
			   shell_stub_mod_helloworld_I_say_hello, p_imp_m);

	return STD_RV_SUC;
}

/**
 * mod_helloworld_I_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_helloworld_I_cleanup(mod_helloworld_t * p_m)
{
	mod_helloworld_imp_t *p_imp_m = (mod_helloworld_imp_t *) p_m;
	mod_iid_t mod_shell_iid = MOD_SHELL_IID;
	std_char_t key[BUF_SIZE_128] = "\0";

    /****shell_unregister******/

	snprintf(key, sizeof(key), "%lul-%s", p_imp_m->unique_id, "mod_helloworld_say_hello");
	mod_shell_unregister(p_imp_m->p_mod_shell, key, std_safe_strlen(key, sizeof(key)));

	mod_shell_cleanup(p_imp_m->p_mod_shell);

	mod_delete_instance(&mod_shell_iid, (std_void_t **) & p_imp_m->p_mod_shell, (mod_ownership_t *) p_imp_m);

	return STD_RV_SUC;
}

/***func_implementation***/

/**
 * mod_helloworld_I_say_hello
 * @brief   
 * @param   p_m
 * @param   str
 * @return  STD_CALL             std_int_t
 */
STD_CALL std_int_t mod_helloworld_I_say_hello(IN mod_helloworld_t * p_m, IN std_char_t * str)
{
    STD_LOG(DISPLAY, "%s %s\n", __FUNCTION__ , str);
	return 0;

}

struct mod_helloworld_ops_st mod_helloworld_I_ops = {
	mod_helloworld_I_init,
	mod_helloworld_I_cleanup,

    /***func_ops***/
	mod_helloworld_I_say_hello,

};

/**
 * mod_helloworld_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_helloworld_I_create_instance(INOUT std_void_t ** pp_handle)
{
	mod_helloworld_imp_t *p_m = NULL;

	p_m = (mod_helloworld_imp_t *) CALLOC(1, sizeof(mod_helloworld_imp_t));
	p_m->unique_id = std_random_u64();
	p_m->p_ops = &mod_helloworld_I_ops;

	mod_ownership_register_ops((mod_ownership_t *) p_m);
	mod_ownership_init((mod_ownership_t *) p_m);
	*pp_handle = p_m;

	return STD_RV_SUC;
}
