add_library(mod_thread_pool_I SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_thread_pool_I.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_thread_pool_I_bundle.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/stub/stub_shell_mod_thread_pool_I.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../public/src/thpool.o
        )

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../public/src/thpool.o
        COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/../public/src
)

target_include_directories(mod_thread_pool_I PRIVATE
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include)

target_link_libraries(mod_thread_pool_I PRIVATE pthread ${LINK_PROFILER})

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/public/desc.json
        DESTINATION ${loris_framework_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_thread_pool_I/)

INSTALL(TARGETS mod_thread_pool_I
        DESTINATION ${loris_framework_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_thread_pool_I/)

