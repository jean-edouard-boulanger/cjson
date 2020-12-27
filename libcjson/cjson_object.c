//
//  cjson_object.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_assert.h"
#include "cjson_object.h"
#include "cjson_str.h"
#include "cjson_value.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const size_t k_default_hash_table_size = 16;

size_t hash_func(const char* key) {
    size_t hash = 5381;
    int c = 0;
    while((c = *key++)) {
        hash = hash * 33 + c;
    }
    return hash;
}

typedef struct CJsonObjectNode {
    char* key;
    CJsonValue* val;

    CJsonObjectNode* next;
    CJsonObjectNode** block_ref;
} CJsonObjectNode;

CJsonObjectNode* cjson_object_node_new(const char* key, CJsonValue* val, CJsonObjectNode** block_ref) {
    CJsonObjectNode* node = (CJsonObjectNode*) malloc(sizeof(CJsonObjectNode));
    node->key = (char*) malloc((strlen(key) + 1) * sizeof(char));
    strcpy(node->key, key);
    node->val = val;
    node->next = NULL;
    node->block_ref = block_ref;
    return node;
}

void cjson_object_node_free(CJsonObjectNode* this) {
    if(this == NULL) { return; }
    free(this->key);
    cjson_value_free(this->val);
    free(this);
}

void cjson_object_node_free_block(CJsonObjectNode* this) {
    if(this == NULL) { return; }
    cjson_object_node_free(this->next);
    cjson_object_node_free(this);
}

void cjson_object_node_insert(CJsonObjectNode* this, const char* key, struct CJsonValue* val) {
    CJsonObjectNode* current = this;
    while(current->next != NULL && cjson_raw_str_cmp(current->key, key) == cjson_ordering_equal) {
        current = current->next;
    }
    if(strcmp(current->key, key) == 0) {
        free(current->key);
        current->key = (char*) malloc((strlen(key) + 1) * sizeof(char));
        strcpy(current->key, key);
        cjson_value_free(current->val);
        current->val = val;
    }
    else {
        current->next = cjson_object_node_new(key, val, current->block_ref);
    }
}

size_t cjson_object_node_size(CJsonObjectNode* this) {
    size_t size = 0;
    CJsonObjectNode* current = this;
    while(current != NULL) {
        size += 1;
        current = current->next;
    }
    return size;
}

CJsonObjectNode* cjson_object_node_find(CJsonObjectNode* this, const char* key) {
    if(this == NULL) { return NULL; }
    if(cjson_raw_str_equals(key, this->key)) {
        return this;
    }
    return cjson_object_node_find(this->next, key);
}

bool cjson_impl_object_is_end_marker(CJsonObjectNode* node) {
    return node->val == NULL && node->block_ref == NULL;
}

CJsonObjectNode* cjson_impl_object_end_marker_new() {
    return cjson_object_node_new("", NULL, NULL);
}

CJsonObject* cjson_object_new() {
    CJsonObject* obj = (CJsonObject*) malloc(sizeof(CJsonObject));
    obj->_slots = k_default_hash_table_size;
    obj->_data = (CJsonObjectNode**) malloc((obj->_slots + 1) * sizeof(CJsonObjectNode*));
    for(size_t i = 0; i != obj->_slots; ++i) {
        obj->_data[i] = NULL;
    }
    obj->_data[obj->_slots + 1] = cjson_impl_object_end_marker_new();
    return obj;
}

void cjson_object_free(CJsonObject* this) {
    for(size_t i = 0; i != this->_slots; ++i) {
        cjson_object_node_free_block(this->_data[i]);
    }
    free(this->_data);
    free(this);
}

CJsonObject* cjson_object_copy(CJsonObject* this) {
    CJsonObject* new_obj = cjson_object_new();
    CJSON_OBJECT_FOREACH_KV(this, key, val) {
        cjson_object_set(new_obj, key, cjson_value_copy(val));
    }
    return new_obj;
}

void cjson_object_set(CJsonObject* this, const char* key, CJsonValue* val) {
    const size_t slot = hash_func(key) % this->_slots;
    CJsonObjectNode* obj = this->_data[slot];
    if(obj == NULL) {
        this->_data[slot] = cjson_object_node_new(key, val, this->_data + slot);
        return;
    }
    cjson_object_node_insert(obj, key, val);
}

