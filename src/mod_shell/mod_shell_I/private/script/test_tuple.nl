def test_tuple1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_tuple{} = {1, 2, 4, 3, 5, 7, 2, 4, "hello string"}
    var safe_to_tuple{} = {2, 5, 8, 9}
    var i
    var tmp

    for (i = 0, i < safe_tuple.count_item(), i += 1) {
        print("safe_tuple", i, "value:", safe_tuple{i})
    }

    safe_tuple.add_item("hello 4")
    safe_tuple.add_item(safe_1)

    for (i = 0, i < safe_tuple.count_item(), i += 1) {
        print("safe_tuple", i, "value:", safe_tuple{i})
    }

    tmp = safe_tuple.find_item(7)
    assert(tmp == 7, "tmp == 7")
}


def test_tuple2()
{
    var safe_tuple{} = {1, 2, 4, 3, 5, 7, "hello string"}
    var i
    var hello = "hello string 2222"
    var safe_tuple2{} = {343, "hello string tuple2"}

    for (i = 0, i < safe_tuple.count_item(), i += 1) {
        print("safe_tuple1", i, "value:", safe_tuple{i})
    }

    safe_tuple.add_item(hello)
    for (i = 0, i < safe_tuple.count_item(), i += 1) {
        print("safe_tuple2", i, "value:", safe_tuple{i})
    }
    assert(hello == "hello string 2222", "hello is still hello string 2222")

    safe_tuple.del_item("hello string")
    for (i = 0, i < safe_tuple.count_item(), i += 1) {
        print("safe_tuple3", i, "value:", safe_tuple{i})
    }

    print("check tuple combine2")
    for (i = 0, i < safe_tuple2.count_item(), i += 1) {
        print("safe_tuple5", i, "value:", safe_tuple2{i})
        safe_tuple.add_item(safe_tuple2{i})
    }
    print("check tuple combine result")
    for (i = 0, i < safe_tuple.count_item(), i = i + 1) {
        print("safe_tuple6", i, "value:", safe_tuple{i})
    }
}


def test_tuple()
{
    print("TEST TUPLE BEGIN")
    test_tuple1()
    test_tuple2()
    print("TEST TUPLE END")
}

