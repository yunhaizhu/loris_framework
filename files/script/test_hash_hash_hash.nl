def test_hash_hash_hash_key(var run_state)
{
    var hash_key_value
    var key
    var value
    var hash_hash
    var hash_hash_key_value<> = <4324324:"hello", 321:"world">

    random_number(32, key)

    hash_key_value = run_state.find_item("hash_key_value")

    hash_key_value.add_key_item(key, hash_hash_key_value)
    hash_hash = hash_key_value.find_item(key)
    print("hash_hash:", run_state, hash_key_value, hash_hash)
    value = hash_hash.find_item(4324324)
    assert(value == "hello", "value == hello")

    value = hash_hash_key_value.find_item(321)
    assert(value == null, "value is null, hash_hash_key_value is moved to hash_key_value")
}

def test_hash_hash_hash()
{
    var hash_key_value<> = <>
	var hash_value_key<> = <>
    var run_state<> = <"hash_key_value":hash_key_value, "hash_value_key":hash_value_key>

    print("test_hash_hash_hash BEGIN")
    test_hash_hash_hash_key(run_state)
    print("test_hash_hash_hash END")
}

