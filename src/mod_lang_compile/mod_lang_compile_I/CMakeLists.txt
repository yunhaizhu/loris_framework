add_library(mod_lang_compile_I SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_compile_I.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_compile_I_bundle.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/ast_code.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/ast_compile.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/lang_ast.c
        )

target_include_directories(mod_lang_compile_I PUBLIC
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include)

target_link_libraries(mod_lang_compile_I PUBLIC pthread ${LINK_PROFILER})

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/private/script/mod_lang_compile_I_test.nl
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/script/)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/public/desc.json
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_compile_I/)

INSTALL(TARGETS mod_lang_compile_I
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_compile_I/)
