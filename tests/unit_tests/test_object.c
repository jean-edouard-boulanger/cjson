//
// Created by Jean-Edouard BOULANGER on 27/12/2020.
//

#include "helpers.h"
#include "test_object.h"

#include <cjson_object.h>
#include <cjson_value.h>
#include <cjson_str.h>


START_TEST(test_new) {
    CJsonObject* obj = cjson_object_new();
    ck_assert_ptr_nonnull(obj);
    ck_assert_int_eq(cjson_object_size(obj), 0);

    cjson_object_free(obj);
}

START_TEST(test_new_dsl) {
    CJsonObject* obj = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_TRUE_V
    );
    ck_assert_ptr_nonnull(obj);
    ck_assert_int_eq(cjson_object_size(obj), 2);

    ck_assert(cjson_object_has(obj, "key1"));
    CJsonValue* val1 = cjson_object_get(obj, "key1");
    ck_assert(cjson_value_is_str(val1));
    ck_assert_str_eq(cjson_str_raw(CJSON_AS_STR(val1)), "value1");

    ck_assert(cjson_object_has(obj, "key2"));
    CJsonValue* val2 = cjson_object_get(obj, "key2");
    ck_assert(cjson_value_is_bool(val2));
    ck_assert(*CJSON_AS_BOOL(val2) == true);

    cjson_object_free(obj);
}

START_TEST(test_set_get) {
    CJsonObject* obj = cjson_object_new();
    ck_assert_ptr_nonnull(obj);

    CJsonValue* val0 = cjson_object_get(obj, "missing");
    ck_assert(val0 == NULL);

    CJsonValue* val1 = CJSON_STR_V("world");
    cjson_object_set(obj, "hello", val1);

    ck_assert_int_eq(cjson_object_size(obj), 1);
    CJsonValue* val1_ref = cjson_object_get(obj, "hello");
    ck_assert_ptr_nonnull(val1_ref);
    ck_assert(val1 == val1_ref);

    cjson_object_free(obj);
}

START_TEST(test_set_get_overwrite) {
    CJsonObject* obj = cjson_object_new();
    ck_assert_ptr_nonnull(obj);

    CJsonValue* val1 = CJSON_STR_V("json");
    CJsonValue* val2 = CJSON_STR_V("yaml");

    cjson_object_set(obj, "key", val1);
    cjson_object_set(obj, "key", val2);

    ck_assert(cjson_object_get(obj, "key") != val1);
    ck_assert(cjson_object_get(obj, "key") == val2);

    cjson_object_free(obj);
}

START_TEST(test_has) {
    CJsonObject* obj = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2")
    );

    ck_assert_ptr_nonnull(obj);
    ck_assert_not(cjson_object_has(obj, "key3"));
    ck_assert(cjson_object_has(obj, "key1"));
    ck_assert(cjson_object_has(obj, "key2"));

    cjson_object_free(obj);
}

START_TEST(test_size) {
    CJsonObject* obj = cjson_object_new();
    ck_assert_ptr_nonnull(obj);
    ck_assert_int_eq(cjson_object_size(obj), 0);

    cjson_object_set(obj, "key1", CJSON_STR_V("val"));
    ck_assert_int_eq(cjson_object_size(obj), 1);

    cjson_object_set(obj, "key2", CJSON_STR_V("val"));
    ck_assert_int_eq(cjson_object_size(obj), 2);

    cjson_object_set(obj, "key2", CJSON_STR_V("val"));
    ck_assert_int_eq(cjson_object_size(obj), 2);

    cjson_object_set(obj, "key3", CJSON_STR_V("val"));
    ck_assert_int_eq(cjson_object_size(obj), 3);

    cjson_object_free(obj);
}

START_TEST(test_del) {
    CJsonObject* obj = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2")
    );
    ck_assert_ptr_nonnull(obj);

    cjson_object_del(obj, "key1");
    ck_assert_not(cjson_object_has(obj, "key1"));
    ck_assert(cjson_object_has(obj, "key2"));

    cjson_object_del(obj, "key3");
    ck_assert(cjson_object_has(obj, "key2"));

    cjson_object_free(obj);
}

START_TEST(test_equals) {
    CJsonObject* obj1 = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2")
    );
    ck_assert_ptr_nonnull(obj1);

    CJsonObject* obj2 = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2")
    );
    ck_assert_ptr_nonnull(obj2);

    ck_assert(cjson_object_equals(obj1, obj2));

    cjson_object_free(obj1);
    cjson_object_free(obj2);
}

START_TEST(test_not_equals_extra_key) {
    CJsonObject* obj1 = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2")
    );
    ck_assert_ptr_nonnull(obj1);

    CJsonObject* obj2 = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2"),
        "key3", CJSON_STR_V("value3")
    );
    ck_assert_ptr_nonnull(obj2);

    ck_assert_not(cjson_object_equals(obj1, obj2));

    cjson_object_free(obj1);
    cjson_object_free(obj2);
}

