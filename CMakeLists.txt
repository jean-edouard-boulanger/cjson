cmake_minimum_required(VERSION 3.10)

project(cjson VERSION 0.0.1)

enable_language(C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_C_FLAGS "-std=c11 -Werror -Wall")
set(CMAKE_C_FLAGS_DEBUG "-g3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(CJSON_ENABLE_SAFE_ASSERTS)
  add_compile_definitions(CJSON_ENABLE_STACKTRACE)
endif()

option(BUILD_TESTS "Build the unit tests" ON)

add_subdirectory(libcjson)
add_subdirectory(tests)
