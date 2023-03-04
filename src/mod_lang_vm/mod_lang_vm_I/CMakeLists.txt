add_library(mod_lang_vm_I SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_vm_I.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_vm_I_bundle.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_object.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_library.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_safe_var.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_var_type.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_tuple_type.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_array_type.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_hash_type.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_READ.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_EXEC_CODEGEN.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/virtual_machine_EXEC_CODE.c
        )

target_include_directories(mod_lang_vm_I PRIVATE
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/src/mod_shell/mod_shell_I/private/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include)

target_link_libraries(mod_lang_vm_I PRIVATE pthread ${LINK_PROFILER})

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/private/script/mod_lang_vm_I_test.nl
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/script/)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/public/desc.json
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_vm_I/)

INSTALL(TARGETS mod_lang_vm_I
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_vm_I/)
