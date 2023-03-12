
def roman_to_int(var string, var ret)
{
    var ret_array[3]
    var i
    var num
    var ans = 0
    var max

    make_array(string, ret_array)

    max = ret_array.count_item()

    for (i = max - 1, i >= 0, i -= 1) {
        var item

        item = ret_array[i]

        if (item == "I"){
            num = 1
        }
        if ( item == "V"){
            num = 5
        }
        if ( item == "X"){
            num = 10
        }
        if ( item == "L"){
            num = 50
        }
        if ( item == "C"){
            num = 100
        }
        if ( item == "D"){
            num = 500
        }
        if ( item == "M"){
            num = 1000
        }

        if (4 * num < ans){
            ans -= num
        }else{
            ans += num
        }
   }
    ret = ans
}

def main()
{
    var string = "III"
    var ret

    roman_to_int(string, ret)
    print(string, "roman to int:", ret)

    string = "LVIII"
    roman_to_int(string, ret)
    print(string, "roman to int:", ret)

    string = "MCMXCIV"
    roman_to_int(string, ret)
    print(string, "roman to int:", ret)
}