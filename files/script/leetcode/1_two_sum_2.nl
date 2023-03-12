
def two_sum(var nums, var nums_max, var target, var ret_i, var ret_j)
{
    var i
    var j
    var hash_table<>

    for (i = 0, i < nums_max, i += 1) {
        var key
        var index

        index = hash_table.find_item(nums[i])
        if ( index != null){
            ret_i = index - 1
            ret_j = i
            return
        }else {
            key = target - nums[i]
            hash_table.add_key_item(key, i + 1)
        }
    }
}

def main()
{
    var nums[4] = [2, 12, 15, 7]
    var target = 9
    var i
    var j

    two_sum(nums, 4, target, i, j)
    print("TWO SUM FIND i, j:", i, j)
}