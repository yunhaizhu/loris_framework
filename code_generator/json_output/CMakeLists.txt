

add_custom_command(
        OUTPUT lex.yy.c
        COMMAND lex ${CMAKE_CURRENT_SOURCE_DIR}/lex.l
)

add_custom_command(
        OUTPUT y.tab.c
        COMMAND yacc ${CMAKE_CURRENT_SOURCE_DIR}/yacc.y
        DEPENDS lex.yy.c
)


add_executable(json_output
        y.tab.c

        )
target_include_directories(json_output PRIVATE
        /usr/include/json-c/
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/src/framework/public/include
        )
target_link_libraries(json_output json-c pthread l)

INSTALL(TARGETS json_output
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

