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
#include "cjson_allocator.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


const size_t k_default_capacity = 8;
const size_t k_capacity_growth_factor = 2;

CJsonArray* cjson_array_new(CJsonAllocator* allocator) {
    allocator = cjson_allocator_or_default(allocator);
    CJsonArray* array = (CJsonArray*) cjson_alloc(allocator, sizeof(CJsonArray));
    if(array == NULL) { return NULL; }
    array->_allocator = allocator;
    array->_size = 0;
    array->_capacity = k_default_capacity;
    array->_data = (CJsonValue**) cjson_alloc(allocator, array->_capacity * sizeof(CJsonValue*));
    if(array->_data == NULL) {
        cjson_dealloc(allocator, array);
        return NULL;
    }
    return array;
}

void cjson_array_free(CJsonArray* this) {
    cjson_array_clear(this);
    cjson_dealloc(this->_allocator, this->_data);
    cjson_dealloc(this->_allocator, this);
}

CJsonArray* cjson_array_copy(CJsonArray* this) {
    CJsonArray* array = (CJsonArray*) cjson_alloc(this->_allocator, sizeof(CJsonArray));
    if(array == NULL) {
        return NULL;
    }
    array->_size = this->_size;
    array->_capacity = this->_capacity;
    array->_data = (CJsonValue**)cjson_alloc(this->_allocator, array->_capacity * sizeof(CJsonValue*));
    if(array->_data == NULL) {
        cjson_dealloc(this->_allocator, array);
        return NULL;
    }
    array->_allocator = this->_allocator;
    for(size_t i = 0; i != array->_size; ++i) {
        array->_data[i] = cjson_value_copy(this->_data[i]);
    }
    return array;
}

size_t cjson_array_size(CJsonArray* this) {
    return this->_size;
}

size_t cjson_array_capacity(CJsonArray* this) {
    return this->_capacity;
}

bool cjson_array_empty(CJsonArray* this) {
    return this->_size == 0;
}

void cjson_array_reserve(CJsonArray* this, size_t capacity) {
    if(this->_capacity >= capacity) {
        return;
    }
    this->_data = (CJsonValue**) cjson_realloc(this->_allocator, this->_data, capacity * sizeof(CJsonValue*));
    if(this->_data == NULL) {
        return;
    }
    this->_capacity = capacity;
}

CJsonValue* cjson_array_at(CJsonArray* this, size_t index) {
    CJSON_CONTRACT(index < this->_size);
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

bool cjson_array_equals(CJsonArray* this, CJsonArray* other) {
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
    CJSON_CONTRACT(index < this->_size);
    cjson_value_free(this->_data[index]);
    this->_data[index] = val;
}

void cjson_array_swap(CJsonArray* this, size_t index, CJsonValue** val) {
    CJSON_CONTRACT(index < this->_size);
    CJsonValue* tmp = this->_data[index];
    this->_data[index] = *val;
    (*val) = tmp;
}

void cjson_array_insert(CJsonArray* this, size_t index, CJsonValue* val) {
    CJSON_CONTRACT(index <= this->_size);
    if(this->_size == this->_capacity) {
        cjson_array_reserve(this, this->_capacity * k_capacity_growth_factor);
    }
    CJSON_SAFE_ASSERT(this->_size < this->_capacity);
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
    CJSON_CONTRACT(index < this->_size);
    cjson_value_free(this->_data[index]);
    for(size_t i = index + 1; i != this->_size; ++i) {
        this->_data[i - 1] = this->_data[i];
    }
    this->_size -= 1;
}

void cjson_array_pop(CJsonArray* this) {
    cjson_array_erase(this, this->_size - 1);
}

void cjson_array_fmt(CJsonStringStream* stream, CJsonArray* this) {
    cjson_string_stream_write(stream, "[");
    for(size_t i = 0; i != this->_size; ++i) {
        cjson_value_fmt(stream, this->_data[i]);
        if(i < this->_size - 1) {
            cjson_string_stream_write(stream, ", ");
        }
    }
    cjson_string_stream_write(stream, "]");
}

CJsonArray* cjson_impl_array_builder(CJsonAllocator* allocator, size_t items, ...) {
    CJsonArray* array = cjson_array_new(allocator);
    va_list ap;
    va_start(ap, items);
    for(size_t i = 0; i < items; ++i) {
        CJsonValue* value = va_arg(ap, CJsonValue*);
        cjson_array_push(array, value);
    }
    va_end(ap);
    return array;
}
