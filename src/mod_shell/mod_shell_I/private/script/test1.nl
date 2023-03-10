def test1_var1()
{
    var safe_i = "string i"
    var safe_2 = "string 2"
    var safe_to
    var safe_to2
    var safe_three
    var safe_four

    print("safe_i:", safe_i, " safe_i.get():", safe_i.get())
    assert(safe_i == "string i", "safe_i == string i")
    assert(safe_i.get() == "string i", "safe_i.get() == string i")

    safe_to = 123
    assert(safe_to.get() == 123, "safe_to.get() == 123")

    safe_three = "hello safe_three"
    print("safe_to:", safe_to, " safe_three.get():", safe_three.get())

    safe_to = safe_i.get()
    print("safe_to:", safe_to)
    assert(safe_to == "string i", " safe_to == string i")

    safe_to2 = safe_i
    print("safe_to2:", safe_to2, " safe_i:", safe_i)
    assert(safe_to2 == "string i", " safe_to2 set to safe_i")

    safe_three = safe_i
    print("safe_three:", safe_three)
    assert(safe_three == "string i", " safe_three set to safe_i")

    safe_three.set("hello safe_three")
    print("safe_three:", safe_three)
    assert(safe_three == "hello safe_three", "safe_three is hello safe_three")

    print("safe_i:", safe_i)
    assert(safe_i != "hello safe_three", "safe_i is still string i")
}

def test1_var2()
{
    var i
    var j = 99U8
    var d = true
    var k

    print("test1_var2")

    random_string(123, k)
    print("k:", k)
    print("d:", d)

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

    d = false
}

def test1_var()
{
    print("TEST VAR BEGIN")
    test1_var1()
    test1_var2()
    print("TEST VAR END")
}

def main()
{
    test1_var()
}


