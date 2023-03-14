/*
You are climbing a stair case. It takes n steps to reach to the top.

Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

Note: Given n will be a positive integer.

Example 1:
Input: 2
Output: 2
Explanation: There are two ways to climb to the top.
1. 1 step + 1 step
2. 2 steps

Example 2:
Input: 3
Output: 3
Explanation: There are three ways to climb to the top.
1. 1 step + 1 step + 1 step
2. 1 step + 2 steps
3. 2 steps + 1 step
*/

def climb_stairs(var numb, var ret_count)
{
    var numb_array[1]
    var i

    numb_array.resize(numb + 1)
    numb_array[0] = 1
    numb_array[1] = 1
    for (i = 2, i <= numb, i+=1){
        numb_array[i] = numb_array[i - 1] + numb_array[i -2]
    }
    ret_count = numb_array[numb]
}

def climb_stairs_2(var numb, var ret_count)
{
    var numb_array[3]
    var i

    numb_array[0] = 1
    numb_array[1] = 1
    for (i = 2, i <= numb, i+=1){
        numb_array[i%3] = numb_array[(i - 1)%3] + numb_array[(i -2)%3]
    }
    ret_count = numb_array[numb%3]
}

def main()
{
    var numb = 4
    var ret_count

    climb_stairs(numb, ret_count)
    print(numb, ret_count)
    climb_stairs_2(numb, ret_count)
    print(numb, ret_count)

    numb = 5
    climb_stairs(numb, ret_count)
    print(numb, ret_count)
    climb_stairs_2(numb, ret_count)
    print(numb, ret_count)
}