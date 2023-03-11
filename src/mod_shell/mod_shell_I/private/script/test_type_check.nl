def test_random_number(var a1:{"OWN_TYPE_NUMBER"}, var a2:{"OWN_TYPE_STRING", "OWN_TYPE_BOOL"})
{
    var key
    var key2
    var i

    print(a1, a2)
    #key2 = random_string(32, key)
    print(key2, key)

    random_number(32, key)

    print("random key", key)

    for (i = 0, i < 1, i += 1){
        random_string(32, key)
        print("random key", i, key)
    }
}

def test_type_check()
{
    var a1 = 100

    test_random_number(a1, "200234")

    test_random_number(a1, 20000000)

    test_random_number(a1, true)
}

