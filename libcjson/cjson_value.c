//
//  cjson_value.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_array.h"
#include "cjson_assert.h"
#include "cjson_object.h"
#include "cjson_str.h"
#include "cjson_stringstream.h"
#include "cjson_value.h"

#include <stdlib.h>
#include <string.h>


CJsonValue* cjson_value_new() {
    CJsonValue* val = (CJsonValue*) malloc(sizeof(CJsonValue));
    val->_type = cjson_null_value;
    val->_null = NULL;
    return val;
}

void cjson_value_free(CJsonValue* this) {
    cjson_value_reset(this);
    free(this);
}

CJsonValue* cjson_value_new_as_null(void) {
    return cjson_value_new();
}

CJsonValue* cjson_value_new_as_object(CJsonObject* object) {
    CJsonValue* val = cjson_value_new();
    cjson_value_make_object(val, object);
    return val;
}

CJsonValue* cjson_value_new_as_array(CJsonArray* array) {
    CJsonValue* val = cjson_value_new();
    cjson_value_make_array(val, array);
    return val;
}

CJsonValue* cjson_value_new_as_str(CJsonStr* str) {
    CJsonValue* val = cjson_value_new();
    cjson_value_make_str(val, str);
    return val;
}

CJsonValue* cjson_value_new_as_bool(bool bool_val) {
    CJsonValue* val = cjson_value_new();
    cjson_value_make_bool(val, bool_val);
    return val;
}

CJsonValue* cjson_value_new_as_number(double number_val) {
    CJsonValue* val = cjson_value_new();
    cjson_value_make_number(val, number_val);
    return val;
}

CJsonValue* cjson_value_copy(const CJsonValue* const this) {
    CJsonValue* val = cjson_value_new();
    val->_type = this->_type;
    switch(val->_type) {
        case cjson_null_value: {
            val->_null = NULL;
            break;
        }
        case cjson_object_value: {
            val->_object = cjson_object_copy(this->_object);
            break;
        }
        case cjson_array_value: {
            val->_array = cjson_array_copy(this->_array);
            break;
        }
        case cjson_str_value: {
            val->_str = cjson_str_copy(this->_str);
            break;
        }
        case cjson_bool_value: {
            val->_bool = this->_bool;
            break;
        }
        case cjson_number_value: {
            val->_number = this->_number;
            break;
        }
    }
    return val;
}

void cjson_value_reset(CJsonValue* this) {
    if(cjson_value_is_object(this)) {
        cjson_object_free(this->_object);
        this->_object = NULL;
    }
    else if(cjson_value_is_array(this)) {
        cjson_array_free(this->_array);
        this->_array = NULL;
    }
    else if(cjson_value_is_str(this)) {
        cjson_str_free(this->_str);
        this->_str = NULL;
    }
    this->_null = NULL;
    this->_type = cjson_null_value;
}

bool cjson_value_is(const CJsonValue* const this, CJsonValueType type) {
    return this->_type == type;
}

bool cjson_value_is_null(const CJsonValue* const this) {
    return cjson_value_is(this, cjson_null_value);
}

bool cjson_value_is_object(const CJsonValue* const this) {
    return cjson_value_is(this, cjson_object_value);
}

bool cjson_value_is_array(const CJsonValue* const this) {
    return cjson_value_is(this, cjson_array_value);
}

bool cjson_value_is_str(const CJsonValue* const this) {
    return cjson_value_is(this, cjson_str_value);
}

bool cjson_value_is_bool(const CJsonValue* const this) {
    return cjson_value_is(this, cjson_bool_value);
}

bool cjson_value_is_number(const CJsonValue* const this) {
    return cjson_value_is(this, cjson_number_value);
}

CJsonObject* cjson_value_get_object(CJsonValue* this) {
    if(!cjson_value_is_object(this)) {
        return NULL;
    }
    return this->_object;
}

