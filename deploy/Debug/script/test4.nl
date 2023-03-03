def test4_hash1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_3 = 1323
    var safe_hash<> = <123321:"hello value", 223321:"hello value 2", 1323:safe_3>
    var value = 100
    var safe_i
    var safe_i2

    print("testing safe_hash #0")
    print("safe_3:", safe_3)
    assert(safe_3 == null, "safe_3 == null")

    print("testing safe_hash #1")
    value = safe_hash.find_item(123321)
    print("value:", value)
    assert(value == "hello value", "value == hello value")
    value = 100

    print("testing safe_hash #2")
    value := safe_hash.find_item(123321)
    print("value:", value)
    assert(value == "hello value", "value == hello value")
    value = 100

    print("testing safe_hash #3")
    value &= safe_hash.find_item(123321)
    print("value:", value)
    assert(value == 100, "value == 100")

    print("testing safe_hash #4")
    safe_hash.add_key_item(safe_1, "hello safe_1")
    value := safe_hash.find_item(safe_1)
    print("value", value)
    assert(value == "hello safe_1", "value == hello safe_1")

    print("testing safe_hash #5")
    safe_i = safe_hash.find_item(1323)
    print("safe_i:", safe_i)
    assert(safe_i == 1323, "safe_i == 1323")

    print("testing safe_hash #6")
    print("safe_3:", safe_3)

    print("testing safe_hash #7")
    safe_3 = 1032
    print("safe_hash.find_item(1323)", safe_hash.find_item(1323))
    safe_i2 = safe_hash.find_item(1323)
    assert(safe_i2 == 1323, "safe_i2 == 1323")
}

def test4_hash_array()
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

    print("testing test4_hash_array #1")
    value = safe_hash.find_item(123321)
    print("value:", value[0])
    assert(value[0] == 1, "value[0] == 1")

    print("value[2]:", value[2])

    print("testing test4_hash_array #2")
    safe_hash.add_key_item(safe_1, safe_array2)

    value3 &= safe_hash.find_item(safe_1)
    print("value:", value3[0])
    assert(value3[0] == 323423, "value3[0] == 323423")

    print("testing test4_hash_array #3")
    value2 = safe_hash.find_item(safe_1)
    print("value:", value2[0])
    assert(value2[0] == 323423, "value2[0] == 323423")

    value4 := safe_hash.find_item(safe_1)
    print("value4[0]:", value4[0])
    assert(value4[0] == 323423, "value4[0] == 323423")

    print("value4[2]:", value4[2])
    print("value4[3]:", value4[2])
    print("value[2]:", value[2])
}



def test4_hash()
{
    test4_hash1()
    test4_hash_array()
}

def main()
{
    test4_hash()
}
