/*
Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes
of the first two lists.

Example:
Input: 1->2->4, 1->3->4
Output: 1->1->2->3->4->4
*/

import lib/array_to_single_list
require array_to_single_list, single_list_loop

def merge_two_sorted_linked_list(var single_list_one, var single_list_two, var ret_merge_two_sorted_linked_list)
{
    var single_list_one_first
    var single_list_two_first
    var ret_merge_two_sorted_linked_list_first

    single_list_one_first = single_list_one.find_item("next")
    single_list_two_first = single_list_two.find_item("next")
    ret_merge_two_sorted_linked_list_first = ret_merge_two_sorted_linked_list

    while(single_list_one_first != null and single_list_two_first != null){
        var item_value_1
        var item_value_2
        var insert_value
        var item_hash<>

        item_value_1 = single_list_one_first.find_item("value")
        item_value_2 = single_list_two_first.find_item("value")

        if (item_value_1 == null){
            item_value_1 = 0
        }
        if (item_value_2 == null){
            item_value_2 = 0
        }

        if (item_value_1 < item_value_2){
            insert_value = item_value_1
            single_list_one_first = single_list_one_first.find_item("next")
        }else {
            insert_value = item_value_2
            single_list_two_first = single_list_two_first.find_item("next")
        }
        print("insert_value", insert_value, item_value_1, item_value_2)

        item_hash.add_key_item("value", insert_value)
        ret_merge_two_sorted_linked_list_first.add_key_item("next", item_hash)
        ret_merge_two_sorted_linked_list_first = ret_merge_two_sorted_linked_list_first.find_item("next")
    }
    if (single_list_one_first != null){
        ret_merge_two_sorted_linked_list_first.add_key_item("next", single_list_one_first)
    }else {
        ret_merge_two_sorted_linked_list_first.add_key_item("next", single_list_two_first)
    }

}

def main()
{
    var one[4] = [1, 2, 4, 5]
    var two[3] = [1, 3, 4]
    var single_list_one<>
    var single_list_two<>
    var ret_merge_two_sorted_linked_list<>

    array_to_single_list(one, single_list_one)
    single_list_loop(single_list_one)

    array_to_single_list(two, single_list_two)
    single_list_loop(single_list_two)

    merge_two_sorted_linked_list(single_list_one, single_list_two, ret_merge_two_sorted_linked_list)
    single_list_loop(ret_merge_two_sorted_linked_list)

}