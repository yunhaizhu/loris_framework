def test_hash_add_find()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_3 = 1323
    var value = 100
    var safe_i
    var safe_i2
    var safe_hash<> = <123321:"hello value", 223321:"hello value 2", 1323:safe_3>

    assert(safe_3 == 1323, "safe_3 == 1323")

    value = safe_hash.find_item(123321)
    assert(value == "hello value", "value == hello value")

    value = safe_hash.find_item(1323)
    assert(value == 1323, "value == 100")

    value = safe_hash.find_item(123321)
    assert(value == "hello value", "value == hello value")

    safe_hash.add_key_item(safe_1, "hello safe_1")
    value = safe_hash.find_item(safe_1)
    assert(value == "hello safe_1", "value == hello safe_1")

    safe_i = safe_hash.find_item(1323)
    assert(safe_i == 1323, "safe_i == 1323")

    safe_3 = 1032
    safe_i2 = safe_hash.find_item(1323)
    assert(safe_i2 == 1323, "safe_i2 == 1323")
}

def test_hash_array()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_array[3] = [1,2,"hello string"]
    var safe_array2[3] = [323423, 32, "hello string array2"]
    var safe_hash<> = <123321: safe_array, 223321:"hello value 2">
    var value
    var value2
    var value3
    var value4

    value = safe_hash.find_item(123321)
    assert(value[0] == 1, "value[0] == 1")

    safe_hash.add_key_item(safe_1, safe_array2)
    value3 = safe_hash.find_item(safe_1)
    assert(value3[0] == 323423, "value3[0] == 323423")

    value2 = safe_hash.find_item(safe_1)
    assert(value2[0] == 323423, "value2[0] == 323423")

    value4 = safe_hash.find_item(safe_1)
    assert(value4[0] == 323423, "value4[0] == 323423")
}

def test_hash()
{
    print("TEST HASH BEGIN")
    test_hash_add_find()
    test_hash_array()
    print("TEST HASH END")
}

