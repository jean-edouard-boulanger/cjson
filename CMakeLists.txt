cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(cjson VERSION 0.0.1)

enable_language(C)

add_subdirectory(libcjson)
add_subdirectory(tests)
