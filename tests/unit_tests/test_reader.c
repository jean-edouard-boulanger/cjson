//
// Created by Jean-Edouard BOULANGER on 26/12/2020.
//

#include "test_reader.h"
#include "helpers.h"

#include <cjson_reader.h>
#include <cjson_value.h>
#include <cjson_array.h>
#include <cjson_str.h>
#include <cjson_stringstream.h>


START_GOOD_READ_TEST(test_good_lonely_null,
    "null",
    CJSON_NULL_V
)
START_GOOD_READ_TEST(test_good_lonely_true,
    "true",
    CJSON_TRUE_V
)
START_GOOD_READ_TEST(test_good_lonely_false,
    "false",
    CJSON_FALSE_V
)
START_GOOD_READ_TEST(test_good_lonely_positive_integer,
    "42",
    CJSON_NUMBER_V(42)
)
START_GOOD_READ_TEST(test_good_lonely_float_number,
    "42.0",
    CJSON_NUMBER_V(42.0)
)
START_GOOD_READ_TEST(test_good_lonely_negative_float_number,
    "-42.0",
    CJSON_NUMBER_V(-42.0)
)
START_GOOD_READ_TEST(test_good_lonely_number_exponent,
    "42.0e2",
    CJSON_NUMBER_V(4200.0)
)
START_GOOD_READ_TEST(test_good_lonely_empty_string,
    "\"\"",
    CJSON_STR_V("")
)
START_GOOD_READ_TEST(test_good_lonely_string,
    "\"hello\"",
    CJSON_STR_V("hello")
)
START_GOOD_READ_TEST(test_good_empty_array,
    " [  ] ",
    CJSON_EMPTY_ARRAY_V
)
START_GOOD_READ_TEST(test_good_array_of_mixed_primitives,
    "[-10.0, \"array\", true, null, false]",
    CJSON_ARRAY_V(
        CJSON_NUMBER_V(-10.0),
        CJSON_STR_V("array"),
        CJSON_TRUE_V,
        CJSON_NULL_V,
        CJSON_FALSE_V
    )
)
START_GOOD_READ_TEST(test_good_nested_arrays,
    "[[[[[]]]]]",
    CJSON_ARRAY_V(
        CJSON_ARRAY_V(
            CJSON_ARRAY_V(
                CJSON_ARRAY_V(
                    CJSON_EMPTY_ARRAY_V
                )
            )
        )
    )
)
START_BAD_READ_TEST(test_bad_lonely_string, "\"bad string")
START_BAD_READ_TEST(test_array_missing_right_bracket, "\"[1, 2, 3")
START_BAD_READ_TEST(test_array_trailing_comma, "\"[1, 2, 3,]")

void reader_case_setup(Suite* suite) {
    TCase* reader_case = tcase_create("reader");
    suite_add_tcase(suite, reader_case);

    tcase_add_test(reader_case, test_good_lonely_null);
    tcase_add_test(reader_case, test_good_lonely_true);
    tcase_add_test(reader_case, test_good_lonely_false);
    tcase_add_test(reader_case, test_good_lonely_string);
    tcase_add_test(reader_case, test_good_lonely_empty_string);
    tcase_add_test(reader_case, test_good_lonely_float_number);
    tcase_add_test(reader_case, test_good_lonely_positive_integer);
    tcase_add_test(reader_case, test_good_lonely_negative_float_number);
    tcase_add_test(reader_case, test_good_lonely_number_exponent);
    tcase_add_test(reader_case, test_good_empty_array);
    tcase_add_test(reader_case, test_good_array_of_mixed_primitives);
    tcase_add_test(reader_case, test_good_nested_arrays);

    tcase_add_test(reader_case, test_bad_lonely_string);
    tcase_add_test(reader_case, test_array_missing_right_bracket);
    tcase_add_test(reader_case, test_array_trailing_comma);
}
