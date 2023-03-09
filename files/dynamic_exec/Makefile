all:
	gcc -g -fPIC -c src/execute_compiled_code_${arg1}.c -o obj/execute_compiled_code_${arg1}.o -I ../../../src/mod_lang_vm/mod_lang_vm_I/private/include/ -I ../../../src/mod_lang_vm/mod_lang_vm_I/private/src/  -I ../../../src/framework/public/include/
	gcc -shared -o lib/lib_${arg1}-${arg2}.so obj/execute_compiled_code_${arg1}.o -lc -ldl -lm -L ../bundle/mod_lang_vm_I -lmod_lang_vm_I
