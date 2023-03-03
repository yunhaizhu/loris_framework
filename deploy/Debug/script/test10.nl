def test10_return1()
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

def test10_return2()
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

def test10_return()
{
    var ret

    test10_return1()
    test10_return2()

    print("ret:", ret)
}

def main()
{
    test10_return()
}
