/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_bundle.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#include "mod_bundle_imp.h"
#include "mod_imp.h"
#include "std_lock_free_key_hash.h"
#include "std_lock_free_list.h"
#include "tiny-json.h"
#include <dlfcn.h>


static std_lock_free_key_hash_t *gp_mod_bundle_id_hash = NULL;
static std_lock_free_list_head_t gp_mod_bundle_service_head;
static std_int_t global_id = 1;

std_rv_t mod_bundle_service_check(IN const std_char_t *bundle_name, IN std_int_t major, IN std_int_t minor, IN std_int_t patch);


/**
 * mod_bundle_init
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_init()
{
    mod_init();
    gp_mod_bundle_id_hash = std_lock_free_key_hash_create(128);
    std_lock_free_list_init(&gp_mod_bundle_service_head,
                            std_lock_free_list_head_entry_offset(mod_srv_t, list),
                            STD_BOOL_TRUE,
                            STD_BOOL_TRUE);

    return STD_RV_SUC;
}

/**
 * mod_bundle_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cleanup()
{
    mod_cleanup();
    std_lock_free_key_hash_destroy(gp_mod_bundle_id_hash);
    std_lock_free_list_cleanup(&gp_mod_bundle_service_head);

    return STD_RV_SUC;
}

/**
 * parse_mod_desc
 * @brief   
 * @param   srv
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t parse_mod_desc(IN mod_srv_t *srv)
{
    json_t const *json = NULL;
    json_t const *json_param;
    const std_char_t *name = NULL;
    const std_char_t *instance = NULL;
    const std_char_t *version = NULL;
    json_t mem[32];
    std_char_t *desc = strdup(srv->desc);

    json = json_create(desc, mem, sizeof mem / sizeof *mem);

    STD_ASSERT_RV_ACTION(json != NULL, STD_RV_ERR_FAIL, FREE(desc););

    json_param = json_getProperty(json, "name");
    name = json_getValue(json_param);
    snprintf(srv->name, sizeof(srv->name), "%s", name);

    json_param = json_getProperty(json, "instance");
    instance = json_getValue(json_param);
    snprintf(srv->impl_name, sizeof(srv->impl_name), "%s", instance);

    json_param = json_getProperty(json, "version");
    version = json_getValue(json_param);
    sscanf(version, "%d.%d.%d",
           &(srv->ver.major),
           &(srv->ver.minor),
           &(srv->ver.patch));

    json_param = json_getProperty(json, "dependence");
    for (json_t const *dep = json_getChild(json_param); dep != 0; dep = json_getSibling(dep)) {
        if (JSON_TEXT == json_getType(dep)) {
            const std_char_t *item_name = json_getName(dep);
            const std_char_t *item_version = json_getValue(dep);

            std_int_t major;
            std_int_t minor;
            std_int_t patch;
            sscanf(item_version, "%d.%d.%d", &major, &minor, &patch);

            if (mod_bundle_service_check(item_name, major, minor, patch) != STD_RV_SUC) {
                STD_LOG(ERR, "--%s-- dependence not satisfied \n", item_name);
                FREE(desc);
                return STD_RV_ERR_FAIL;
            }
        }
    }

    FREE(desc);
    return STD_RV_SUC;
}
/**
 * mod_bundle_cmd_install
 * @brief   
 * @param   bundle_name
 * @param   p_bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_install(IN const std_char_t *bundle_name, IN std_int_t bundle_name_len, INOUT std_uint_t *p_bundle_id)
{
    std_char_t key[BUF_SIZE_128] = "\0";
    std_char_t file[BUF_SIZE_128] = "\0";
    FILE *fp = NULL;
    mod_srv_t *srv = NULL;

    STD_ASSERT_RV(bundle_name != NULL, STD_RV_ERR_FAIL);
    srv = (mod_srv_t *) CALLOC(1, sizeof(struct mod_srv_s));
    STD_ASSERT_RV(srv != NULL, STD_RV_ERR_FAIL);

    snprintf(srv->name, sizeof(srv->name), "%s", bundle_name);

    snprintf(file, sizeof(file), "bundle/%s/desc.json", bundle_name);
    STD_ASSERT_RV_ACTION((fp = fopen(file, "r")) != NULL, STD_RV_ERR_FAIL, FREE(srv));

    srv->desc = (std_char_t *) CALLOC(1, BUF_SIZE_4096);
    srv->desc_size = BUF_SIZE_4096;

    STD_ASSERT_RV_ACTION(fread(srv->desc, 1, BUF_SIZE_4096, fp) > 0, STD_RV_ERR_FAIL,
                         FREE(srv->desc);
                         FREE(srv);
                         fclose(fp););
    fclose(fp);

    STD_ASSERT_RV_ACTION(parse_mod_desc(srv) == STD_RV_SUC, STD_RV_ERR_FAIL,
                         FREE(srv->desc);
                         FREE(srv););


    *p_bundle_id = srv->id = global_id++;

    snprintf(file, sizeof(file), "bundle/%s/lib%s.so", bundle_name, bundle_name);

    srv->dl_handle = dlopen(file, RTLD_LAZY | RTLD_LOCAL);

    STD_ASSERT_RV_ACTION(srv->dl_handle != NULL, STD_RV_ERR_FAIL,
                         FREE(srv->desc);
                         FREE(srv););

    srv->create = dlsym(srv->dl_handle, "bundle_create");
    srv->start = dlsym(srv->dl_handle, "bundle_start");
    srv->stop = dlsym(srv->dl_handle, "bundle_stop");
    srv->destroy = dlsym(srv->dl_handle, "bundle_destroy");

    STD_ASSERT_RV_ACTION(srv->create != NULL, STD_RV_ERR_FAIL, FREE(srv->desc); FREE(srv));
    STD_ASSERT_RV_ACTION(srv->start != NULL, STD_RV_ERR_FAIL, FREE(srv->desc); FREE(srv));
    STD_ASSERT_RV_ACTION(srv->stop != NULL, STD_RV_ERR_FAIL, FREE(srv->desc); FREE(srv));
    STD_ASSERT_RV_ACTION(srv->destroy != NULL, STD_RV_ERR_FAIL, FREE(srv->desc); FREE(srv));

    STD_ASSERT_RV_ACTION(srv->create(srv->id) == STD_RV_SUC, STD_RV_ERR_FAIL, FREE(srv->desc); FREE(srv));

    srv->state = BUNDLE_INSTALLED;

    snprintf(key, sizeof(key), "%d", srv->id);
    std_lock_free_key_hash_add(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128), srv);
    std_lock_free_list_add(&gp_mod_bundle_service_head, &srv->list, (std_u64_t) &srv->list);

    return STD_RV_SUC;
}

/**
 * mod_bundle_cmd_uninstall
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_uninstall(std_uint_t bundle_id)
{
    std_char_t key[BUF_SIZE_128] = "\0";
    mod_srv_t *srv = NULL;

    snprintf(key, sizeof(key), "%d", bundle_id);

    srv = std_lock_free_key_hash_find(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128));
    STD_ASSERT_RV(srv != NULL, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(srv->state == BUNDLE_INSTALLED, STD_RV_ERR_FAIL);

    std_lock_free_key_hash_del(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128));

    srv->destroy();

#if 0
    dlclose(srv->dl_handle);
#endif

    FREE(srv->desc);

    std_lock_free_list_del(&gp_mod_bundle_service_head, (std_u64_t) &srv->list);

    return STD_RV_SUC;
}

/**
 * mod_bundle_cmd_start
 * @brief   
 * @param   bundle_id
 * @param   arg
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_start(IN const std_uint_t bundle_id, IN const std_char_t *arg, IN const std_int_t arg_len)
{
    std_char_t key[BUF_SIZE_128] = "\0";
    mod_srv_t *srv = NULL;

    snprintf(key, sizeof(key), "%d", bundle_id);

    srv = std_lock_free_key_hash_find(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128));

    STD_ASSERT_RV(srv != NULL, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(srv->state == BUNDLE_INSTALLED, STD_RV_ERR_FAIL);

    srv->state = BUNDLE_STARTING;
    STD_ASSERT_RV_ACTION(srv->start(arg, arg_len) == STD_RV_SUC, STD_RV_ERR_FAIL, srv->state = BUNDLE_START_FAILED;);
    srv->state = BUNDLE_ACTIVE;

    return STD_RV_SUC;
}

/**
 * mod_bundle_cmd_stop
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_cmd_stop(IN const std_uint_t bundle_id)
{
    std_char_t key[BUF_SIZE_128] = "\0";
    mod_srv_t *srv = NULL;

    snprintf(key, sizeof(key), "%d", bundle_id);

    srv = (mod_srv_t *) std_lock_free_key_hash_find(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128));

    STD_ASSERT_RV(srv != NULL, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(srv->state == BUNDLE_ACTIVE, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_get_instance_counts(srv->p_iid) == 0, STD_RV_ERR_FAIL);

    srv->state = BUNDLE_STOPPING;
    srv->stop();
    srv->state = BUNDLE_INSTALLED;

    return STD_RV_SUC;
}

/**
 * mod_bundle_get_desc
 * @brief   
 * @param   bundle_id
 * @param   desc
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_get_desc(IN const std_uint_t bundle_id, INOUT std_char_t **desc)
{
    std_char_t key[BUF_SIZE_128] = "\0";
    mod_srv_t *srv = NULL;

    snprintf(key, sizeof(key), "%d", bundle_id);

    srv = (mod_srv_t *) std_lock_free_key_hash_find(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128));

    STD_ASSERT_RV(srv != NULL, STD_RV_ERR_FAIL);

    *desc = (std_char_t *) CALLOC(1, srv->desc_size);
    snprintf(*desc, srv->desc_size, "%s", srv->desc);

    return STD_RV_SUC;
}


/**
 * bundle_trans_state
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_char_t *
 */
