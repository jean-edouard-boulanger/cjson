//
// Created by Jean-Edouard BOULANGER on 26/12/2020.
//

#include "test_str.h"
#include "helpers.h"

#include <cjson_str.h>
#include <cjson_stringstream.h>

#include <string.h>


START_TEST(test_new) {
    CJsonStr* s = cjson_str_new();
    ck_assert_ptr_nonnull(s);
    ck_assert_int_eq(cjson_str_length(s), 0);
    ck_assert_str_eq(cjson_str_raw(s), "");

    cjson_str_free(s);
}

START_TEST(test_new_from_raw) {
    const char* const raw_str = "hello";
    CJsonStr* s = cjson_str_new_from_raw(raw_str);
    ck_assert_ptr_nonnull(s);
    ck_assert_str_eq(raw_str, cjson_str_raw(s));
    ck_assert_int_eq(strlen(raw_str), cjson_str_length(s));

    cjson_str_free(s);
}

START_TEST(test_new_of_size) {
    const char* const expected = "hhhh";
    CJsonStr* s = cjson_str_new_of_size(strlen(expected), expected[0]);
    ck_assert_ptr_nonnull(s);
    ck_assert_str_eq(expected, cjson_str_raw(s));
    ck_assert_int_eq(cjson_str_length(s), strlen(expected));

    cjson_str_free(s);
}

START_TEST(test_copy) {
    CJsonStr* s1 = cjson_str_new_from_raw("test_copy");
    ck_assert_ptr_nonnull(s1);
    CJsonStr* s2 = cjson_str_copy(s1);
    ck_assert_ptr_nonnull(s2);
    ck_assert_str_eq(cjson_str_raw(s1), cjson_str_raw(s2));
    ck_assert_int_eq(cjson_str_length(s1), cjson_str_length(s2));
    ck_assert_int_eq(cjson_str_length(s1), strlen(cjson_str_raw(s1)));

    cjson_str_free(s1);
    cjson_str_free(s2);
}

START_TEST(test_clear) {
    CJsonStr* s = cjson_str_new_from_raw("test_clear");
    ck_assert_ptr_nonnull(s);
    cjson_str_clear(s);
    ck_assert_int_eq(cjson_str_length(s), 0);
    ck_assert_str_eq(cjson_str_raw(s), "");

    cjson_str_free(s);
}

START_TEST(test_append_raw_string) {
    CJsonStr* s = cjson_str_new_from_raw("hello");
    cjson_str_append_raw_string(s, " world");
    ck_assert_str_eq("hello world", cjson_str_raw(s));

    cjson_str_free(s);
}

START_TEST(test_append) {
    CJsonStr* s1 = cjson_str_new_from_raw("hello");
    CJsonStr* s2 = cjson_str_new_from_raw(" world");
    ck_assert_ptr_nonnull(s1);
    ck_assert_ptr_nonnull(s2);

    cjson_str_append(s1, s2);
    ck_assert_str_eq("hello world", cjson_str_raw(s1));
    ck_assert_str_eq(" world", cjson_str_raw(s2));

    cjson_str_free(s1);
    cjson_str_free(s2);
}

START_TEST(test_pop_back) {
    CJsonStr* s = cjson_str_new_from_raw("hello");
    ck_assert_ptr_nonnull(s);
    cjson_str_pop_back(s);
    ck_assert_str_eq("hell", cjson_str_raw(s));

    cjson_str_free(s);
}

START_TEST(test_pop_back_empty_string) {
    CJsonStr* s = cjson_str_new_from_raw("");
    cjson_str_pop_back(s);
}

START_TEST(test_substr) {
    CJsonStr* s0 = cjson_str_new_from_raw("hello world");
    ck_assert_ptr_nonnull(s0);

    CJsonStr* s1 = cjson_str_substr(s0, 0, 5);
    ck_assert_ptr_nonnull(s1);
    ck_assert_str_eq("hello", cjson_str_raw(s1));
    cjson_str_free(s1);

    s1 = cjson_str_substr(s0, 6, 11);
    ck_assert_ptr_nonnull(s1);
    ck_assert_str_eq("world", cjson_str_raw(s1));
    cjson_str_free(s1);
}

START_TEST(test_substr_out_of_bounds) {
    CJsonStr* s0 = cjson_str_new_from_raw("hello world");
    ck_assert_ptr_nonnull(s0);
    cjson_str_substr(s0, 100, 200);
}

START_TEST(test_concat) {
    CJsonStr* s1 = cjson_str_new_from_raw("hello ");
    CJsonStr* s2 = cjson_str_new_from_raw("world");
    ck_assert_ptr_nonnull(s1);
    ck_assert_ptr_nonnull(s2);

    CJsonStr* s3 = cjson_str_concat(s1, s2);
    ck_assert_ptr_nonnull(s3);
    ck_assert_str_eq("hello world", cjson_str_raw(s3));

    cjson_str_free(s1);
    cjson_str_free(s2);
    cjson_str_free(s3);
}

START_TEST(test_at) {
    CJsonStr* s = cjson_str_new_from_raw("world");
    ck_assert(cjson_str_at(s, 0) == 'w');
    ck_assert(cjson_str_at(s, 1) == 'o');
    ck_assert(cjson_str_at(s, 4) == 'd');

    cjson_str_free(s);
}

START_TEST(test_at_out_of_bounds) {
    CJsonStr* s = cjson_str_new_from_raw("world");
    cjson_str_at(s, 100);
}

