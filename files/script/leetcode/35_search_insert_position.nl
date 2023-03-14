/*
Given a sorted array of distinct integers and a target value, return the index if the target is found. If not,
return the index where it would be if it were inserted in order.

You must write an algorithm with O(log n) runtime complexity.

 Example 1:
 Input: nums = [1,3,5,6], target = 5
 Output: 2

 Example 2:
 Input: nums = [1,3,5,6], target = 2
 Output: 1

 Example 3:
 Input: nums = [1,3,5,6], target = 7
 Output: 4

  Constraints:
  1 <= nums.length <= 104
  -104 <= nums[i] <= 104
  nums contains distinct values sorted in ascending order.
  -104 <= target <= 104
*/

def search_insert_position(var nums, var target, var ret_index)
{
    var left
    var right
    var nums_max
    var mid

    nums_max = nums.count_item()
    left = 0
    right = nums_max

    while( left < right){
        mid = left + (right - left) / 2

        if (nums[mid] == target){
            ret_index = mid
            return
        }

        if (nums[mid] > target){
            right = mid
        }else {
            left = mid + 1
        }
    }
    ret_index = left
}

def main()
{
    var nums[4] = [1,3,5,6]
    var target = 5
    var ret_index

    search_insert_position(nums, target, ret_index)
    print(target, "FIND:", ret_index)

    target = 2
    search_insert_position(nums, target, ret_index)
    print(target, "FIND:", ret_index)

    target = 7
    search_insert_position(nums, target, ret_index)
    print(target, "FIND:", ret_index)
}