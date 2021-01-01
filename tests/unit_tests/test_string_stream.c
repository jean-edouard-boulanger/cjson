//
// Created by Jean-Edouard BOULANGER on 30/12/2020.
//

#include "cases.h"

#include <cjson_stringstream.h>
#include <cjson_allocator.h>


START_TEST(test_new) {
    CJsonStringStream* stream = cjson_string_stream_new(NULL);
    ck_assert_ptr_nonnull(stream);
    cjson_string_stream_free(stream);
}

START_TEST(test_write_null) {
    CJsonStringStream* stream = cjson_string_stream_new(NULL);
    ck_assert_ptr_nonnull(stream);

    cjson_string_stream_write(stream, NULL);
    char* str = cjson_string_stream_str(stream);
    ck_assert_str_eq(str, "");

    cjson_string_stream_free(stream);
    cjson_dealloc(NULL, str);
}

START_TEST(test_write_empty) {
    CJsonStringStream* stream = cjson_string_stream_new(NULL);
    ck_assert_ptr_nonnull(stream);

    cjson_string_stream_write(stream, "");
    char* str = cjson_string_stream_str(stream);
    ck_assert_str_eq(str, "");

    cjson_string_stream_free(stream);
    cjson_dealloc(NULL, str);
}

START_TEST(test_write) {
    CJsonStringStream* stream = cjson_string_stream_new(NULL);
    ck_assert_ptr_nonnull(stream);

    cjson_string_stream_write(stream, "the answer is");
    char* str = cjson_string_stream_str(stream);
    ck_assert_str_eq(str, "the answer is");
    cjson_dealloc(NULL, str);

    cjson_string_stream_write(stream, " 42");
    str = cjson_string_stream_str(stream);
    ck_assert_str_eq(str, "the answer is 42");
    cjson_dealloc(NULL, str);

    cjson_string_stream_free(stream);
}

START_TEST(test_write_bytes) {
    CJsonStringStream* stream = cjson_string_stream_new(NULL);
    ck_assert_ptr_nonnull(stream);

    cjson_string_stream_write_bytes(stream, "42 is the answer", 2);
    char* str = cjson_string_stream_str(stream);
    ck_assert_str_eq(str, "42");
    cjson_dealloc(NULL, str);

    cjson_string_stream_free(stream);
}

START_TEST(test_write_double) {
    CJsonStringStream* stream = cjson_string_stream_new(NULL);
    ck_assert_ptr_nonnull(stream);

    const double value = 42.42;
    cjson_string_stream_write_double(stream, &value);
    char* str = cjson_string_stream_str(stream);
    ck_assert_str_eq(str, "42.42000000");
    cjson_dealloc(NULL, str);

    cjson_string_stream_free(stream);
}

void string_stream_case_setup(Suite* suite) {
    TCase* string_stream_case = tcase_create("string_stream");
    suite_add_tcase(suite, string_stream_case);

    tcase_add_test(string_stream_case, test_new);
    tcase_add_test(string_stream_case, test_write_null);
    tcase_add_test(string_stream_case, test_write_empty);
    tcase_add_test(string_stream_case, test_write);
    tcase_add_test(string_stream_case, test_write_bytes);
    tcase_add_test(string_stream_case, test_write_double);
}
