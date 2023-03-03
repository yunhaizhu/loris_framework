def mod_lang_parse_parse(var root, var file_name, var source_buffer, var source_buffer_len, var ret)
{
    var name_value_hash<> = <"file_name":file_name,"source_buffer":source_buffer,"source_buffer_len":source_buffer_len>
    var json_args

    json_args = make_json(name_value_hash)

    ret = run("mod_lang_parse_parse", root, json_args)
}


def run_parse(var root, var run_state)
{
    var file_name
    var source_buffer
    var source_buffer_len
    var ret_parse
    var keys_tuple = run_state.find("keys_tuple")
    var hash_key_value = run_state.find("hash_key_value")

    mod_lang_parse_parse(root,file_name,source_buffer,source_buffer_len,ret_parse)
}


def main()
{
    var mod_lang_parse_test
    var iid = "0xa0bb6965, 0x8e6c, 0xf1f6, 0x5c, 0x80, 0x4a, 0x2d, 0x38, 0xde, 0x86, 0xba"
    var args = "{}"
    var keys_tuple{} = {}
    var hash_key_value<> = <>
	var run_state<> = <"keys_tuple": keys_tuple, "hash_key_value":hash_key_value>
    var i
    var run_max = 1000

    debug("ERR")

    mod_lang_parse_test = create_instance(iid, args)
    print("mod_lang_parse_test:", mod_lang_parse_test)

    run_parse(mod_lang_parse_test, run_state)


    for (i = 0, i < run_max, i += 1) {
        var k
        var run_random

        run_random = random_number(32)
        k = run_random % 1

        if ( k == 0) {
            run_parse(mod_lang_parse_test, run_state)
        }

    }

    delete_instance(iid, mod_lang_parse_test)
}
