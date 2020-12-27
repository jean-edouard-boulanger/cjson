//
//  cjson_array.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_array.h"
#include "cjson_value.h"
#include "cjson_assert.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


const size_t k_default_capacity = 8;
const size_t k_capacity_growth_factor = 2;


CJsonArray* cjson_array_new() {
    CJsonArray* array = (CJsonArray*) malloc(sizeof(CJsonArray));
    CJSON_CHECK_ALLOC(array);
    array->_size = 0;
    array->_capacity = k_default_capacity;
    array->_data = (CJsonValue**)malloc(array->_capacity * sizeof(CJsonValue*));
    CJSON_CHECK_ALLOC(array->_data);
    return array;
}

void cjson_array_free(CJsonArray* this) {
    cjson_array_clear(this);
    free(this->_data);
    free(this);
}

CJsonArray* cjson_array_copy(const CJsonArray* const this) {
    CJsonArray* array = (CJsonArray*) malloc(sizeof(CJsonArray));
    CJSON_CHECK_ALLOC(array);
    array->_size = this->_size;
    array->_capacity = this->_capacity;
    array->_data = (CJsonValue**)malloc(array->_capacity * sizeof(CJsonValue*));
    CJSON_CHECK_ALLOC(array->_data);
    for(size_t i = 0; i != array->_size; ++i) {
        array->_data[i] = cjson_value_copy(this->_data[i]);
    }
    return array;
}

size_t cjson_array_size(const CJsonArray* const this) {
    return this->_size;
}

size_t cjson_array_capacity(const CJsonArray* const this) {
    return this->_capacity;
}

bool cjson_array_empty(const CJsonArray* const this) {
    return this->_size == 0;
}

void cjson_array_reserve(CJsonArray* this, size_t capacity) {
    if(this->_capacity >= capacity) {
        return;
    }
    this->_data = realloc(this->_data, capacity * sizeof(CJsonValue*));
    CJSON_CHECK_ALLOC(this->_data);
    this->_capacity = capacity;
}

CJsonValue* cjson_array_at(CJsonArray* this, size_t index) {
    if(index >= this->_size) {
        return NULL;
    }
    return this->_data[index];
}

CJsonValue* cjson_array_front(CJsonArray* this) {
    return cjson_array_at(this, 0);
}

CJsonValue* cjson_array_back(CJsonArray* this) {
    return cjson_array_at(this, this->_size);
}

void cjson_array_clear(CJsonArray* this) {
    for(size_t i = 0; i < this->_size; ++i) {
        cjson_value_free(this->_data[i]);
    }
    this->_size = 0;
}

bool cjson_array_equals(const CJsonArray* this, const CJsonArray* other) {
    if(this->_size != other->_size) { return false; }
    for(size_t i = 0; i != this->_size; ++i) {
        const CJsonValue* this_value = this->_data[i];
        const CJsonValue* other_value = other->_data[i];
        if(!cjson_value_equals(this_value, other_value)) {
            return false;
        }
    }
    return true;
}

void cjson_array_assign(CJsonArray* this, size_t index, CJsonValue* val) {
    CJSON_ASSERT(index < this->_size);
    cjson_value_free(this->_data[index]);
    this->_data[index] = val;
}

void cjson_array_swap(CJsonArray* this, size_t index, CJsonValue** val) {
    CJSON_ASSERT(index < this->_size);
    CJsonValue* tmp = this->_data[index];
    this->_data[index] = *val;
    (*val) = tmp;
}

void cjson_array_insert(CJsonArray* this, size_t index, CJsonValue* val) {
    CJSON_ASSERT(index <= this->_size);
    if(this->_size == this->_capacity) {
        cjson_array_reserve(this, this->_capacity * k_capacity_growth_factor);
    }
    CJSON_ASSERT(this->_size < this->_capacity);
    for(size_t i = this->_size; i != index; --i) {
        this->_data[i] = this->_data[i - 1];
    }
    this->_data[index] = val;
    ++this->_size;
}

void cjson_array_push(CJsonArray* this, CJsonValue* val) {
    cjson_array_insert(this, this->_size, val);
}

void cjson_array_erase(CJsonArray* this, size_t index) {
    CJSON_ASSERT(index < this->_size);
    cjson_value_free(this->_data[index]);
    for(size_t i = index + 1; i != this->_size; ++i) {
        this->_data[i - 1] = this->_data[i];
    }
    this->_size -= 1;
}

void cjson_array_pop(CJsonArray* this) {
    cjson_array_erase(this, this->_size - 1);
}

void cjson_array_fmt(CJsonStringStream* stream, const CJsonArray* const this) {
    cjson_string_stream_write(stream, "[");
    for(size_t i = 0; i != this->_size; ++i) {
        cjson_value_fmt(stream, this->_data[i]);
        if(i < this->_size - 1) {
            cjson_string_stream_write(stream, ", ");
        }
    }
    cjson_string_stream_write(stream, "]");
}

CJsonArray* cjson_impl_array_builder(size_t elems, ...) {
    CJsonArray* array = cjson_array_new();
    va_list ap;
    va_start(ap, elems);
    for(size_t i = 0; i < elems; ++i) {
        CJsonValue* value = va_arg(ap, CJsonValue*);
        cjson_array_push(array, value);
    }
    va_end(ap);
    return array;
}
