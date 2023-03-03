def mod_lang_vm_run_init(var root, var file_name, var bytecode_buffer, var ret)
{
    var name_value_hash<> = <"file_name":file_name,"bytecode_buffer":bytecode_buffer>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_vm_run_init", root, json_args)
}


def mod_lang_vm_run_execute(var root, var file_name, var u64_key, var ret)
{
    var name_value_hash<> = <"file_name":file_name,"u64_key":u64_key>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_vm_run_execute", root, json_args)
}


def mod_lang_vm_run_cleanup(var root, var file_name, var ret)
{
    var name_value_hash<> = <"file_name":file_name>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_vm_run_cleanup", root, json_args)
}


def run_run_init(var root, var run_state)
{
    var file_name
    var bytecode_buffer
    var ret_run_init
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_vm_run_init(root,file_name,bytecode_buffer,ret_run_init)
}

def run_run_execute(var root, var run_state)
{
    var file_name
    var u64_key
    var ret_run_execute
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_vm_run_execute(root,file_name,u64_key,ret_run_execute)
}

def run_run_cleanup(var root, var run_state)
{
    var file_name
    var ret_run_cleanup
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_vm_run_cleanup(root,file_name,ret_run_cleanup)
}


def main()
{
    var mod_lang_vm_test
    var iid = "0xa7e515a2, 0xdb52, 0xdf54, 0x5f, 0x9f, 0xa9, 0x9a, 0x6a, 0x8d, 0xf4, 0xcb"
    var args = "{}"
    var keys_tuple{} = {}
    var hash_key_value<> = <>
	var run_state<> = <"keys_tuple": keys_tuple, "hash_key_value":hash_key_value>
    var i
    var run_max = 1000

    debug("ERR")

    mod_lang_vm_test = create_instance(iid, args)
    print("mod_lang_vm_test:", mod_lang_vm_test)

    run_run_init(mod_lang_vm_test, run_state)

    run_run_execute(mod_lang_vm_test, run_state)

    run_run_cleanup(mod_lang_vm_test, run_state)


    for (i = 0, i < run_max, i += 1) {
        var k
        var run_random

        run_random = random_number(32)
        k = run_random % 3

        if ( k == 0) {
            run_run_init(mod_lang_vm_test, run_state)
        }

        if ( k == 1) {
            run_run_execute(mod_lang_vm_test, run_state)
        }

        if ( k == 2) {
            run_run_cleanup(mod_lang_vm_test, run_state)
        }

    }

    delete_instance(iid, mod_lang_vm_test)
}