CJsonArray* cjson_value_get_array(CJsonValue* this) {
    if(!cjson_value_is_array(this)) {
        return NULL;
    }
    return this->_array;
}

CJsonStr* cjson_value_get_str(CJsonValue* this) {
    if(!cjson_value_is_str(this)) {
        return NULL;
    }
    return this->_str;
}

bool* cjson_value_get_bool(CJsonValue* this) {
    if(!cjson_value_is_bool(this)) {
        return NULL;
    }
    return &this->_bool;
}

double* cjson_value_get_number(CJsonValue* this) {
    if(!cjson_value_is_number(this)) {
        return NULL;
    }
    return &this->_number;
}

void cjson_value_make_null(CJsonValue* this) {
    if(!cjson_value_is_null(this)) {
        cjson_value_reset(this);
    }
}

void cjson_value_make_object(CJsonValue* this, CJsonObject* object) {
    if(!cjson_value_is_object(this)) {
        cjson_value_reset(this);
        this->_type = cjson_object_value;
    }
    this->_object = object;
}

void cjson_value_make_array(CJsonValue* this, CJsonArray* array) {
    if(!cjson_value_is_array(this)) {
        cjson_value_reset(this);
        this->_type = cjson_array_value;
    }
    this->_array = array;
}

void cjson_value_make_str(CJsonValue* this, CJsonStr* str) {
    if(!cjson_value_is_str(this)) {
        cjson_value_reset(this);
        this->_type = cjson_str_value;
    }
    this->_str = str;
}

void cjson_value_make_bool(CJsonValue* this, bool val) {
    if(!cjson_value_is_bool(this)) {
        cjson_value_reset(this);
        this->_type = cjson_bool_value;
    }
    this->_bool = val;
}

void cjson_value_make_number(CJsonValue* this, double val) {
    if(!cjson_value_is_number(this)) {
        cjson_value_reset(this);
        this->_type = cjson_number_value;
    }
    this->_number = val;
}

bool cjson_value_equals(const CJsonValue* this, const CJsonValue* other) {
    if(this->_type != other->_type) { return false; }
    switch(this->_type) {
        case cjson_null_value: return true;
        case cjson_object_value: return cjson_object_equals(this->_object, other->_object);
        case cjson_array_value: return cjson_array_equals(this->_array, other->_array);
        case cjson_str_value: return cjson_str_equals(this->_str, other->_str);
        case cjson_bool_value: return this->_bool == other->_bool;
        case cjson_number_value: return this->_number == other->_number;
    }
}

void cjson_bool_fmt(CJsonStringStream* stream, const bool* const val) {
    if(*val) {
        cjson_string_stream_write(stream, "true");
    }
    else {
        cjson_string_stream_write(stream, "false");
    }
}

void cjson_null_fmt(CJsonStringStream* stream) {
    cjson_string_stream_write(stream, "null");
}

void cjson_number_fmt(CJsonStringStream* stream, const double* val) {
    const size_t buffer_sz = snprintf(NULL, 0, "%f", *val);
    char* buffer = malloc(buffer_sz * sizeof(char));
    CJSON_CHECK_ALLOC(buffer);
    sprintf(buffer, "%f", *val);
    cjson_string_stream_write(stream, buffer);
    free(buffer);
}

void cjson_value_fmt(CJsonStringStream* stream, const CJsonValue* const this) {
    switch(this->_type) {
        case cjson_null_value:
            cjson_null_fmt(stream);
            return;
        case cjson_object_value:
            cjson_object_fmt(stream, this->_object);
            return;
        case cjson_array_value:
            cjson_array_fmt(stream, this->_array);
            return;
        case cjson_str_value:
            cjson_str_fmt(stream, this->_str);
            return;
        case cjson_bool_value:
            cjson_bool_fmt(stream, &this->_bool);
            return;
        case cjson_number_value:
            cjson_number_fmt(stream, &this->_number);
            return;
    }
}
