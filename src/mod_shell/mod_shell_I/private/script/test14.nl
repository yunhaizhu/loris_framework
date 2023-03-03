def test14_andor1()
{
    var a1 = 1
    var a2 = 0

    if (a1 and a2){
        print("a1 and a2 is true", a1, a2)
    } else {
        print("a1 and a2 is false", a1, a2)
    }
}

def test14_andor2()
{
    var a1 = 1
    var a2 = 0

    if (a1 or a2){
        print("a1 or a2 is true", a1, a2)
    } else {
        print("a1 or a2 is false", a1, a2)
    }
}

def test14_math3()
{
    var a1 = 1
    var a2 = 0

    a2 = a1 + 3 * 2
    print("a2", a2)

    a2 = (a1 + 3) * 2
    print("a2", a2)

    a2 += 1
    print("a2 += 1", a2)

    a2 -= 1
    print("a2 -= 1", a2)

    a2 *= 8
    print("a2 *= 8", a2)

    a2 /= 2
    print("a2 /= 2", a2)

    a2 %= 3
    print("a2 %= 3", a2)

    #comment

    a2 = 0x343a4425AF
}

def test14_andor()
{
    test14_andor1()
    test14_andor2()
    test14_math3()
}

def main()
{
    test14_andor()
}
