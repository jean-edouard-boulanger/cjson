cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_C_FLAGS "-std=c11 -Werror -Wall -O3")
add_compile_definitions(CJSON_ENABLE_SAFE_ASSERTS)
add_compile_definitions(CJSON_ENABLE_STACKTRACE)

project(cjson VERSION 0.0.1)

enable_language(C)

option(BUILD_TESTS "Build the unit tests" ON)

add_subdirectory(libcjson)
add_subdirectory(tests)
