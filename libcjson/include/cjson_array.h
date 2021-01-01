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
typedef struct CJsonAllocator CJsonAllocator;

typedef struct CJsonArray {
    CJsonValue** _data;
    size_t _size;
    size_t _capacity;
    CJsonAllocator* _allocator;
} CJsonArray;

CJsonArray* cjson_array_new(CJsonAllocator* allocator);
CJsonArray* cjson_array_copy(CJsonArray* this);
void cjson_array_free(CJsonArray* this);

size_t cjson_array_size(CJsonArray* this);
size_t cjson_array_capacity(CJsonArray* this);
bool cjson_array_empty(CJsonArray* this);
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

bool cjson_array_equals(CJsonArray* this, CJsonArray* other);

void cjson_array_fmt(CJsonStringStream* stream, CJsonArray* this);

CJsonArray* cjson_impl_array_builder(CJsonAllocator* allocator, size_t items, ...);

#define CJSON_EMPTY_ARRAY_A(allocator) cjson_array_new(allocator)
#define CJSON_EMPTY_ARRAY CJSON_EMPTY_ARRAY_A(NULL)
#define CJSON_ARRAY_A(allocator, ...)\
    (cjson_impl_array_builder(\
        allocator, CJSON_VA_COUNT(__VA_ARGS__), __VA_ARGS__))
#define CJSON_ARRAY(...) CJSON_ARRAY_A(__VA_ARGS__)

#define CJSON_IMPL_ARRAY_ITERATOR_NAME CJSON_COMBINE(arr_it_, __LINE__)

#define CJSON_ARRAY_ENUMERATE(arr, index_var, val_var) \
    size_t index_var = 0; \
    CJsonValue* val_var = cjson_array_at(arr, index_var); \
    for(; index_var != cjson_array_size(arr); ++index_var, val_var = cjson_array_at(index_var))

#define CJSON_ARRAY_FOREACH_ITEM(arr, val_var) CJSON_ARRAY_ENUMERATE(arr, val_var, CJSON_IMPL_ARRAY_ITERATOR_NAME)

#endif /* cjson_array_h */
