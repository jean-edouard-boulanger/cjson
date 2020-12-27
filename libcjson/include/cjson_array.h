//
//  cjson_array.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_array_h
#define cjson_array_h

#include "cjson_stringstream.h"
#include "cjson_utils.h"

#include <stdlib.h>
#include <stdbool.h>


typedef struct CJsonValue CJsonValue;

typedef struct CJsonArray {
    CJsonValue** _data;
    size_t _size;
    size_t _capacity;
} CJsonArray;

CJsonArray* cjson_array_new(void);
CJsonArray* cjson_array_copy(const CJsonArray* this);
void cjson_array_free(CJsonArray* this);

size_t cjson_array_size(const CJsonArray* this);
size_t cjson_array_capacity(const CJsonArray* this);
bool cjson_array_empty(const CJsonArray* this);
void cjson_array_reserve(CJsonArray* this, size_t capacity);

CJsonValue* cjson_array_at(CJsonArray* this, size_t index);
CJsonValue* cjson_array_front(CJsonArray* this);
CJsonValue* cjson_array_back(CJsonArray* this);

void cjson_array_assign(CJsonArray* this, size_t index, CJsonValue* val);
void cjson_array_insert(CJsonArray* this, size_t index, CJsonValue* val);
void cjson_array_swap(CJsonArray* this, size_t index, CJsonValue** val);
void cjson_array_push(CJsonArray* this, CJsonValue* val);
void cjson_array_erase(CJsonArray* this, size_t index);
void cjson_array_pop(CJsonArray* this);
void cjson_array_clear(CJsonArray* this);

bool cjson_array_equals(const CJsonArray* this, const CJsonArray* other);

void cjson_array_fmt(CJsonStringStream* stream, const CJsonArray* this);

CJsonArray* cjson_impl_array_builder(size_t items, ...);

#define CJSON_EMPTY_ARRAY cjson_array_new()
#define CJSON_ARRAY(...)\
    cjson_impl_array_builder(\
        CJSON_NUMARGS(__VA_ARGS__), __VA_ARGS__)

#endif /* cjson_array_h */
