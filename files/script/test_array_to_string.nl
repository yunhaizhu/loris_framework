
def test_array_to_string()
{
    var ret_string
    var array[6] = ["H", "E", "L", "L", "O", " WORD"]
    var ones[10] = ["", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]

    array_to_string(array, ret_string)
    print(ret_string)

    array_to_string(ones, ret_string)
    print(ret_string)
}

def main()
{
    test_array_to_string()
}