add_library(mod_lang_parse_I SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_parse_I.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_parse_I_bundle.c

        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/lang_lex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/lang_parse.c
        )

target_include_directories(mod_lang_parse_I PUBLIC
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include)

target_link_libraries(mod_lang_parse_I PUBLIC pthread ${LINK_PROFILER} mod_lang_compile_I)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/private/script/mod_lang_parse_I_test.nl
        DESTINATION ${loris_framework_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/script/)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/public/desc.json
        DESTINATION ${loris_framework_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_parse_I/)

INSTALL(TARGETS mod_lang_parse_I
        DESTINATION ${loris_framework_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_parse_I/)