void cjson_object_del(CJsonObject* this, const char* const key) {
    const size_t slot = hash_func(key) % this->_slots;
    CJsonObjectNode* obj = this->_data[slot];
    if(obj == NULL) { return; }
    if(cjson_raw_str_cmp(obj->key, key) == cjson_ordering_equal) {
        this->_data[slot] = obj->next;
        cjson_object_node_free(obj);
        return;
    }
    CJsonObjectNode* current = obj;
    while(current->next != NULL) {
        CJsonObjectNode* next = current->next;
        if(cjson_raw_str_cmp(next->key, key) == cjson_ordering_equal) {
            current->next = next->next;
            cjson_object_node_free(next);
        }
    }
}

CJsonValue* cjson_object_get(CJsonObject* this, const char* const key) {
    const size_t slot = hash_func(key) % this->_slots;
    CJsonObjectNode* node = cjson_object_node_find(this->_data[slot], key);
    if(node == NULL) {
        return NULL;
    }
    return node->val;
}

bool cjson_object_has(CJsonObject* this, const char* const key) {
    const size_t slot = hash_func(key) % this->_slots;
    CJsonObjectNode* node = cjson_object_node_find(this->_data[slot], key);
    return node != NULL;
}

size_t cjson_object_size(CJsonObject* this) {
    size_t size = 0;
    for(size_t i = 0; i != this->_slots; ++i) {
        size += cjson_object_node_size(this->_data[i]);
    }
    return size;
}

CJsonObjectIterator cjson_object_iter_begin(CJsonObject* this) {
    for(size_t i = 0; i != this->_slots; ++i) {
        CJsonObjectNode* current = this->_data[i];
        if(current != NULL) {
            return current;
        }
    }
    return cjson_object_iter_end(this);
}

CJsonObjectIterator cjson_object_iter_end(CJsonObject* this) {
    CJsonObjectNode* end_marker = this->_data[this->_slots + 1];
    CJSON_ASSERT(cjson_impl_object_is_end_marker(end_marker));
    return end_marker;
}

CJsonObjectIterator cjson_object_iter_next(CJsonObjectIterator it) {
    if(cjson_impl_object_is_end_marker(it)) { return it; }
    if(it->next != NULL) {
        return it->next;
    }
    CJsonObjectNode** current_block = it->block_ref + 1;
    while(*current_block == NULL) { ++current_block; }
    return *current_block;
}

bool cjson_object_iter_is_end(CJsonObjectIterator it) {
    return cjson_impl_object_is_end_marker(it);
}

char* cjson_object_iter_get_key(CJsonObjectIterator it) {
    if(cjson_impl_object_is_end_marker(it)) {
        return NULL;
    }
    return it->key;
}

CJsonValue* cjson_object_iter_get_value(CJsonObjectIterator it) {
    return it->val;
}

bool cjson_object_equals(CJsonObject* this, CJsonObject* other) {
    if(cjson_object_size(this) != cjson_object_size(other)) {
        return false;
    }
    CJSON_OBJECT_FOREACH_KV(this, key, val) {
        CJsonValue* other_val = cjson_object_get(other, key);
        if(other_val == NULL || !cjson_value_equals(val, other_val)) {
            return false;
        }
    }
    return true;
}

void cjson_object_fmt(CJsonStringStream* stream, CJsonObject* this) {
    cjson_string_stream_write(stream, "{");
    CJsonObjectIterator it = cjson_object_iter_begin(this);
    while(!cjson_object_iter_is_end(it)) {
        it = cjson_object_iter_next(it);
        cjson_raw_str_fmt(stream, it->key);
        cjson_string_stream_write(stream, ": ");
        cjson_value_fmt(stream, it->val);
        if(!cjson_object_iter_is_end(it)) {
            cjson_string_stream_write(stream, ", ");
        }
    }
    cjson_string_stream_write(stream, "}");
}

CJsonObject* cjson_impl_object_builder(size_t items, ...) {
    CJSON_ASSERT_MSG(cjson_mod((int)items, 2) == 0, "bad object construction");
    CJsonObject* object = cjson_object_new();
    va_list ap;
    va_start(ap, items);
    char* key = NULL;
    for(size_t i = 0; i < items; ++i) {
        // key
        if(i % 2 == 0) {
            CJSON_ASSERT(key == NULL);
            key = va_arg(ap, char*);
        }
        // value
        else {
            CJSON_ASSERT(key != NULL);
            cjson_object_set(object, key, va_arg(ap, CJsonValue*));
            key = NULL;
        }
    }
    va_end(ap);
    return object;
}