START_TEST(test_foreach) {
    CJsonObject* obj = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2"),
        "key3", CJSON_STR_V("value3")
    );
    ck_assert_ptr_nonnull(obj);

    CJsonStringStream* stream = cjson_string_stream_new();
    ck_assert_ptr_nonnull(stream);

    size_t loops = 0;
    CJSON_OBJECT_FOREACH(obj, it) {
        ck_assert_ptr_nonnull(it);
        ck_assert(!cjson_object_iter_is_end(it));

        const char* key = cjson_object_iter_get_key(it);
        ck_assert_ptr_nonnull(key);
        ck_assert(cjson_object_has(obj, key));

        CJsonValue* value = cjson_object_iter_get_value(it);
        ck_assert_ptr_nonnull(value);

        cjson_string_stream_write(stream, key);
        cjson_string_stream_write(stream, "=");
        cjson_string_stream_write(stream, CJSON_AS_RAW_STR(value));
        cjson_string_stream_write(stream, "/");

        loops++;
    }

    ck_assert_int_eq(loops, cjson_object_size(obj));
    char* buff = cjson_string_stream_str(stream);

    ck_assert_ptr_nonnull(strstr(buff, "key1=value1/"));
    ck_assert_ptr_nonnull(strstr(buff, "key2=value2/"));
    ck_assert_ptr_nonnull(strstr(buff, "key3=value3/"));

    cjson_object_free(obj);
    free(buff);
}

START_TEST(test_foreach_item) {
    CJsonObject* obj = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2"),
        "key3", CJSON_STR_V("value3")
    );
    ck_assert_ptr_nonnull(obj);

    CJsonStringStream* stream = cjson_string_stream_new();
    ck_assert_ptr_nonnull(stream);

    size_t loops = 0;
    CJSON_OBJECT_FOREACH_ITEM(obj, key, val) {
        ck_assert_ptr_nonnull(key);
        ck_assert_ptr_nonnull(val);

        ck_assert(cjson_object_has(obj, key));

        cjson_string_stream_write(stream, key);
        cjson_string_stream_write(stream, "=");
        cjson_string_stream_write(stream, CJSON_AS_RAW_STR(val));
        cjson_string_stream_write(stream, "/");

        loops++;
    }

    ck_assert_int_eq(loops, cjson_object_size(obj));
    char* buff = cjson_string_stream_str(stream);

    ck_assert_ptr_nonnull(strstr(buff, "key1=value1/"));
    ck_assert_ptr_nonnull(strstr(buff, "key2=value2/"));
    ck_assert_ptr_nonnull(strstr(buff, "key3=value3/"));

    cjson_object_free(obj);
    free(buff);
}

START_TEST(test_foreach_key) {
    CJsonObject* obj = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2"),
        "key3", CJSON_STR_V("value3")
    );
    ck_assert_ptr_nonnull(obj);

    CJsonStringStream* stream = cjson_string_stream_new();
    ck_assert_ptr_nonnull(stream);

    size_t loops = 0;
    CJSON_OBJECT_FOREACH_KEY(obj, key) {
        ck_assert_ptr_nonnull(key);
        ck_assert(cjson_object_has(obj, key));

        cjson_string_stream_write(stream, key);
        cjson_string_stream_write(stream, "/");

        loops++;
    }

    ck_assert_int_eq(loops, cjson_object_size(obj));
    char* buff = cjson_string_stream_str(stream);

    ck_assert_ptr_nonnull(strstr(buff, "key1/"));
    ck_assert_ptr_nonnull(strstr(buff, "key2/"));
    ck_assert_ptr_nonnull(strstr(buff, "key3/"));

    cjson_object_free(obj);
    free(buff);
}

START_TEST(test_foreach_value) {
    CJsonObject* obj = CJSON_OBJECT(
        "key1", CJSON_STR_V("value1"),
        "key2", CJSON_STR_V("value2"),
        "key3", CJSON_STR_V("value3")
    );
    ck_assert_ptr_nonnull(obj);

    CJsonStringStream* stream = cjson_string_stream_new();
    ck_assert_ptr_nonnull(stream);

    size_t loops = 0;
    CJSON_OBJECT_FOREACH_VALUE(obj, value) {
        ck_assert_ptr_nonnull(value);

        cjson_string_stream_write(stream, CJSON_AS_RAW_STR(value));
        cjson_string_stream_write(stream, "/");

        loops++;
    }

    ck_assert_int_eq(loops, cjson_object_size(obj));
    char* buff = cjson_string_stream_str(stream);

    ck_assert_ptr_nonnull(strstr(buff, "value1/"));
    ck_assert_ptr_nonnull(strstr(buff, "value2/"));
    ck_assert_ptr_nonnull(strstr(buff, "value3/"));

    cjson_object_free(obj);
    free(buff);
}

void object_case_setup(Suite* suite) {
    TCase* object_case = tcase_create("object");
    suite_add_tcase(suite, object_case);

    tcase_add_test(object_case, test_new);
    tcase_add_test(object_case, test_new_dsl);
    tcase_add_test(object_case, test_size);
    tcase_add_test(object_case, test_set_get);
    tcase_add_test(object_case, test_set_get_overwrite);
    tcase_add_test(object_case, test_has);
    tcase_add_test(object_case, test_del);
    tcase_add_test(object_case, test_equals);
    tcase_add_test(object_case, test_not_equals_extra_key);
    tcase_add_test(object_case, test_foreach);
    tcase_add_test(object_case, test_foreach_item);
    tcase_add_test(object_case, test_foreach_key);
    tcase_add_test(object_case, test_foreach_value);
}
