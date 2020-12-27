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


typedef struct CJsonNull CJsonNull;
typedef struct CJsonObject CJsonObject;
typedef struct CJsonArray CJsonArray;
typedef struct CJsonStr CJsonStr;
typedef struct CJsonStringStream CJsonStringStream;

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
        struct CJsonNull* _null;
        struct CJsonObject* _object;
        struct CJsonArray* _array;
        struct CJsonStr* _str;
        bool _bool;
        double _number;
    };
    CJsonValueType _type;
} CJsonValue;

CJsonValue* cjson_value_new(void);
CJsonValue* cjson_value_new_as_null(void);
CJsonValue* cjson_value_new_as_object(CJsonObject* object);
CJsonValue* cjson_value_new_as_array(CJsonArray* array);
CJsonValue* cjson_value_new_as_str(CJsonStr* str);
CJsonValue* cjson_value_new_as_bool(bool val);
CJsonValue* cjson_value_new_as_number(double val);
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

#define CJSON_NULL_V (cjson_value_new_as_null())
#define CJSON_BOOL_V(x) (cjson_value_new_as_bool(x))
#define CJSON_TRUE_V CJSON_BOOL_V(true)
#define CJSON_FALSE_V CJSON_BOOL_V(false)
#define CJSON_NUMBER_V(x) (cjson_value_new_as_number(x))
#define CJSON_STR_V(x) (cjson_value_new_as_str(CJSON_STR(x)))
#define CJSON_EMPTY_ARRAY_V (cjson_value_new_as_array(CJSON_EMPTY_ARRAY))
#define CJSON_ARRAY_V(...)\
    (cjson_value_new_as_array(CJSON_ARRAY(__VA_ARGS__)))
#define CJSON_EMPTY_OBJECT_V (cjson_value_new_as_object(CJSON_EMPTY_OBJECT))
#define CJSON_OBJECT_V(...)\
    (cjson_value_new_as_object(CJSON_OBJECT(__VA_ARGS__)))

#define CJSON_AS_BOOL(v) (cjson_value_get_bool(v))
#define CJSON_AS_NUMBER(v) (cjson_value_get_number(v))
#define CJSON_AS_STR(v) (cjson_value_get_str(v))
#define CJSON_AS_RAW_STR(v) (cjson_str_raw(CJSON_AS_STR(v)))
#define CJSON_AS_ARRAY(v) (cjson_value_get_array(v))
#define CJSON_AS_OBJECT(v) (cjson_value_get_object(v))

#endif /* cjson_value_h */
