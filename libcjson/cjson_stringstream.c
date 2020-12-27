//
//  cjson_stringstream.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 23/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_stringstream.h"
#include "cjson_utils.h"
#include "cjson_assert.h"

#include <stdlib.h>
#include <string.h>

#ifndef CJSON_STRING_STREAM_BLOCK_SIZE
#define CJSON_STRING_STREAM_BLOCK_SIZE 128
#endif

CJSON_STATIC_ASSERT(CJSON_STRING_STREAM_BLOCK_SIZE > 0);


typedef struct StringStreamBlock {
    char* data;
    size_t size;
    struct StringStreamBlock* next;
} StringStreamBlock;

StringStreamBlock* string_stream_block_new(void) {
    StringStreamBlock* block = (StringStreamBlock*) malloc(sizeof(StringStreamBlock));
    block->data = (char*) malloc(CJSON_STRING_STREAM_BLOCK_SIZE * sizeof(char));
    block->size = 0;
    block->next = NULL;
    return block;
}

size_t string_stream_block_write_bytes(StringStreamBlock* this, const char* data, size_t bytes) {
    const size_t capacity = CJSON_STRING_STREAM_BLOCK_SIZE - this->size;
    const size_t written_bytes = CJSON_MIN(bytes, capacity);
    memcpy(this->data + this->size, data, written_bytes * sizeof(char));
    this->size += written_bytes;
    return written_bytes;
}

void string_stream_block_free(StringStreamBlock* this) {
    free(this->data);
    if(this->next != NULL) {
        string_stream_block_free(this->next);
    }
    free(this);
}

typedef struct CJsonImplStringStream {
    StringStreamBlock* head;
    StringStreamBlock* tail;
} CJsonImplStringStream;

CJsonImplStringStream* cjson_impl_string_stream_new(void) {
    CJsonImplStringStream* stream = (CJsonImplStringStream*) malloc(sizeof(CJsonImplStringStream));
    stream->head = string_stream_block_new();
    stream->tail = stream->head;
    return stream;
}

void cjson_impl_string_stream_free(CJsonImplStringStream* this) {
    string_stream_block_free(this->head);
    free(this);
}

void cjson_impl_string_stream_write_bytes(CJsonImplStringStream* this, const char* data, size_t bytes) {
    StringStreamBlock* block = this->tail;
    size_t written_bytes = 0;
    for(;;) {
        written_bytes += string_stream_block_write_bytes(
            block, data + written_bytes, bytes - written_bytes);
        CJSON_ASSERT(written_bytes <= bytes);
        if(written_bytes == bytes) {
            return;
        }
        block = string_stream_block_new();
        this->tail->next = block;
        this->tail = block;
    }
}

size_t cjson_impl_string_stream_bytes(const CJsonImplStringStream* const this) {
    size_t size = 0;
    StringStreamBlock* current = this->head;
    while(current != NULL) {
        size += current->size;
        current = current->next;
    }
    return size;
}

char* cjson_impl_string_stream_str(const CJsonImplStringStream* const this) {
    const size_t size = cjson_impl_string_stream_bytes(this);
    char* buff = (char*) malloc((size + 1) * sizeof(char));
    char* ptr = buff;
    buff[size] = '\0';
    StringStreamBlock* current = this->head;
    while(current != NULL) {
        memcpy(ptr, current->data, current->size * sizeof(char));
        ptr += current->size;
        current = current->next;
    }
    return buff;
}

CJsonStringStream* cjson_string_stream_new(void) {
    CJsonStringStream* stream = (CJsonStringStream*) malloc(sizeof(CJsonStringStream));
    stream->impl = cjson_impl_string_stream_new();
    return stream;
}

void cjson_string_stream_free(CJsonStringStream* this) {
    cjson_impl_string_stream_free(this->impl);
    free(this);
}

void cjson_string_stream_write_bytes(CJsonStringStream* this, const char* data, size_t bytes) {
    cjson_impl_string_stream_write_bytes(this->impl, data, bytes);
}

void cjson_string_stream_write(CJsonStringStream* this, const char* data) {
    cjson_string_stream_write_bytes(this, data, strlen(data));
}

char* cjson_string_stream_str(const CJsonStringStream* const this) {
    return cjson_impl_string_stream_str(this->impl);
}
