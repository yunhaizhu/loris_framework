def test_key(var run_state)
{
    var hash_key_value
    var key
    var data
    var data2
    var data3
    var hash_key_value33<> = <4324324:"hello">

    random_number(32, key)
    random_string(12, data)

    hash_key_value = run_state.find_item("hash_key_value")

    data3 = data.get()
    print("data3:", data3)
    hash_key_value.add_key_item(key, data)

    print("data:", data)

    data2 = hash_key_value.find_item(key)
    print("key:", key, "data3:", data3, "data2:", data2)
    assert(data3 == data2, "COMPARE MEMORY")

    hash_key_value.add_key_item(key, "key data")
    data2 = hash_key_value.find_item(key)
    assert(data2 == "key data", "data2 == key data")

    hash_key_value.add_key_item(key, hash_key_value33)
    data2 = hash_key_value.find_item(key)
    print("data2:", data2)
    data3 = data2.find_item(4324324)
    assert(data3 == "hello", "data3 == hello")
}

def test8_hash_hash()
{
    var hash_key_value<> = <>
	var hash_value_key<> = <>
    var run_state<> = <"hash_key_value":hash_key_value, "hash_value_key":hash_value_key>

    test_key(run_state)
}

def main()
{
    test8_hash_hash()
}
