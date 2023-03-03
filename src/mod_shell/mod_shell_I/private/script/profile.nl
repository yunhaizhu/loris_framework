def test_while()
{
    var i
    var n

    i = 0
    n = 2000*1000*1000

    while (i < n) {
        i += 1
    }
    print("i:", i)
}


def main()
{
    test_while()
}

