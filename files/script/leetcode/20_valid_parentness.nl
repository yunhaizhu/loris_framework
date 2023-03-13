/*
Given a string s containing just the characters '(', ')', '{', '}', '[' and ']',
determine if the input string is valid.

An input string is valid if:

Open brackets must be closed by the same type of brackets.
Open brackets must be closed in the correct order.
Every close bracket has a corresponding open bracket of the same type.


 Example 1:
 Input: s = "()"
 Output: true

 Example 2:
 Input: s = "()[]{}"
 Output: true

 Example 3:
 Input: s = "(]"
 Output: false

  Constraints:
  1 <= s.length <= 104
  s consists of parentheses only '()[]{}'.
*/

import lib/hash_to_stack
require stack_init, stack_push, stack_pop, stack_is_empty


def valid_parentness(var string, var ret_valid)
{
    var string_array[1]
    var i
    var stack<>
    var ret_stack_empty

    ret_valid = 0

    stack_init(stack)

    string_to_array(string, string_array)

    for (i = 0, i < string_array.count_item(), i += 1) {
        var char
        var ret_char_pop

        char = string_array[i]
        if (char == "(" or char == "[" or char == "{"){
            if (char == "("){
                stack_push(stack, ")")
            }
            if (char == "["){
                stack_push(stack, "]")
            }
            if (char == "{"){
                stack_push(stack, "}")
            }
        }else {
            stack_is_empty(stack, ret_stack_empty)
            stack_pop(stack, ret_char_pop)
#            print("ret_stack_empty", ret_stack_empty, "ret_char_pop:", ret_char_pop, "char:", char)
            if (ret_stack_empty == 99 or char != ret_char_pop){
                ret_valid = 0
#                print("NOT valid")
                return
            }
        }
    }

    stack_is_empty(stack, ret_stack_empty)
    ret_valid = ret_stack_empty
}


def main()
{
    var string = "()"
    var ret_valid

    valid_parentness(string, ret_valid)
    print("valid_parentness:", string, ret_valid)

    string = "()[]{}"
    valid_parentness(string, ret_valid)
    print("valid_parentness:", string, ret_valid)

    string = "(]"
    valid_parentness(string, ret_valid)
    print("valid_parentness:", string, ret_valid)
}
