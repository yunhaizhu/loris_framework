
def test_make_array()
{
    var string = "HELLO WORLD"
    var ret_array[3]
    var i
    var max

    max = ret_array.count_item()
    print(max)
    make_array(string, ret_array)

    max = ret_array.count_item()
    print(max)

    for (i = 0, i < max, i = i + 1) {
        var item
        item = ret_array[i]

        print("i", i, item)
    }
}

#def main()
#{
#    test_make_array()
#}