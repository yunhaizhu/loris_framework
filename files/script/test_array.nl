def test_array_1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_array[3] = [1,2,"hello string"]

    assert(safe_array[0] == 1, "safe_array[0] == 1")
    assert(safe_array[1] == 2, "safe_array[1] == 2")
    assert(safe_array[2] == "hello string", "safe_array[2] == hello string")

    safe_array[1] = safe_1
    assert(safe_array[1] == "string 1", "safe_array[1] == string 1")
    assert(safe_1 == "string 1", "safe_1 == string 1")

    safe_array[1] = safe_2.get()
    assert(safe_array[1] == "string 2", "safe_array[1] == string 2")
    assert(safe_2 == "string 2", "safe_2 == string 2")
}

def test_array_2()
{
    var safe_array[3]
    var safe_array2[22]
    var i = 0

    safe_array[0] = "string array 0"
    assert(safe_array[0] == "string array 0", "safe_array[0] == string array 0")

    safe_array[i] = 123
    assert(safe_array[i] == 123, "safe_array[0] == 123")

    safe_array[4] = 432434
    assert(safe_array[4] == null, "safe_array[4] == null")

    safe_array2[0] = safe_array
    assert(safe_array2[0] == 0, "safe_array2[0] == 0")
}

def test_array()
{
    print("TEST ARRAY BEGIN")
    test_array_1()
    test_array_2()
    print("TEST ARRAY END")
}

def main()
{
    test_array()
}

