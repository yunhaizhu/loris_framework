load_lib "shell_lib"
import test_var, test_array, test_tuple, test_hash, test_hash_tuple, test_hash_hash, test_hash_hash_hash, test_lifetime, test_for, test_arg, test_return, test_while, test_ifelse, test_make_parse_json, test_andor_math, test_type_check
require test_var, test_array, test_tuple, test_hash, test_hash_tuple, test_hash_hash, test_hash_hash_hash, test_lifetime, test_for, test_arg, test_return, test_while, test_ifelse, test_make_parse_json, test_andor_math, test_type_check

def main()
{
    debug("ERR")

    test_var()
    test_array()
    test_tuple()
    test_hash()
    test_hash_tuple()
    test_hash_hash()
    test_hash_hash_hash()
    test_lifetime()
    test_for()
    test_arg()
    test_return()
    test_while()
    test_ifelse()
    test_make_parse_json()
    test_andor_math()
    test_type_check()
}

