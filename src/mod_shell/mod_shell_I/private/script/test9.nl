def test_key_data(var key, var data, var ret)
{
   ret = 100
}

def test9_arg()
{
    var key=99
    var data="string"
    var ret

    print("test9_arg ret", key, data, ret)

    test_key_data(key, data, ret)

    print("test9_arg ret", key, data, ret)
    assert(ret == 100, "ret == 100")
}

def main()
{
    test9_arg()
}
