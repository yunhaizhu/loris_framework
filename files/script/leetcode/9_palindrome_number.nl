
def palindrome_number(var num, var ret)
{
    var revert_num = 0
    var x = num

    if (num < 0){
        ret = false
        return
    }

    while (x != 0) {
        revert_num = revert_num * 10 + x % 10
        x /= 10
    }

   if (num == revert_num ){
        ret = true
   }else {
        ret =false
   }
}

def main()
{
    var num = 12231
    var ret

    palindrome_number(num, ret)
    print(num, "is palindrome number:", ret)

    num = 1221
    palindrome_number(num, ret)
    print(num, "is palindrome number:", ret)

    num = 121
    palindrome_number(num, ret)
    print(num, "is palindrome number:", ret)

    num = -121
    palindrome_number(num, ret)
    print(num, "is palindrome number:", ret)
}