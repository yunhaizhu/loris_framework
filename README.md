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

## run loris_framework 
Use these steps to run

1. cd deploy/Debug
2. ./loris_framework
3. type "exit()" to exit the loris_framework

## run loris_framework with Profile
Use these steps to profile 

1. CPUPROFILE=/tmp/prof.out CPUPROFILESIGNAL=12 ./loris_framework
2. ~/go/bin/pprof -pdf /tmp/prof.out 
3. killall -12 loris_framework


## Add doxygen
find ./src -name *.[ch] |xargs python3 add_doxygen_comment.py 

## code generator
1. cd code_generator
2. python3 code_generator.py idl/mod_lang_vm_I.idl
3. copy directory "target/orig/1.0.0/mod_lang_vm" into "src/"


## COPYRIGHT
GNU General Public License, version 3

