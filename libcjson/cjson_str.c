//
//  cjson_str.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_assert.h"
#include "cjson_str.h"

#include <string.h>


CJsonStr* cjson_str_new_of_size(size_t size, char c) {
    CJsonStr* str = (CJsonStr*) malloc(sizeof(CJsonStr));
    CJSON_CHECK_ALLOC(str);
    const size_t buffer_sz = sizeof(char) * (size + 1);
    str->_data = (char*) malloc(buffer_sz);
    CJSON_CHECK_ALLOC(str->_data);
    memset(str->_data, c, buffer_sz);
    str->_data[size] = '\0';
    str->_size = size;
    return str;
}

CJsonStr* cjson_str_new() {
    return cjson_str_new_of_size(0, '\0');
}

CJsonStr* cjson_str_new_from_raw(const char* const cstr) {
    CJsonStr* str = cjson_str_new();
    str->_size = strlen(cstr);
    if(str->_size > 0) {
        const size_t buff_sz = sizeof(char) * (str->_size + 1);
        str->_data = (char*) malloc(buff_sz);
        memcpy(str->_data, cstr, buff_sz);
    }
    return str;
}

CJsonStr* cjson_str_copy(const CJsonStr* const this) {
    CJsonStr* str = cjson_str_new();
    str->_data = (char*) malloc(this->_size * sizeof(char));
    strcpy(str->_data, this->_data);
    str->_size = this->_size;
    return str;
}

char* cjson_raw_str_copy(const char* this) {
    char* buffer = (char*) malloc((strlen(this) + 1) * sizeof(char));
    strcpy(buffer, this);
    return buffer;
}

void cjson_str_free(CJsonStr* this) {
    cjson_str_clear(this);
    free(this);
}

void cjson_str_append_raw_string(CJsonStr* this, const char* const source) {
    const size_t source_sz = strlen(source);
    if(source_sz == 0) { return; }
    this->_data = realloc(this->_data, sizeof(char) * (this->_size + source_sz + 1));
    CJSON_CHECK_ALLOC(this->_data);
    memcpy(this->_data + this->_size, source, source_sz + 1);
    this->_size += source_sz;
}

void cjson_str_append(CJsonStr* this, const CJsonStr* source) {
    cjson_str_append_raw_string(this, source->_data);
}

void cjson_str_clear(CJsonStr* this) {
    this->_data = realloc(this->_data, sizeof(char));
    this->_data[0] = '\0';
    this->_size = 0;
}

void cjson_str_pop_back(CJsonStr* this) {
    CJSON_ASSERT(this->_size > 0);
    --this->_size;
    this->_data[this->_size] = '\0';
}

CJsonStr* cjson_str_substr(const CJsonStr* this, size_t begin, size_t end) {
    CJSON_ASSERT(end >= begin);
    CJSON_ASSERT(begin < this->_size && end <= this->_size);
    const size_t bytes = end - begin;
    CJsonStr* str = cjson_str_new_of_size(bytes, '\0');
    memcpy(str->_data, this->_data + begin, bytes);
    return str;
}

CJsonStr* cjson_str_concat(const CJsonStr* const s1, const CJsonStr* const s2) {
    CJsonStr* new_str = cjson_str_copy(s1);
    cjson_str_append(new_str, s2);
    return new_str;
}

CJsonOrdering cjson_str_cmp(const CJsonStr* const this, const CJsonStr* const other) {
    return cjson_raw_str_cmp(this->_data, other->_data);
}

CJsonOrdering cjson_raw_str_cmp(const char* const this, const char* const other) {
    const int cmp = strcmp(this, other);
    if(cmp < 0) { return cjson_ordering_less; }
    if(cmp > 0) { return cjson_ordering_greater; }
    return cjson_ordering_equal;
}

bool cjson_str_equals(const CJsonStr* const this, const CJsonStr* const other) {
    return cjson_raw_str_cmp(this->_data, other->_data) == cjson_ordering_equal;
}

bool cjson_str_equals_raw(const CJsonStr* const this, const char* const other) {
    return cjson_raw_str_cmp(this->_data, other) == cjson_ordering_equal;
}

bool cjson_raw_str_equals(const char* this, const char* other) {
    return cjson_raw_str_cmp(this, other) == cjson_ordering_equal;
}

size_t cjson_str_length(const CJsonStr* const this) {
    return this->_size;
}

void cjson_str_fmt(CJsonStringStream* stream, const CJsonStr* const this) {
    cjson_raw_str_fmt(stream, this->_data);
}

void cjson_raw_str_fmt(CJsonStringStream* stream, const char* const str) {
    const char* ptr = str;
    cjson_string_stream_write(stream, "\"");
    while(*ptr != '\0') {
        if(*ptr == '\"') {
            cjson_string_stream_write(stream, "\\\"");
        }
        else {
            cjson_string_stream_write_bytes(stream, ptr, 1);
        }
        ++ptr;
    }
    cjson_string_stream_write(stream, "\"");
}

char* cjson_str_raw(const CJsonStr* const this) {
    return this->_data;
}

char cjson_str_at(const CJsonStr* const this, size_t index) {
    CJSON_ASSERT(index < this->_size);
    return this->_data[index];
}

char cjson_str_front(const CJsonStr* const this) {
    CJSON_ASSERT(this->_size > 0);
    return this->_data[0];
}

char cjson_str_back(const CJsonStr* const this) {
    CJSON_ASSERT(this->_size > 0);
    return this->_data[this->_size - 1];
}

bool cjson_str_contains_raw(const CJsonStr* this, const char* substr) {
    return strstr(this->_data, substr) != NULL;
}

bool cjson_str_contains(const CJsonStr* this, const CJsonStr* other) {
    return cjson_str_contains_raw(this, other->_data);
}
