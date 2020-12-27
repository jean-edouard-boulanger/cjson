//
// Created by Jean-Edouard BOULANGER on 26/12/2020.
//

#include "test_reader.h"
#include "helpers.h"

#include <cjson_reader.h>
#include <cjson_value.h>
#include <cjson_array.h>
#include <cjson_object.h>
#include <cjson_str.h>
#include <cjson_stringstream.h>


START_GOOD_READ_TEST(test_good_lonely_null,
    RAW_JSON(null),
    CJSON_NULL_V
)
START_GOOD_READ_TEST(test_good_lonely_true,
    RAW_JSON(true),
    CJSON_TRUE_V
)
START_GOOD_READ_TEST(test_good_lonely_false,
    RAW_JSON(false),
    CJSON_FALSE_V
)
START_GOOD_READ_TEST(test_good_lonely_positive_integer,
    RAW_JSON(42),
    CJSON_NUMBER_V(42)
)
START_GOOD_READ_TEST(test_good_lonely_negative_integer,
    RAW_JSON(-42),
    CJSON_NUMBER_V(-42)
)
START_GOOD_READ_TEST(test_good_lonely_float_number,
    RAW_JSON(42.0),
    CJSON_NUMBER_V(42.0)
)
START_GOOD_READ_TEST(test_good_lonely_negative_float_number,
    RAW_JSON(-42.0),
    CJSON_NUMBER_V(-42.0)
)
START_GOOD_READ_TEST(test_good_lonely_number_exponent,
    RAW_JSON(42.0e2),
    CJSON_NUMBER_V(4200.0)
)
START_GOOD_READ_TEST(test_good_lonely_number_plus_exponent,
    RAW_JSON(42.0e+2),
    CJSON_NUMBER_V(4200.0)
)
START_GOOD_READ_TEST(test_good_lonely_number_minus_exponent,
    RAW_JSON(42.0e-2),
    CJSON_NUMBER_V(0.42)
)
START_GOOD_READ_TEST(test_good_lonely_empty_string,
    RAW_JSON(""),
    CJSON_STR_V("")
)
START_GOOD_READ_TEST(test_good_lonely_string,
    RAW_JSON("hello"),
    CJSON_STR_V("hello")
)
START_GOOD_READ_TEST(test_good_empty_array,
    RAW_JSON([]),
    CJSON_EMPTY_ARRAY_V
)
START_GOOD_READ_TEST(test_good_empty_array_with_spaces,
    RAW_JSON([  ]),
    CJSON_EMPTY_ARRAY_V
)
START_GOOD_READ_TEST(test_good_array_with_lonely_true,
    RAW_JSON([true]),
    CJSON_ARRAY_V(CJSON_TRUE_V)
)
START_GOOD_READ_TEST(test_good_array_with_lonely_false,
    RAW_JSON([false]),
    CJSON_ARRAY_V(CJSON_FALSE_V)
)
START_GOOD_READ_TEST(test_good_array_with_lonely_null,
    RAW_JSON([null]),
    CJSON_ARRAY_V(CJSON_NULL_V)
)
START_GOOD_READ_TEST(test_good_array_of_mixed_primitives_only,
    RAW_JSON([
        -10.0,
        "array",
        true,
        null,
        false
    ]),
    CJSON_ARRAY_V(
        CJSON_NUMBER_V(-10.0),
        CJSON_STR_V("array"),
        CJSON_TRUE_V,
        CJSON_NULL_V,
        CJSON_FALSE_V
    )
)
START_GOOD_READ_TEST(test_good_array_of_mixed_items,
    RAW_JSON([
        null,
        1,
        "1",
        {},
        []
    ]),
    CJSON_ARRAY_V(
        CJSON_NULL_V,
        CJSON_NUMBER_V(1),
        CJSON_STR_V("1"),
        CJSON_EMPTY_OBJECT_V,
        CJSON_EMPTY_ARRAY_V
    )
)
START_GOOD_READ_TEST(test_good_nested_arrays,
    RAW_JSON([[[[[]]]]]),
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
START_GOOD_READ_TEST(test_good_empty_object,
    RAW_JSON({}),
    CJSON_EMPTY_OBJECT_V
)
START_GOOD_READ_TEST(test_good_empty_object_with_spaces,
    RAW_JSON({     }),
    CJSON_EMPTY_OBJECT_V
)
START_GOOD_READ_TEST(test_good_object_with_string_values,
    RAW_JSON({
        "key1": "value1",
        "key2": "value2"
    }),
    CJSON_OBJECT_V(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2")
    )
)
START_GOOD_READ_TEST(test_good_empty_object_with_duplicated_keys,
    RAW_JSON({
        "key1": "value1",
        "key1": "value2"
    }),
    CJSON_OBJECT_V(
        "key1", CJSON_STR_V("value2")
    )
)
START_GOOD_READ_TEST(test_good_empty_object_with_empty_key,
    RAW_JSON({
        "": "value1"
    }),
    CJSON_OBJECT_V(
        "", CJSON_STR_V("value1")
    )
)
START_GOOD_READ_TEST(test_good_complex_example1,
    RAW_JSON({
        "glossary": {
            "title": "example glossary",
            "GlossDiv": {
                "title": "S",
                "GlossList": {
                    "GlossEntry": {
                        "ID": "SGML",
                        "SortAs": "SGML",
                        "GlossTerm": "Standard Generalized Markup Language",
                        "Acronym": "SGML",
                        "Abbrev": "ISO 8879:1986",
                        "GlossDef": {
                            "para": "A meta-markup language, used to create markup languages such as DocBook.",
                            "GlossSeeAlso": [
                                "GML",
                                "XML"
                            ]
                        },
                        "GlossSee": "markup"
                    }
                }
            }
        }
    }),
    CJSON_OBJECT_V(
        "glossary", CJSON_OBJECT_V(
            "title", CJSON_STR_V("example glossary"),
            "GlossDiv", CJSON_OBJECT_V(
                "title", CJSON_STR_V("S"),
                "GlossList", CJSON_OBJECT_V(
                    "GlossEntry", CJSON_OBJECT_V(
                        "ID", CJSON_STR_V("SGML"),
                        "SortAs", CJSON_STR_V("SGML"),
                        "GlossTerm", CJSON_STR_V("Standard Generalized Markup Language"),
                        "Acronym", CJSON_STR_V("SGML"),
                        "Abbrev", CJSON_STR_V("ISO 8879:1986"),
                        "GlossDef", CJSON_OBJECT_V(
                            "para", CJSON_STR_V("A meta-markup language, used to create markup languages such as DocBook."),
                            "GlossSeeAlso", CJSON_ARRAY_V(
                                CJSON_STR_V("GML"),
                                CJSON_STR_V("XML")
                            )
                        ),
                        "GlossSee", CJSON_STR_V("markup")
                    )
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
    tcase_add_test(reader_case, test_good_lonely_negative_integer);
    tcase_add_test(reader_case, test_good_lonely_negative_float_number);
    tcase_add_test(reader_case, test_good_lonely_number_exponent);
    tcase_add_test(reader_case, test_good_lonely_number_plus_exponent);
    tcase_add_test(reader_case, test_good_lonely_number_minus_exponent);
    tcase_add_test(reader_case, test_good_empty_array);
    tcase_add_test(reader_case, test_good_empty_array_with_spaces);
    tcase_add_test(reader_case, test_good_array_with_lonely_true);
    tcase_add_test(reader_case, test_good_array_with_lonely_false);
    tcase_add_test(reader_case, test_good_array_with_lonely_null);
    tcase_add_test(reader_case, test_good_array_of_mixed_primitives_only);
    tcase_add_test(reader_case, test_good_array_of_mixed_items);
    tcase_add_test(reader_case, test_good_nested_arrays);
    tcase_add_test(reader_case, test_good_empty_object);
    tcase_add_test(reader_case, test_good_empty_object_with_spaces);
    tcase_add_test(reader_case, test_good_object_with_string_values);
    tcase_add_test(reader_case, test_good_empty_object_with_duplicated_keys);
    tcase_add_test(reader_case, test_good_empty_object_with_empty_key);
    tcase_add_test(reader_case, test_good_empty_object_with_empty_key);
    tcase_add_test(reader_case, test_good_complex_example1);

    tcase_add_test(reader_case, test_bad_lonely_string);
    tcase_add_test(reader_case, test_array_missing_right_bracket);
    tcase_add_test(reader_case, test_array_trailing_comma);
}
