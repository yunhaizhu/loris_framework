/*
Write a function to find the longest common prefix string amongst an array of strings.

If there is no common prefix, return an empty string "".

Example 1:
Input: ["flower","flow","flight"]
Output: "fl"

Example 2:
Input: ["dog","racecar","car"]
Output: ""

Explanation: There is no common prefix among the input strings.

Note:
All given inputs are in lowercase letters a-z
*/


def longest_common_prefix(var strs, var ret_common_prefix)
{
    var strs_max
    var i
    var j
    var prefix_str
    var prefix_array[1]

    strs_max = strs.count_item()
    prefix_str = strs[0]

    for (i = 0, i < strs_max, i += 1) {
        var item
        var item_size

        item = strs[i]
        item_size = item.count_item()
        print("item", item, "item_size", item_size)

        if (item_size < prefix_str.count_item()){
            prefix_str = strs[i]
        }
    }
    print("prefix_str", prefix_str, "prefix_str_size", prefix_str.count_item())

    string_to_array(prefix_str, prefix_array)

    for (i = 0, i < prefix_array.count_item(), i += 1) {
        for (j = 0, j < strs_max, j += 1) {
            var ret_str_array[1]

            string_to_array(strs[j], ret_str_array)
            if (ret_str_array[i] != prefix_array[i]){
                var k

                for (k = 0, k < prefix_array.count_item(), k += 1) {
                    if (k >= i){
                        prefix_array[k] = ""
                    }
                }
                array_to_string(prefix_array, ret_common_prefix)
                return
            }
        }
    }
}

def main()
{
    var strs[3] = ["flower","flow","flight"]
    var ret_longest_common_prefix

    longest_common_prefix(strs, ret_longest_common_prefix)
    print("ret_longest_common_prefix:", ret_longest_common_prefix)
}