//
//  cjson_object.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_assert.h"
#include "cjson_object.h"
#include "cjson_ordering.h"
#include "cjson_str.h"
#include "cjson_value.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct CJsonObjectNode {
    char* key;
    CJsonValue* val;

    CJsonObjectNode* left;
    CJsonObjectNode* right;
} CJsonObjectNode;

CJsonObjectNode* cjson_object_node_new(const char* const key, CJsonValue* val) {
    CJsonObjectNode* node = (CJsonObjectNode*) malloc(sizeof(CJsonObjectNode));
    node->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(node->key, key);
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void cjson_object_node_free(CJsonObjectNode* this) {
    if(this == NULL) {
        return;
    }
    free(this->key);
    cjson_value_free(this->val);
    cjson_object_node_free(this->left);
    cjson_object_node_free(this->right);
    free(this);
}

CJsonObjectNode* cjson_object_node_copy(CJsonObjectNode* this) {
    if(this == NULL) { return NULL; }
    CJsonObjectNode* node = cjson_object_node_new(this->key, cjson_value_copy(this->val));
    cjson_object_node_copy(node->left);
    cjson_object_node_copy(node->right);
    return node;
}

void cjson_object_node_insert(CJsonObjectNode* this, const char* const key, CJsonValue* val) {
    CJSON_ASSERT(this != NULL);
    const CJsonOrdering cmp = cjson_raw_str_cmp(key, this->key);
    if(cmp == cjson_ordering_equal) {
        cjson_value_free(this->val);
        this->val = val;
    }
    else if(cmp == cjson_ordering_less) {
        if(this->left == NULL) {
            this->left = cjson_object_node_new(key, val);
        }
        else {
            cjson_object_node_insert(this->left, key, val);
        }
    }
    else if(cmp == cjson_ordering_greater) {
        if(this->right == NULL) {
            this->right = cjson_object_node_new(key, val);
        }
        else {
            cjson_object_node_insert(this->right, key, val);
        }
    }
}

CJsonObjectNode* cjson_object_node_find(CJsonObjectNode* this, const char* const key) {
    if(this == NULL) { return NULL; }
    const CJsonOrdering cmp = cjson_raw_str_cmp(key, this->key);
    if(cmp == cjson_ordering_equal) { return this; }
    if(cmp == cjson_ordering_less) {
        return cjson_object_node_find(this->left, key);
    }
    return cjson_object_node_find(this->right, key);
}

size_t cjson_object_node_size(const CJsonObjectNode* this) {
    if(this == NULL) { return 0; }
    return 1 + cjson_object_node_size(this->left) + cjson_object_node_size(this->right);
}

CJsonObject* cjson_object_new() {
    CJsonObject* object = (CJsonObject*) malloc(sizeof(CJsonObject));
    object->_root = NULL;
    return object;
}

void cjson_object_free(CJsonObject* this) {
    if(this->_root != NULL) {
        cjson_object_node_free(this->_root);
    }
    free(this);
}

CJsonObject* cjson_object_copy(const CJsonObject* const this) {
    CJsonObject* obj = cjson_object_new();
    if(this->_root != NULL) {
        obj->_root = cjson_object_node_copy(this->_root);
    }
    return obj;
}

void cjson_object_set(CJsonObject* this, const char* const key, CJsonValue* val) {
    if(this->_root == NULL) {
        this->_root = cjson_object_node_new(key, val);
        return;
    }
    cjson_object_node_insert(this->_root, key, val);
}

void cjson_object_del(CJsonObject* this, const char* const key) {
    // TODO implement: cjson_object_del
    CJSON_ABORT("not implemented: cjson_object_del");
}

CJsonValue* cjson_object_get(CJsonObject* this, const char* const key) {
    CJsonObjectNode* node = cjson_object_node_find(this->_root, key);
    if(node == NULL) {
        return NULL;
    }
    return node->val;
}

bool cjson_object_has(const CJsonObject* const this, const char* const key) {
    CJsonObjectNode* node = cjson_object_node_find(this->_root, key);
    return node != NULL;
}

size_t cjson_object_size(const CJsonObject* this) {
    return cjson_object_node_size(this->_root);
}

bool cjson_object_equals(const CJsonObject* this, const CJsonObject* other) {
    const size_t this_size = cjson_object_size(this);
    if(this_size != cjson_object_size(other)) { return false; }
    // TODO implement: cjson_object_equals
    CJSON_ABORT("not implemented: cjson_object_equals");
}

void cjson_object_node_fmt(CJsonStringStream* stream, const CJsonObjectNode* const node, bool sep) {
    if(node == NULL) { return; }
    if(sep) { cjson_string_stream_write(stream, ", "); }
    cjson_raw_str_fmt(stream, node->key);
    cjson_string_stream_write(stream, ": ");
    cjson_value_fmt(stream, node->val);
    cjson_object_node_fmt(stream, node->left, true);
    cjson_object_node_fmt(stream, node->right, true);
}

void cjson_object_fmt(CJsonStringStream* stream, const CJsonObject* const this) {
    cjson_string_stream_write(stream, "{");
    const bool sep = false;
    cjson_object_node_fmt(stream, this->_root, sep);
    cjson_string_stream_write(stream, "}");
}

CJsonObject* cjson_impl_object_builder(size_t elems, ...) {
    CJSON_ASSERT_MSG(cjson_mod((int)elems, 2) == 0, "bad object construction");
    CJsonObject* object = cjson_object_new();
    va_list ap;
    va_start(ap, elems);
    char* key = NULL;
    for(size_t i = 0; i < elems; ++i) {
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
