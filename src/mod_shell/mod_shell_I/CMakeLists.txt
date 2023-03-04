add_library(mod_shell_I SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_shell_I.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_shell_I_bundle.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/shell.c
        )

target_include_directories(mod_shell_I PRIVATE
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src)

target_link_libraries(mod_shell_I PRIVATE gsl gslcblas m)

INSTALL(DIRECTORY private/script/
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/script/)

INSTALL(FILES public/desc.json
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_shell_I/)

INSTALL(TARGETS mod_shell_I
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_shell_I/)


add_library(shell_lib SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/private/script/shell_lib.c
        )

target_include_directories(shell_lib PRIVATE
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/src/mod_lang_vm/mod_lang_vm_I/private/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include)

target_link_libraries(shell_lib PRIVATE pthread ${LINK_PROFILER} mod_lang_vm_I mod_shell_I)

INSTALL(TARGETS shell_lib
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/script/)