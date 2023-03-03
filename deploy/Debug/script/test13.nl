def test13_make_json(var ret_json_string)
{
    var u64_up_key
    var u64_down_key
    var p_data
    var p_address
    var name_value_hash<> = <>
    var json_args
    var u64_up_key2
    var u64_down_key2
    var p_data2
    var p_address2

    random_number(64, u64_up_key)
    random_number(64, u64_down_key)
    random_string(12, p_data)
    random_address(p_address)

    name_value_hash.add_key_item("u64_up_key", u64_up_key)
    name_value_hash.add_key_item("u64_down_key", u64_down_key)
    name_value_hash.add_key_item("p_data", p_data)
    name_value_hash.add_key_item("p_address", p_address)

    make_json(name_value_hash, ret_json_string)

    print("json_args", ret_json_string)
#    ret_json_string = json_args
}

def test13_parse_json(var json_string)
{
    var u64_up_key
    var u64_down_key
    var p_data
    var p_address
    var name_value_hash<> = <"u64_up_key":3U64, "u64_down_key":3U64, "p_data":"p_data", "p_address":0xFF>
    var json_args

    parse_json(json_string, name_value_hash)

    u64_up_key = name_value_hash.find_item("u64_up_key")
    u64_down_key = name_value_hash.find_item("u64_down_key")
    p_data = name_value_hash.find_item("p_data")
    p_address = name_value_hash.find_item("p_address")

    print("u64_up_key:", u64_up_key, "u64_down_key:", u64_down_key, "p_data:", p_data, "p_address:", p_address)
}

def test13_make_json2()
{
    var name_value_hash<> = <>
    var json_args

    make_json(name_value_hash, json_args)
    print("json_args", json_args)
}

def test13_make_parse_json()
{
    var ret_json_string
    var ret

    test13_make_json(ret_json_string)
    print("test13_make_parse_json ret_json_string:", ret_json_string)
    test13_parse_json(ret_json_string)

    print("ret:", ret)
    test13_parse_json(ret)

    test13_make_json2()
}

def main()
{
    test13_make_parse_json()
}
