#include <cjson_stringstream.h>
#include <cjson_value.h>

#define ABORT_SIGNAL 6

#define tcase_add_test_abort(x, y) tcase_add_test_raise_signal(x, y, ABORT_SIGNAL)

#define ck_assert_not(x) ck_assert(!(x))

#define START_GOOD_READ_TEST(name, input_value, expected_value) \
    START_TEST(name) { \
        CJsonValue* expected = expected_value; \
        CJsonValue* actual = cjson_read(input_value, NULL); \
        ck_assert_ptr_nonnull(expected); \
        ck_assert_ptr_nonnull(actual); \
        if(actual == NULL) { \
            ck_abort_msg("failed reading input value: '%s'", input_value); \
        } \
        if(!cjson_value_equals(actual, expected)) { \
            CJsonStringStream* stream = cjson_string_stream_new(NULL); \
            cjson_string_stream_write(stream, "expected: "); \
            cjson_value_fmt(stream, expected); \
            cjson_string_stream_write(stream, " got: "); \
            cjson_value_fmt(stream, actual); \
            ck_abort_msg("%s", cjson_string_stream_str(stream)); \
        } \
        cjson_value_free(expected); \
        cjson_value_free(actual); \
    }

#define START_BAD_READ_TEST(name, input_value) \
    START_TEST(name) { \
        CJsonValue* result = cjson_read(input_value, NULL); \
        if(result != NULL) { \
            CJsonStringStream* stream = cjson_string_stream_new(NULL); \
            cjson_string_stream_write(stream, "expected read failure, instead got: "); \
            cjson_value_fmt(stream, result); \
            ck_abort_msg("%s", cjson_string_stream_str(stream)); \
        } \
    }

#define RAW_JSON(...) #__VA_ARGS__
