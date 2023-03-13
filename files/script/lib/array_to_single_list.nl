def array_to_single_list(var array, var single_list)
{
    var current_hash
    var max
    var i

    current_hash = single_list
    max = array.count_item()
    for (i = 0, i < max, i += 1) {
        var item_hash<>
        var item_value

        item_value = array[i]
        item_hash.add_key_item("value", item_value)

        current_hash.add_key_item("next", item_hash)
        current_hash = current_hash.find_item("next")
     }
}

def single_list_loop(var single_list)
{
    var current_hash

    current_hash = single_list.find_item("next")
    while(current_hash != null){
        var item_value

        item_value = current_hash.find_item("value")
        if (item_value == null){
            item_value = 0
        }
        print("single_list_loop item_value:", item_value)
        current_hash = current_hash.find_item("next")
    }
}

#def main()
#{
#    var ones[10] = ["", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]
#    var single_list<>
#
#    array_to_single_list(ones, single_list)
#    single_list_loop(single_list)
#}