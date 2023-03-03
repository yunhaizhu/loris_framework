def mod_lang_compile_func(var root, var func_id, var func_params, var func_body, var ret)
{
    var name_value_hash<> = <"func_id":func_id,"func_params":func_params,"func_body":func_body>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_compile_func", root, json_args)
}


def mod_lang_compile_cmd(var root, var cmd, var ret)
{
    var name_value_hash<> = <"cmd":cmd>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_compile_cmd", root, json_args)
}


def mod_lang_compile_expr(var root, var cmd, var ret)
{
    var name_value_hash<> = <"cmd":cmd>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_compile_expr", root, json_args)
}


def mod_lang_compile_generated_bytecode(var root, var file_name, var ret)
{
    var name_value_hash<> = <"file_name":file_name>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_compile_generated_bytecode", root, json_args)
}


def run_func(var root, var run_state)
{
    var func_id
    var func_params
    var func_body
    var ret_func
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_compile_func(root,func_id,func_params,func_body,ret_func)
}

def run_cmd(var root, var run_state)
{
    var cmd
    var ret_cmd
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_compile_cmd(root,cmd,ret_cmd)
}

def run_expr(var root, var run_state)
{
    var cmd
    var ret_expr
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_compile_expr(root,cmd,ret_expr)
}

def run_generated_bytecode(var root, var run_state)
{
    var file_name
    var ret_generated_bytecode
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_compile_generated_bytecode(root,file_name,ret_generated_bytecode)
}


def main()
{
    var mod_lang_compile_test
    var iid = "0x88695ae1, 0xf662, 0x3356, 0xee, 0xef, 0xcc, 0x8b, 0x4c, 0x0e, 0xdd, 0xf9"
    var args = "{}"
    var keys_tuple{} = {}
    var hash_key_value<> = <>
	var run_state<> = <"keys_tuple": keys_tuple, "hash_key_value":hash_key_value>
    var i
    var run_max = 1000

    debug("ERR")

    mod_lang_compile_test = create_instance(iid, args)
    print("mod_lang_compile_test:", mod_lang_compile_test)

    run_func(mod_lang_compile_test, run_state)

    run_cmd(mod_lang_compile_test, run_state)

    run_expr(mod_lang_compile_test, run_state)

    run_generated_bytecode(mod_lang_compile_test, run_state)


    for (i = 0, i < run_max, i += 1) {
        var k
        var run_random

        run_random = random_number(32)
        k = run_random % 4

        if ( k == 0) {
            run_func(mod_lang_compile_test, run_state)
        }

        if ( k == 1) {
            run_cmd(mod_lang_compile_test, run_state)
        }

        if ( k == 2) {
            run_expr(mod_lang_compile_test, run_state)
        }

        if ( k == 3) {
            run_generated_bytecode(mod_lang_compile_test, run_state)
        }

    }

    delete_instance(iid, mod_lang_compile_test)
}
