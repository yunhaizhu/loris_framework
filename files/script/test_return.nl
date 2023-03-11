def test_return1()
{
    var key = 0
    var data

    random_string(12, data)
    if (key == 0){
        print("return IGNORED")
        return
    }

    print("data:", data)
}

def test_return2()
{
    var key = 0
    var data

    random_string(12, data)
    if (key == 0){
        print("return IGNORED")
        return data
    }

    print("data:", data)
}

def test_return()
{
    var ret

    test_return1()
    test_return2()

    print("ret:", ret)
}


