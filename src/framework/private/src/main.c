/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    main.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#include "mod.h"
#include "mod_bundle.h"
#include "mod_lang_compile.h"
#include "mod_lang_parse.h"
#include "mod_lang_vm.h"
#include "mod_shell.h"
#include "std_common.h"

/**
 * main
 * @brief   
 * @return  std_int_t
 */
STD_CALL std_int_t main()
{
    std_uint_t bundle_id = 0;
    mod_lang_parse_t *mod_lang_parse;
    mod_lang_compile_t *mod_lang_compile;
    mod_lang_vm_t *mod_lang_vm;
    mod_shell_t *mod_shell = NULL;
    mod_ownership_t main_ownership;
    mod_iid_t mod_lang_parse_iid = MOD_LANG_PARSE_IID;
    mod_iid_t mod_lang_compile_iid = MOD_LANG_COMPILE_IID;
    mod_iid_t mod_lang_vm_iid = MOD_LANG_VM_IID;
    
    mod_iid_t mod_shell_iid = MOD_SHELL_IID;

    STD_LOG_INIT(ERR);

    mod_bundle_init();

    mod_ownership_register_ops(&main_ownership);
    mod_ownership_init(&main_ownership);

    STD_ASSERT_RV(mod_bundle_cmd_install("mod_lang_compile_I", strlen("mod_lang_compile_I"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);
    mod_create_instance(&mod_lang_compile_iid, (std_void_t **) &mod_lang_compile, &main_ownership);
    mod_lang_compile_init(mod_lang_compile, NULL, 0);

    STD_ASSERT_RV(mod_bundle_cmd_install("mod_lang_parse_I", strlen("mod_lang_parse_I"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);
    mod_create_instance(&mod_lang_parse_iid, (std_void_t **) &mod_lang_parse, &main_ownership);
    mod_lang_parse_init(mod_lang_parse, NULL, 0);

    STD_ASSERT_RV(mod_bundle_cmd_install("mod_lang_vm_I", strlen("mod_lang_vm_I"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);
    mod_create_instance(&mod_lang_vm_iid, (std_void_t **) &mod_lang_vm, &main_ownership);
    mod_lang_vm_init(mod_lang_vm, NULL, 0);

    STD_ASSERT_RV(mod_bundle_cmd_install("mod_shell_I", strlen("mod_shell_I"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);
    mod_create_instance(&mod_shell_iid, (std_void_t **) &mod_shell, &main_ownership);
    mod_shell_init(mod_shell, NULL, 0);

    mod_shell_shell(mod_shell);

    mod_lang_compile_cleanup(mod_lang_compile);
    mod_delete_instance(&mod_lang_compile_iid, (std_void_t **) &mod_lang_compile, &main_ownership);

    mod_lang_parse_cleanup(mod_lang_parse);
    mod_delete_instance(&mod_lang_parse_iid, (std_void_t **) &mod_lang_parse, &main_ownership);

    mod_lang_vm_cleanup(mod_lang_vm);
    mod_delete_instance(&mod_lang_vm_iid, (std_void_t **) &mod_lang_vm, &main_ownership);

    mod_shell_cleanup(mod_shell);
    mod_delete_instance(&mod_shell_iid, (std_void_t **) &mod_shell, &main_ownership);

    for (std_uint_t i = bundle_id ; i >= 1; i--) {
        STD_ASSERT_RV(mod_bundle_cmd_stop(i) == STD_RV_SUC, STD_RV_ERR_FAIL);
        STD_ASSERT_RV(mod_bundle_cmd_uninstall(i) == STD_RV_SUC, STD_RV_ERR_FAIL);
    }

    mod_bundle_cleanup();

    STD_LOG(INFO, "exit main\n");

    return STD_RV_SUC;
}
