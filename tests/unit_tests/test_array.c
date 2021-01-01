//
// Created by Jean-Edouard BOULANGER on 01/01/2021.
//

#include "cases.h"
#include "helpers.h"

#include <cjson_array.h>
#include <cjson_value.h>


START_TEST(test_new) {
    CJsonArray* array = cjson_array_new(NULL);
    ck_assert_ptr_nonnull(array);
    ck_assert_int_eq(cjson_array_size(array), 0);

    cjson_array_free(array);
}

START_TEST(test_copy) {
    CJsonArray* array0 = cjson_array_new(NULL);
    ck_assert_ptr_nonnull(array0);
    ck_assert_int_eq(cjson_array_size(array0), 0);

    CJsonArray* array1 = cjson_array_copy(array0);
    ck_assert_ptr_nonnull(array0);
    ck_assert_ptr_ne(array0, array1);

    cjson_array_push(array0, CJSON_NULL_V);
    cjson_array_push(array0, CJSON_NULL_V);
    ck_assert_int_eq(cjson_array_size(array0), 2);
    ck_assert_int_eq(cjson_array_size(array1), 0);

    CJsonArray* array2 = cjson_array_copy(array0);
    ck_assert_ptr_nonnull(array2);
    ck_assert_ptr_ne(array0, array2);
    ck_assert_int_eq(cjson_array_size(array0), cjson_array_size(array2));

    cjson_array_free(array0);
    cjson_array_free(array1);
    cjson_array_free(array2);
}

START_TEST(test_size) {
    CJsonArray* array = cjson_array_new(NULL);
    ck_assert_ptr_nonnull(array);
    ck_assert_int_eq(cjson_array_size(array), 0);

    cjson_array_push(array, CJSON_NULL_V);
    ck_assert_int_eq(cjson_array_size(array), 1);

    cjson_array_push(array, CJSON_NULL_V);
    cjson_array_push(array, CJSON_NULL_V);
    ck_assert_int_eq(cjson_array_size(array), 3);

    cjson_array_pop(array);
    ck_assert_int_eq(cjson_array_size(array), 2);

    cjson_array_free(array);
}

START_TEST(test_empty) {
    CJsonArray* array = cjson_array_new(NULL);
    ck_assert_ptr_nonnull(array);
    ck_assert(cjson_array_empty(array));

    cjson_array_push(array, CJSON_NULL_V);
    ck_assert_not(cjson_array_empty(array));

    cjson_array_free(array);
}

START_TEST(test_reserve) {
    CJsonArray* array = cjson_array_new(NULL);
    ck_assert_ptr_nonnull(array);

    cjson_array_reserve(array, 100);
    ck_assert_int_eq(cjson_array_capacity(array), 100);

    cjson_array_free(array);
}

START_TEST(test_at) {
    CJsonArray* array = cjson_array_new(NULL);
    ck_assert_ptr_nonnull(array);

    CJsonValue* val0 = CJSON_NULL_V;
    cjson_array_push(array, val0);
    CJsonValue* val1 = cjson_array_at(array, 0);
    ck_assert_ptr_eq(val0, val1);

    cjson_array_free(array);
}

START_TEST(test_at_out_of_bounds) {
    CJsonArray* array = cjson_array_new(NULL);
    ck_assert_ptr_nonnull(array);
    cjson_array_at(array, 0);
}

void array_case_setup(Suite* suite) {
    TCase* array_case = tcase_create("array");
    suite_add_tcase(suite, array_case);

    tcase_add_test(array_case, test_new);
    tcase_add_test(array_case, test_copy);
    tcase_add_test(array_case, test_size);
    tcase_add_test(array_case, test_empty);
    tcase_add_test(array_case, test_reserve);
    tcase_add_test(array_case, test_at);

    TCase* array_bad_case = tcase_create("array_bad");
    suite_add_tcase(suite, array_bad_case);
    tcase_add_test_abort(array_bad_case, test_at_out_of_bounds);
}