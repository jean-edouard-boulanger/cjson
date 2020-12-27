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

typedef struct CJsonObject {
    CJsonObjectNode* _root;
} CJsonObject;

CJsonObject* cjson_object_new(void);
CJsonObject* cjson_object_copy(const CJsonObject* this);
void cjson_object_free(CJsonObject* this);

void cjson_object_set(CJsonObject* this, const char* key, CJsonValue* val);
void cjson_object_del(CJsonObject* this, const char* key);
CJsonValue* cjson_object_get(CJsonObject* this, const char* key);
bool cjson_object_has(const CJsonObject* this, const char* key);
size_t cjson_object_size(const CJsonObject* this);

bool cjson_object_equals(const CJsonObject* this, const CJsonObject* other);

void cjson_object_fmt(CJsonStringStream* stream, const CJsonObject* this);

CJsonObject* cjson_impl_object_builder(size_t kvs, ...);

#define CJSON_EMPTY_OBJECT cjson_object_new()
#define CJSON_OBJECT(...) \
    cjson_impl_object_builder( \
        CJSON_NUMARGS(__VA_ARGS__), __VA_ARGS__)

#define CJSON_OBJECT_FOREACH(object, it) \
    for(CJsonObjectIterator* it = cjson_object_iter_begin())

#endif /* cjson_object_h */