START_TEST(test_front) {
    CJsonStr* s = cjson_str_new_from_raw("world");
    ck_assert(cjson_str_front(s) == 'w');

    cjson_str_free(s);
}

START_TEST(test_front_empty_string) {
    CJsonStr* s = cjson_str_new_from_raw("");
    cjson_str_front(s);
}

START_TEST(test_back) {
    CJsonStr* s = cjson_str_new_from_raw("world");
    ck_assert(cjson_str_back(s) == 'd');

    cjson_str_free(s);
}

START_TEST(test_back_empty_string) {
    CJsonStr* s = cjson_str_new_from_raw("");
    ck_assert_ptr_nonnull(s);
    cjson_str_back(s);
}

START_TEST(test_cmp) {
    CJsonStr* s1 = cjson_str_new_from_raw("world");
    CJsonStr* s2 = cjson_str_new_from_raw("world");
    CJsonStr* s3 = cjson_str_new_from_raw("hello");
    ck_assert_ptr_nonnull(s1);
    ck_assert_ptr_nonnull(s2);
    ck_assert_ptr_nonnull(s3);
    ck_assert(cjson_str_cmp(s1, s2) == cjson_ordering_equal);
    ck_assert(cjson_str_cmp(s1, s1) == cjson_ordering_equal);
    ck_assert(cjson_str_cmp(s1, s3) == cjson_ordering_greater);
    ck_assert(cjson_str_cmp(s3, s2) == cjson_ordering_less);

    cjson_str_free(s1);
    cjson_str_free(s2);
    cjson_str_free(s3);
}

START_TEST(test_raw_str_cmp) {
    ck_assert(cjson_raw_str_cmp("hello", "hello") == cjson_ordering_equal);
    ck_assert(cjson_raw_str_cmp("hello", "world") == cjson_ordering_less);
    ck_assert(cjson_raw_str_cmp("world", "hello") == cjson_ordering_greater);
}

START_TEST(test_equals) {
    CJsonStr* s1 = cjson_str_new_from_raw("hello");
    CJsonStr* s2 = cjson_str_new_from_raw("hello");
    CJsonStr* s3 = cjson_str_new_from_raw("world");

    ck_assert_ptr_nonnull(s1);
    ck_assert_ptr_nonnull(s2);
    ck_assert_ptr_nonnull(s3);

    ck_assert_not(cjson_str_equals(s2, s3));
    ck_assert(cjson_str_equals(s1, s1));
    ck_assert(cjson_str_equals(s1, s2));

    cjson_str_free(s1);
    cjson_str_free(s2);
    cjson_str_free(s3);
}

START_TEST(test_equals_raw) {
    CJsonStr* s1 = cjson_str_new_from_raw("hello");
    ck_assert_ptr_nonnull(s1);
    ck_assert(cjson_str_equals_raw(s1, "hello"));
    ck_assert_not(cjson_str_equals_raw(s1, "world"));

    cjson_str_free(s1);
}

START_TEST(test_raw_str_equals) {
    ck_assert(cjson_raw_str_equals("hello", "hello"));
    ck_assert_not(cjson_raw_str_equals("hello", "world"));
}

START_TEST(test_fmt) {
    CJsonStr* s = cjson_str_new_from_raw("world");
    ck_assert_ptr_nonnull(s);
    CJsonStringStream* stream = cjson_string_stream_new();
    cjson_str_fmt(stream, s);
    char* buff = cjson_string_stream_str(stream);
    ck_assert_str_eq(buff, "\"world\"");

    free(buff);
    cjson_str_free(s);
    cjson_string_stream_free(stream);
}

START_TEST(test_raw_str_fmt) {
    CJsonStringStream* stream = cjson_string_stream_new();
    cjson_raw_str_fmt(stream, "world");
    char* buff = cjson_string_stream_str(stream);
    ck_assert_str_eq(buff, "\"world\"");

    free(buff);
    cjson_string_stream_free(stream);
}

void str_case_setup(Suite* suite) {
    TCase* str_case = tcase_create("str");
    suite_add_tcase(suite, str_case);

    tcase_add_test(str_case, test_new);
    tcase_add_test(str_case, test_new_from_raw);
    tcase_add_test(str_case, test_new_of_size);
    tcase_add_test(str_case, test_copy);
    tcase_add_test(str_case, test_clear);
    tcase_add_test(str_case, test_append_raw_string);
    tcase_add_test(str_case, test_append);
    tcase_add_test(str_case, test_pop_back);
    tcase_add_test(str_case, test_substr);
    tcase_add_test(str_case, test_concat);
    tcase_add_test(str_case, test_at);
    tcase_add_test(str_case, test_front);
    tcase_add_test(str_case, test_back);
    tcase_add_test(str_case, test_cmp);
    tcase_add_test(str_case, test_raw_str_cmp);
    tcase_add_test(str_case, test_equals);
    tcase_add_test(str_case, test_equals_raw);
    tcase_add_test(str_case, test_raw_str_equals);
    tcase_add_test(str_case, test_fmt);
    tcase_add_test(str_case, test_raw_str_fmt);

    tcase_add_test_abort(str_case, test_at_out_of_bounds);
    tcase_add_test_abort(str_case, test_substr_out_of_bounds);
    tcase_add_test_abort(str_case, test_pop_back_empty_string);
    tcase_add_test_abort(str_case, test_front_empty_string);
    tcase_add_test_abort(str_case, test_back_empty_string);
}