STD_CALL static inline std_char_t *bundle_trans_state(mod_bundle_state_t state)
{
    switch (state) {
        case BUNDLE_INSTALLED:
            return "installed";
        case BUNDLE_ACTIVE:
            return "active";
        case BUNDLE_STARTING:
            return "starting";
        case BUNDLE_STOPPING:
            return "stopping";
        case BUNDLE_START_FAILED:
            return "start_failed";
        default:
            break;
    }

    return "";
}

typedef std_void_t (*callback_func_t)(std_void_t *data);
/**
 * mod_bundle_walk
 * @brief   
 * @param   callback_func
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_walk(IN callback_func_t callback_func)
{
    mod_srv_t *srv = NULL;

    STD_LOG(DISPLAY, "%4s %-40s %-40s %-40s\n", "ID", "MODULE_NAME", "MODULE_IMPLEMENTATION_NAME", "STATE");

    for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(gp_mod_bundle_service_head.next);
         pos != NULL;) {
        srv = std_lock_free_list_head_entry(pos, mod_srv_t, list);
        pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);
        if (callback_func) {
            callback_func(NULL);
        }
        if (strcmp(srv->impl_name, "mod_vm_ADM5120") == 0){
            STD_LOG(DISPLAY, "%4d %-40s %-40s %-40s\n", srv->id, srv->name, "mod_vm_MIPS", bundle_trans_state(srv->state));
        }else {
            STD_LOG(DISPLAY, "%4d %-40s %-40s %-40s\n", srv->id, srv->name, srv->impl_name, bundle_trans_state(srv->state));
        }

    }

    return STD_RV_SUC;
}

/**
 * mod_bundle_IID_register
 * @brief   
 * @param   bundle_id
 * @param   p_iid
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_IID_register(IN std_uint_t bundle_id, IN const mod_iid_t *p_iid)
{
    std_char_t key[BUF_SIZE_128] = "\0";
    mod_srv_t *srv = NULL;

    snprintf(key, sizeof(key), "%d", bundle_id);

    srv = std_lock_free_key_hash_find(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128));
    STD_ASSERT_RV(srv != NULL, STD_RV_ERR_FAIL);

    srv->p_iid = p_iid;

    return STD_RV_SUC;
}

/**
 * mod_bundle_IID_unregister
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_IID_unregister(std_uint_t bundle_id)
{
    std_char_t key[BUF_SIZE_128] = "\0";
    mod_srv_t *srv = NULL;

    snprintf(key, sizeof(key), "%d", bundle_id);

    srv = std_lock_free_key_hash_find(gp_mod_bundle_id_hash, key, std_safe_strlen(key, BUF_SIZE_128));
    STD_ASSERT_RV(srv != NULL, STD_RV_ERR_FAIL);

    srv->p_iid = NULL;

    return STD_RV_SUC;
}

/**
 * mod_bundle_service_check
 * @brief   
 * @param   bundle_name
 * @param   major
 * @param   minor
 * @param   patch
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_bundle_service_check(IN const std_char_t *bundle_name, IN std_int_t major, IN std_int_t minor, IN std_int_t patch)
{
    const mod_srv_t *srv = NULL;

    for (std_lock_free_list_head_t *pos = STD_LOCK_FREE_LIST_STRIP_MARK(gp_mod_bundle_service_head.next);
         pos != NULL;) {
        srv = std_lock_free_list_head_entry(pos, mod_srv_t, list);
        pos = STD_LOCK_FREE_LIST_STRIP_MARK(pos->next);

        if (strncmp(srv->name, bundle_name, std_safe_strlen(bundle_name, BUF_SIZE_128)) == 0 && std_safe_strlen(srv->name, sizeof(srv->name)) == std_safe_strlen(bundle_name, BUF_SIZE_128)) {
            STD_ASSERT_RV(srv->state == BUNDLE_ACTIVE, STD_RV_ERR_FAIL);
            STD_ASSERT_RV(major * 1000 + minor * 100 + patch <= srv->ver.major * 1000 + srv->ver.minor * 100 + srv->ver.patch, STD_RV_ERR_FAIL);
            return STD_RV_SUC;
        }
    }
    return STD_RV_ERR_POINTER;
}
