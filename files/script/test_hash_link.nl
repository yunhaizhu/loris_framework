def test_hash_link()
{
    var hash_link<> =<"hello": "hello">
    var current_hash
    var max
    var i
    var ones[10] = ["W", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]

    current_hash = hash_link
    print("current_hash1", current_hash)

    max = ones.count_item()
    for (i = max - 1, i >= 0, i -= 1) {
        var item_hash<>
        var item_value
        var item_next

        item_value = ones[i]
        item_hash.add_key_item("value", item_value)

        current_hash.add_key_item("next", item_hash)
        current_hash = current_hash.find_item("next")

        item_value = current_hash.find_item("value")
        item_next = current_hash.find_item("next")
        print("current_hash", current_hash, "item_value:", item_value, "item_next:", item_next)
     }

    current_hash = hash_link
    while(current_hash != null){
        var item

        item = current_hash.find_item("value")
        print("while value:", item)
        current_hash = current_hash.find_item("next")
    }
}

def main()
{
    test_hash_link()
}