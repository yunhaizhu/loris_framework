
def two_sum(var nums, var nums_max, var target, var ret_i, var ret_j)
{
    var i
    var j

    for (i = 0, i < nums_max, i += 1) {
        for (j = i + 1, j < nums_max, j += 1) {
            var sum
            sum = nums[i] + nums[j]
            if (sum == target){
                ret_i = i
                ret_j = j
                return
            }
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