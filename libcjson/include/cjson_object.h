//
//  cjson_object.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_object_h
#define cjson_object_h

#include "cjson_stringstream.h"

#include <stdbool.h>


typedef struct CJsonValue CJsonValue;
typedef struct CJsonArray CJsonArray;
typedef struct CJsonStr CJsonStr;

typedef struct CJsonObjectNode CJsonObjectNode;
typedef CJsonObjectNode* CJsonObjectIterator;

typedef struct CJsonObject {
    CJsonObjectNode** _data;
    size_t _slots;
} CJsonObject;

CJsonObject* cjson_object_new(void);
CJsonObject* cjson_object_copy(CJsonObject* this);
void cjson_object_free(CJsonObject* this);

void cjson_object_set(CJsonObject* this, const char* key, CJsonValue* val);
void cjson_object_del(CJsonObject* this, const char* key);
CJsonValue* cjson_object_get(CJsonObject* this, const char* key);
bool cjson_object_has(CJsonObject* this, const char* key);
size_t cjson_object_size(CJsonObject* this);

CJsonObjectIterator cjson_object_iter_begin(CJsonObject* this);
CJsonObjectIterator cjson_object_iter_end(CJsonObject* this);
CJsonObjectIterator cjson_object_iter_next(CJsonObjectIterator it);
bool cjson_object_iter_is_end(CJsonObjectIterator it);
char* cjson_object_iter_get_key(CJsonObjectIterator it);
CJsonValue* cjson_object_iter_get_value(CJsonObjectIterator it);

bool cjson_object_equals(CJsonObject* this, CJsonObject* other);

void cjson_object_fmt(CJsonStringStream* stream, CJsonObject* this);

CJsonObject* cjson_impl_object_builder(size_t kvs, ...);

#define CJSON_EMPTY_OBJECT cjson_object_new()
#define CJSON_OBJECT(...) \
    cjson_impl_object_builder( \
        CJSON_NUMARGS(__VA_ARGS__), __VA_ARGS__)

#define CJSON_IMPL_OBJECT_ITERATOR_NAME CJSON_COMBINE(obj_it_, __LINE__)

#define CJSON_OBJECT_FOREACH(obj, it_var) \
    for(CJsonObjectIterator it_var = cjson_object_iter_begin(obj); \
        !cjson_object_iter_is_end(it_var); \
        it_var = cjson_object_iter_next(it_var))

#define CJSON_OBJECT_FOREACH_ITEM(obj, key_var, val_var) \
    CJsonObjectIterator CJSON_IMPL_OBJECT_ITERATOR_NAME = cjson_object_iter_begin(obj); \
    char* key_var = cjson_object_iter_get_key(CJSON_IMPL_OBJECT_ITERATOR_NAME); \
    CJsonValue* val_var = cjson_object_iter_get_value(CJSON_IMPL_OBJECT_ITERATOR_NAME); \
    for(; !cjson_object_iter_is_end(CJSON_IMPL_OBJECT_ITERATOR_NAME); \
        CJSON_IMPL_OBJECT_ITERATOR_NAME = cjson_object_iter_next(CJSON_IMPL_OBJECT_ITERATOR_NAME), \
        key_var = cjson_object_iter_get_key(CJSON_IMPL_OBJECT_ITERATOR_NAME), \
        val_var = cjson_object_iter_get_value(CJSON_IMPL_OBJECT_ITERATOR_NAME))

#define CJSON_OBJECT_FOREACH_KEY(obj, key_var) \
    CJsonObjectIterator CJSON_IMPL_OBJECT_ITERATOR_NAME = cjson_object_iter_begin(obj); \
    char* key_var = cjson_object_iter_get_key(CJSON_IMPL_OBJECT_ITERATOR_NAME); \
    for(; !cjson_object_iter_is_end(CJSON_IMPL_OBJECT_ITERATOR_NAME); \
        CJSON_IMPL_OBJECT_ITERATOR_NAME = cjson_object_iter_next(CJSON_IMPL_OBJECT_ITERATOR_NAME), \
        key_var = cjson_object_iter_get_key(CJSON_IMPL_OBJECT_ITERATOR_NAME))

#define CJSON_OBJECT_FOREACH_VALUE(obj, val_var) \
    CJsonObjectIterator CJSON_IMPL_OBJECT_ITERATOR_NAME = cjson_object_iter_begin(obj); \
    CJsonValue* val_var = cjson_object_iter_get_value(CJSON_IMPL_OBJECT_ITERATOR_NAME); \
    for(; !cjson_object_iter_is_end(CJSON_IMPL_OBJECT_ITERATOR_NAME); \
        CJSON_IMPL_OBJECT_ITERATOR_NAME = cjson_object_iter_next(CJSON_IMPL_OBJECT_ITERATOR_NAME), \
        val_var = cjson_object_iter_get_value(CJSON_IMPL_OBJECT_ITERATOR_NAME))

#endif /* cjson_object_h */
