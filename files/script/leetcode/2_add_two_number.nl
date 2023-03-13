/*
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order
 and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example:

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.

*/

import lib/array_to_single_list
require array_to_single_list, single_list_loop

def add_two_number(var single_list_one, var single_list_two, var ret_single_list_one_two_sum)
{
    var single_list_one_first
    var single_list_two_first
    var ret_single_list_one_two_sum_first
    var carry = 0

    single_list_one_first = single_list_one.find_item("next")
    single_list_two_first = single_list_two.find_item("next")
    ret_single_list_one_two_sum_first = ret_single_list_one_two_sum

    while(single_list_one_first != null or single_list_two_first != null or carry){
        var item_value_1
        var item_value_2
        var add
        var item_hash<>

        item_value_1 = single_list_one_first.find_item("value")
        item_value_2 = single_list_two_first.find_item("value")

        if (item_value_1 == null){
            item_value_1 = 0
        }
        if (item_value_2 == null){
            item_value_2 = 0
        }

        add = item_value_1 + item_value_2 + carry

        if (add >=10){
            carry = 1
            add = add - 10
        }else {
            carry = 0
        }
        print("add", add, item_value_1, item_value_2, carry)

        single_list_one_first = single_list_one_first.find_item("next")
        single_list_two_first = single_list_two_first.find_item("next")

        item_hash.add_key_item("value", add)

        ret_single_list_one_two_sum_first.add_key_item("next", item_hash)
        ret_single_list_one_two_sum_first = ret_single_list_one_two_sum_first.find_item("next")
    }
}

def main()
{
    var one[3] = [2, 4, 3]
    var two[3] = [5, 6, 4]
    var single_list_one<>
    var single_list_two<>
    var ret_simple_list_one_two_sum<>

    array_to_single_list(one, single_list_one)
    single_list_loop(single_list_one)

    array_to_single_list(two, single_list_two)
    single_list_loop(single_list_two)

    add_two_number(single_list_one, single_list_two, ret_simple_list_one_two_sum)
    single_list_loop(ret_simple_list_one_two_sum)

}