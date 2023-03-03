
def test5_hash_tuple1()
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

    print("testing test5_hash_tuple #1")
    value = safe_hash.find_item(123321)
    print("value:", value{0})
    for (i = 0, i < value.count_item(), i = i + 1) {
        print("value", i, " value:", value{i})
    }
    print("safe_tuple{0}:", safe_tuple{0})

    print("testing test5_hash_tuple #2")
    safe_hash.add_key_item(safe_1, safe_tuple2)

    print("testing test5_hash_tuple #3")
    value2 = safe_hash.find_item(safe_1)
    print("value:", value2{0})
    for (i = 0, i < value2.count_item(), i = i + 1) {
        print("value2", i, " value2:", value2{i})
    }

    value3 = safe_hash.find_item(505432)
    print("value3:", value3{0})
    value3.add_item("hello world value3")
    for (i = 0, i < value3.count_item(), i = i + 1) {
        print("value3", i, " value3:", value3{i})
    }
}



def test5_hash_tuple2_arg(var safe_hash)
{
    var value3
    var i

    print("test5_hash_tuple2_arg value3")
    value3 = safe_hash.find_item(505432)
    print("test5_hash_tuple2_arg value3:", value3{0})
    value3.add_item("hello world value3")
    for (i = 0, i < value3.count_item(), i = i + 1) {
        print("test5_hash_tuple2_arg value3", i, " value3:", value3{i})
    }
}

def test5_hash_tuple_arg2(var safe_tuple_arg)
{
    var i

    for (i = 0, i < safe_tuple_arg.count_item(), i = i + 1) {
        print("test5_hash_tuple_arg2", i, " safe_tuple_arg:", safe_tuple_arg{i})
    }
}


def test5_hash_tuple2()
{
    var safe_tuple{} = {1,2,"hello string"}
    var safe_tuple3{}
    var safe_hash<> = <123321: safe_tuple, 223321:"hello value 2", 505432:safe_tuple>
    var value3
    var i

    print("test5_hash_tuple2 begin")
    test5_hash_tuple2_arg(safe_hash)

    value3 = safe_hash.find_item(505432)
    print("test5_hash_tuple2 value3:", value3{0})
    for (i = 0, i < value3.count_item(), i = i + 1) {
        print("value3", i, " value3:", value3{i})
    }

    test5_hash_tuple_arg2(value3)
}

def test5_hash_tuple()
{
    test5_hash_tuple1()
    test5_hash_tuple2()
}

def main()
{
    test5_hash_tuple()
}
