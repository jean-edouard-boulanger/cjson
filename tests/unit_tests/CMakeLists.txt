if(BUILD_TESTS)
    enable_testing()
    find_package(Check REQUIRED)
    add_executable(unit_tests
            main.c
            helpers.c
            test_str.c
            test_reader.c
            test_object.c
            test_string_stream.c)
    target_include_directories(unit_tests PRIVATE ${CHECK_INCLUDE_DIRS})
    target_link_directories(unit_tests PRIVATE ${CHECK_LIBRARY_DIRS})
    target_link_libraries(unit_tests cjson ${CHECK_LIBRARIES})
endif()
