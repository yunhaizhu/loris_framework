## install depends in centos
1. sudo dnf install epel-release epel-next-release
2. yum -y install gsl-devel
3. sudo yum install xxhash-devel
4. sudo yum install libconfuse-devel
5. sudo yum -y install pprof
6. sudo yum install golang-bin
7. wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.9.1/gperftools-2.9.1.tar.gz
8. sudo yum install elfutils-libelf-devel
9. sudo yum install lex
10. yum -y install flex-devel
11. sudo yum install json-c-devel
12. sudo yum install python3-pip
13. pip3 install jinja2


## install depends in ubuntu
1. sudo apt-get install libjson-c-dev
2. sudo apt-get install flex
3. sudo apt-get install yacc
4. sudo apt install libelf-dev
5. sudo apt install libconfuse-dev
6. sudo apt-get install fuse
7. sudo apt install git
8. wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.9.1/gperftools-2.9.1.tar.gz
9. sudo apt install python3-pip
10. pip3 install jinja2
11. sudo apt install indent

## compile loris_framework
1. cmake --build cmake-build-debug
2. cmake --install cmake-build-debug

## run loris_framework 
Use these steps to run

1. cd deploy/Debug
2. ./loris_framework
3. type in console to exit the loris_framework with: exit()

## run loris_framework with Profile
Use these steps to profile 

1. CPUPROFILE=/tmp/prof.out CPUPROFILESIGNAL=12 ./loris_framework
2. ~/go/bin/pprof -pdf /tmp/prof.out 
3. killall -12 loris_framework


## Add doxygen
find ./src -name *.[ch] |xargs python3 add_doxygen_comment.py 

## code generator for helloworld module
1. cd code_generator
2. python3 code_generator.py idl/mod_helloworld_I.idl
3. copy directory "target/orig/1.0.0/mod_helloworld_I" into "src/"
4. compile and run 
5. type in console with: install("mod_helloworld_I")
6. type in console with: start(5)
7. type in console with: script("mod_helloworld_I_test.nl")

## Usage
A MIPS virtual machine with JIT support is built on loris framework. 
https://github.com/yunhaizhu/loris_framework_mips

## loris language
1. Loris is a ownership based memory safe script language, can provide memory safe like RUST, and easy to use like Python. 
2. Every variable has a ownership, only the owner can handle the variable. 

## test loris language
1. script("test_all.nl")
2. script("test_var_math.nl")
3. script("test_string_to_array.nl")
4. script("test_array_to_string.nl")
5. script("test_hash_link.nl")
6. script("leetcode/array_to_single_list.nl")
7. script("test_or3.nl")

## test leetcode in loris_language
1. script("leetcode/1_two_sum.nl")
2. script("leetcode/1_two_sum_2.nl")
3. script("leetcode/9_palindrome_number.nl")
4. script("leetcode/12_int_to_roman.nl")
5. script("leetcode/13_roman_to_int.nl")
6. script("leetcode/13_roman_to_int_2.nl")
7. script("leetcode/2_add_two_number.nl")

## COPYRIGHT
GNU General Public License, version 3


