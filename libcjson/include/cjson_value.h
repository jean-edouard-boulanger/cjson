//
//  cjson_value.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_value_h
#define cjson_value_h

#include "cjson_utils.h"

#include <stdbool.h>


typedef struct CJsonObject CJsonObject;
typedef struct CJsonArray CJsonArray;
typedef struct CJsonStr CJsonStr;
typedef struct CJsonStringStream CJsonStringStream;
typedef struct CJsonAllocator CJsonAllocator;

typedef enum {
    cjson_null_value = 0,
    cjson_object_value,
    cjson_array_value,
    cjson_str_value,
    cjson_bool_value,
    cjson_number_value
} CJsonValueType;

typedef struct CJsonValue {
    union {
        struct CJsonObject* _object;
        struct CJsonArray* _array;
        struct CJsonStr* _str;
        bool _bool;
        double _number;
    };
    CJsonValueType _type;
    CJsonAllocator* _allocator;
} CJsonValue;

CJsonValue* cjson_value_new(CJsonAllocator* allocator);
CJsonValue* cjson_value_new_as_null(CJsonAllocator* allocator);
CJsonValue* cjson_value_new_as_object(CJsonObject* object, CJsonAllocator* allocator);
CJsonValue* cjson_value_new_as_array(CJsonArray* array, CJsonAllocator* allocator);
CJsonValue* cjson_value_new_as_str(CJsonStr* str, CJsonAllocator* allocator);
CJsonValue* cjson_value_new_as_bool(bool val, CJsonAllocator* allocator);
CJsonValue* cjson_value_new_as_number(double val, CJsonAllocator* allocator);
CJsonValue* cjson_value_copy(const CJsonValue* this);
void cjson_value_free(CJsonValue* this);
void cjson_value_reset(CJsonValue* this);

bool cjson_value_is(const CJsonValue* this, CJsonValueType type);
bool cjson_value_is_null(const CJsonValue* this);
bool cjson_value_is_object(const CJsonValue* this);
bool cjson_value_is_array(const CJsonValue* this);
bool cjson_value_is_str(const CJsonValue* this);
bool cjson_value_is_bool(const CJsonValue* this);
bool cjson_value_is_number(const CJsonValue* this);

CJsonObject* cjson_value_get_object(CJsonValue* this);
CJsonArray* cjson_value_get_array(CJsonValue* this);
CJsonStr* cjson_value_get_str(CJsonValue* this);
bool* cjson_value_get_bool(CJsonValue* this);
double* cjson_value_get_number(CJsonValue* this);

void cjson_value_make_null(CJsonValue* this);
void cjson_value_make_object(CJsonValue* this, CJsonObject* object);
void cjson_value_make_array(CJsonValue* this, CJsonArray* array);
void cjson_value_make_str(CJsonValue* this, CJsonStr* str);
void cjson_value_make_bool(CJsonValue* this, bool val);
void cjson_value_make_number(CJsonValue* this, double val);

bool cjson_value_equals(const CJsonValue* this, const CJsonValue* other);

void cjson_bool_fmt(CJsonStringStream* stream, const bool* val);
void cjson_number_fmt(CJsonStringStream* stream, const double* val);
void cjson_value_fmt(CJsonStringStream* stream, const CJsonValue* this);
void cjson_null_fmt(CJsonStringStream* stream);

#define CJSON_NULL_V_A(allocator) (cjson_value_new_as_null(allocator))
#define CJSON_NULL_V CJSON_NULL_V_A(NULL)
#define CJSON_BOOL_V_A(x, allocator) (cjson_value_new_as_bool(x, allocator))
#define CJSON_BOOL_V(x) CJSON_BOOL_V_A(x, NULL)
#define CJSON_TRUE_V_A(allocator) CJSON_BOOL_V_A(true, allocator)
#define CJSON_TRUE_V CJSON_TRUE_V_A(NULL)
#define CJSON_FALSE_V_A(allocator) CJSON_BOOL_V_A(false, allocator)
#define CJSON_FALSE_V CJSON_FALSE_V_A(NULL)
#define CJSON_NUMBER_V_A(x, allocator) (cjson_value_new_as_number(x, allocator))
#define CJSON_NUMBER_V(x) CJSON_NUMBER_V_A(x, NULL)
#define CJSON_STR_V_A(x, allocator) (cjson_value_new_as_str(CJSON_STR(x), allocator))
#define CJSON_STR_V(x) CJSON_STR_V_A(x, NULL)
#define CJSON_EMPTY_ARRAY_V_A(allocator) (cjson_value_new_as_array(CJSON_EMPTY_ARRAY_A(allocator), allocator))
#define CJSON_EMPTY_ARRAY_V CJSON_EMPTY_ARRAY_V_A(NULL)
#define CJSON_ARRAY_V_A(allocator, ...)\
    (cjson_value_new_as_array(CJSON_ARRAY_A(allocator, __VA_ARGS__), allocator))
#define CJSON_ARRAY_V(...) CJSON_ARRAY_V_A(NULL, __VA_ARGS__)
#define CJSON_EMPTY_OBJECT_V_A(allocator) (cjson_value_new_as_object(CJSON_EMPTY_OBJECT_A(allocator), allocator))
#define CJSON_EMPTY_OBJECT_V CJSON_EMPTY_OBJECT_V_A(NULL)
#define CJSON_OBJECT_V_A(allocator, ...)\
    (cjson_value_new_as_object(CJSON_OBJECT_A(allocator, __VA_ARGS__), allocator))
#define CJSON_OBJECT_V(...) CJSON_OBJECT_V_A(NULL, __VA_ARGS__)

#define CJSON_AS_BOOL(v) (cjson_value_get_bool(v))
#define CJSON_AS_NUMBER(v) (cjson_value_get_number(v))
#define CJSON_AS_STR(v) (cjson_value_get_str(v))
#define CJSON_AS_RAW_STR(v) (cjson_str_raw(CJSON_AS_STR(v)))
#define CJSON_AS_ARRAY(v) (cjson_value_get_array(v))
#define CJSON_AS_OBJECT(v) (cjson_value_get_object(v))

#endif /* cjson_value_h */
