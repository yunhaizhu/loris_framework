
def int_to_roman(var num, var ret)
{
    var ones[10] = ["", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"]
    var tens[10] = ["", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"]
    var hundreds[10] = ["", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"]
    var thousands[4] = ["", "M", "MM", "MMM"]
    var roman[4]

    roman[0] = thousands[num / 1000]
    roman[1] = hundreds[(num % 1000) / 100]
    roman[2] = tens[(num % 100) / 10]
    roman[3] = ones[num % 10]
    array_to_string(roman, ret)
}

def main()
{
    var num = 3
    var ret

    num = 3
    int_to_roman(num, ret)
    print(num, "int to roman:", ret)

    num = 58
    int_to_roman(num, ret)
    print(num, "int to roman:", ret)

    num = 1994
    int_to_roman(num, ret)
    print(num, "int to roman:", ret)
}
