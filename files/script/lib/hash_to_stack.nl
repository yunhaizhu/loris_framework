def stack_init(var stack)
{
    stack.add_key_item("stack_index", 1)
}

def stack_push(var stack, var push_value)
{
    var stack_index

    stack_index = stack.find_item("stack_index")

    stack_index += 1
    stack.add_key_item(stack_index, push_value)
    stack.add_key_item("stack_index", stack_index)
}

def stack_pop(var stack, var ret_pop_value)
{
    var stack_index

    stack_index = stack.find_item("stack_index")
    ret_pop_value = stack.find_item(stack_index)
    stack.del_item(stack_index)

    stack_index -= 1
    stack.add_key_item("stack_index", stack_index)
}

def stack_is_empty(var stack, var ret_pop_value)
{
    var stack_index

    stack_index = stack.find_item("stack_index")

    if (stack_index == 1){
        ret_pop_value = 1
    }else {
        ret_pop_value = 0
    }
}

def test_abc(var stack, var ret_valid)
{
    var ret_stack_empty = 0

    stack_is_empty(stack, ret_stack_empty)
    ret_valid = ret_stack_empty
    print("RET_VASLID", ret_stack_empty, ret_valid)

    stack_is_empty(stack, ret_valid)
    print("RET_VASLID2", ret_stack_empty, ret_valid)

}

/*
def main()
{
    var ones[10] = ["", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]
    var stack<>
    var i
    var ret_valid = 0

    stack_init(stack)

    for (i = 0, i < ones.count_item(), i = i + 1) {
        var item

        item = ones[i]
        stack_push(stack, item)
    }

    for (i = 0, i < ones.count_item(), i = i + 1) {
        var item

        stack_pop(stack, item)
        print("item", item)
    }

    test_abc(stack, ret_valid)
    print("ret_valid", ret_valid)
}
*/