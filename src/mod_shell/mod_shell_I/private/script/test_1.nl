
def test_var_type()
{
    var i
    var j = 99U8
    var d = true
    var k

    print("test_var_type begin")

    random_string(123, k)
    print("k:", k)

    assert(d == true, "d == true")

    d = false
    assert(d == false, "d == false")

    assert(j == 99U8, "j == 99U8")

    j = 100U16
    assert(j == 100U16, "j == 100U16")

    j = 101U32
    assert(j == 101U32, "j == 101U32")

    j = 102I8
    assert(j == 102I8, "j == 102I8")

    j = 103I16
    assert(j == 103I16, "j = 103I16")

    j = 104I32
    assert(j == 104I32, "j == 104I32")

    j = 105I64
    assert(j == 105I64, "j == 105I64")

    j = 106U64
    assert(j == 106U64, "j == 106U64")

    j = 107
    assert(j != 3243, "j != 3243")
    assert(j >= 32, "j >= 32")
    assert(j <= 323432, "j <= 323432")
    assert(j > 32,  "j > 32")
    assert(j < 323432, "j < 323432")

    j = 110

    assert(j + 10 == 120, "j + 10 == 120")
    assert(j - 10 == 100, "j - 10 == 100")
    assert(j * 10 == 1100, "j * 10 == 1100")
    assert(j / 10 == 11, "j / 10 == 11")

    j = 3
    assert(j % 10 == 3, "j % 10 == 3")

    print("test_var_type end")
}

def test_var_get()
{
    var safe_1 = "string 1"

    print("safe_1:", safe_1, "safe_1.get():", safe_1.get())
    assert(safe_1 == "string 1", "safe_1 == string 1")
    assert(safe_1.get() == "string 1", "safe_i.get() == string 1")
}

def test_var_set()
{
    var safe_1 = "string 1"

    safe_1.set("hello world")
    print("safe_1:", safe_1)
    assert(safe_1 == "hello world", "safe_1 is hello world")
    assert(safe_1.get() == "hello world", "safe_1.get() == hello world")
}

def test_var()
{
    print("TEST VAR BEGIN")
    test_var_type()
    test_var_get()
    test_var_set()
    print("TEST VAR END")
}

def main()
{
    test_var()
}
