//
//  cjson_str.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_str_h
#define cjson_str_h

#include "cjson_ordering.h"
#include "cjson_stringstream.h"

#include <stdlib.h>
#include <stdbool.h>


typedef struct CJsonStr {
    char* _data;
    size_t _size;
} CJsonStr;

CJsonStr* cjson_str_new_from_raw(const char* cstr);
CJsonStr* cjson_str_new_of_size(size_t size, char c);
CJsonStr* cjson_str_new(void);
CJsonStr* cjson_str_copy(const CJsonStr* this);
char* cjson_raw_str_copy(const char* this);
void cjson_str_free(CJsonStr* this);

void cjson_str_clear(CJsonStr* this);
void cjson_str_append(CJsonStr* this, const CJsonStr* source);
void cjson_str_append_raw_string(CJsonStr* this, const char* source);
void cjson_str_pop_back(CJsonStr* this);

CJsonStr* cjson_str_substr(const CJsonStr* this, size_t begin, size_t end);

size_t cjson_str_length(const CJsonStr* this);
char cjson_str_at(const CJsonStr* this, size_t index);
char cjson_str_front(const CJsonStr* this);
char cjson_str_back(const CJsonStr* this);

bool cjson_str_contains(const CJsonStr* this, const CJsonStr* other);
bool cjson_str_contains_raw(const CJsonStr* this, const char* substr);
CJsonOrdering cjson_str_cmp(const CJsonStr* this, const CJsonStr* other);
CJsonOrdering cjson_raw_str_cmp(const char* this, const char* other);
bool cjson_str_equals(const CJsonStr* this, const CJsonStr* other);
bool cjson_str_equals_raw(const CJsonStr* this, const char* other);
bool cjson_raw_str_equals(const char* this, const char* other);

char* cjson_str_raw(const CJsonStr* this);

CJsonStr* cjson_str_concat(const CJsonStr* s1, const CJsonStr* s2);

void cjson_str_fmt(CJsonStringStream* stream, const CJsonStr* this);
void cjson_raw_str_fmt(CJsonStringStream* stream, const char* str);

#define CJSON_STR(x) cjson_str_new_from_raw(x)

#endif /* cjson_str_h */
