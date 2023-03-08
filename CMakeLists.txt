cmake_minimum_required(VERSION 3.16)
set(PROJECT_CUSTOM_NAME "loris_framework")
project(${PROJECT_CUSTOM_NAME} C)

set(CMAKE_C_COMPILER "clang")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(" ")
    message("CMAKE IN DEBUG MODE")
#    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -O0 -D_USE_JIT_ -DSOFT_FPU -g -Wall -D_DEBUG -Werror -fPIC -fno-stack-protector -fno-common -march=native")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-enum-compare -DDEBUG_JIT -fsanitize=address -fno-omit-frame-pointer")
    message(" ")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message(" ")
    message("CMAKE IN RELEASE MODE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -D_USE_JIT_ -DSOFT_FPU -g -Wall -DNDEBUG -D_DEBUG -Werror -fPIC -fno-stack-protector -fno-common -march=native")
    message(" ")
endif ()

set(CMAKE_ENABLE_EXPORTS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(LINK_PROFILER "profiler")

add_subdirectory(src)
add_subdirectory(code_generator)

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/files/
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/)

