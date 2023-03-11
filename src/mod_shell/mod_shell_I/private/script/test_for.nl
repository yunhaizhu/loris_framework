def test_for()
{
    var i
    var j
    var max
    var safe_tuple{} = {1,2,"hello string"}

	max = safe_tuple.count_item()
	print("max:", max)
    for (i = 0, i < max, i = i + 1) {
        var item

        print("BEGIN")
        item = safe_tuple{i}
        print("END")

        for (j = 0, j < 3, j = j + 1) {
            print("j:", j)
            if (j == 2){
                print("for j break")
                break
            }
        }

        if (i == 1){
            print("for i break ", i)
            break
        }

        print("i:", i, "item:", item, "safe_tuple{i}:", safe_tuple{i})
    }
}

