def test_while()
{
    var safe_tuple{} = {1,2,"hello string"}
    var i = 0

    while(true){
        var j

        j = i + 100
        print("i:", i, "safe_tuple:", safe_tuple{i}, "j:", j)
        i = i + 1
        if (i >= safe_tuple.count_item()){
            print("while break")
            break
        }
    }
    print("test end")
}
