def test_lifetime()
{
    var ret

    ret = 100

    print("ret:", ret)
    assert(ret == 100, "ret == 100")
    {
        var ret
        var i
        var j

        ret = 300
        i = 2000
        j = "hello world j"
        print("block ret:", ret, "block i:", i, "block j:", j)
        assert(ret == 300, "ret == 300")
        assert(i == 2000, "i == 2000")
        assert(j == "hello world j", "j == hello world j")
        random_number(32, i)
        print("random i:", i)
    }
    print("ret:", ret)
    assert(ret == 100, "ret == 100")
}
