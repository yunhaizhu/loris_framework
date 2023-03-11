
def test_hash_tuple1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_tuple{} = {1,2,"hello string"}
    var safe_tuple3{}
    var safe_hash<> = <123321: safe_tuple, 223321:"hello value 2", 505432:safe_tuple>
    var value = 100
    var value2
    var value3
    var safe_tuple2{} = {2,2, 4324324, 32432423535, "hello string 2"}
    var i

    value = safe_hash.find_item(123321)
    for (i = 0, i < value.count_item(), i = i + 1) {
        print("value", i, " value:", value{i})
    }

    for (i = 0, i < safe_tuple.count_item(), i = i + 1) {
        print("safe_tuple ", i, " :", safe_tuple{i})
    }

    safe_hash.add_key_item(safe_1, safe_tuple2)
    value2 = safe_hash.find_item(safe_1)
    for (i = 0, i < value2.count_item(), i = i + 1) {
        print("value2", i, " value2:", value2{i})
    }

    value3 = safe_hash.find_item(505432)
    value3.add_item("hello world value3")
    for (i = 0, i < value3.count_item(), i = i + 1) {
        print("value3", i, " value3:", value3{i})
    }
}

def test_hash_tuple2_arg(var safe_hash)
{
    var value3
    var i

    value3 = safe_hash.find_item(505432)
    value3.add_item("hello world value3")
    for (i = 0, i < value3.count_item(), i = i + 1) {
        print("test_hash_tuple2_arg value3", i, " value3:", value3{i})
    }
}

def test_hash_tuple_arg2(var safe_tuple_arg)
{
    var i

    for (i = 0, i < safe_tuple_arg.count_item(), i = i + 1) {
        print("test_hash_tuple_arg2", i, " safe_tuple_arg:", safe_tuple_arg{i})
    }
}


def test_hash_tuple2()
{
    var safe_tuple{} = {1,2,"hello string"}
    var safe_tuple3{}
    var safe_hash<> = <123321: safe_tuple3, 223321:"hello value 2", 505432:safe_tuple>
    var value3
    var i

    print("test5_hash_tuple2 safe_hash begin")
    value3 = safe_hash.find_item(505432)
    for (i = 0, i < value3.count_item(), i = i + 1) {
        print("value3 ", i, " value3:", value3{i})
    }

    print("test5_hash_tuple2 begin")
    test_hash_tuple2_arg(safe_hash)

    value3 = safe_hash.find_item(505432)
    for (i = 0, i < value3.count_item(), i = i + 1) {
        print("value3 ", i, " value3:", value3{i})
    }

    test_hash_tuple_arg2(value3)
}

def test_hash_tuple()
{
    print("TEST HASH TUPLE BEGIN")
    test_hash_tuple1()
    test_hash_tuple2()
    print("TEST HASH TUPLE END")
}

