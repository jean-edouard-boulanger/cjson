//
//  cjson_stringstream.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 23/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_allocator.h"
#include "cjson_buffer.h"
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
    CJsonAllocator* allocator;
} StringStreamBlock;

StringStreamBlock* string_stream_block_new(CJsonAllocator* allocator) {
    StringStreamBlock* block = (StringStreamBlock*) cjson_alloc(allocator, sizeof(StringStreamBlock));
    block->data = (char*) cjson_alloc(allocator, CJSON_STRING_STREAM_BLOCK_SIZE * sizeof(char));
    block->size = 0;
    block->next = NULL;
    block->allocator = allocator;
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
    cjson_dealloc(this->allocator, this->data);
    if(this->next != NULL) {
        string_stream_block_free(this->next);
    }
    cjson_dealloc(this->allocator, this);
}

typedef struct CJsonImplStringStream {
    StringStreamBlock* head;
    StringStreamBlock* tail;
    CJsonBuffer* format_buffer;
    CJsonAllocator* allocator;
} CJsonImplStringStream;

CJsonImplStringStream* cjson_impl_string_stream_new(CJsonAllocator* allocator) {
    CJsonImplStringStream* stream = (CJsonImplStringStream*) cjson_alloc(allocator, sizeof(CJsonImplStringStream));
    stream->head = string_stream_block_new(allocator);
    stream->tail = stream->head;
    stream->format_buffer = cjson_buffer_new(64, allocator);
    stream->allocator = allocator;
    return stream;
}

void cjson_impl_string_stream_free(CJsonImplStringStream* this) {
    string_stream_block_free(this->head);
    cjson_buffer_free(this->format_buffer);
    cjson_dealloc(this->allocator, this);
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
        block = string_stream_block_new(this->allocator);
        this->tail->next = block;
        this->tail = block;
    }
}

void cjson_impl_string_stream_write_double(CJsonImplStringStream* this, const double* val) {
    const size_t total_bytes = snprintf(this->format_buffer->buffer, this->format_buffer->size, "%f", *val) + 1;
    if(total_bytes > this->format_buffer->size) {
        cjson_buffer_resize(this->format_buffer, total_bytes);
        sprintf(this->format_buffer->buffer, "%f", *val);
    }
    cjson_impl_string_stream_write_bytes(this, this->format_buffer->buffer, total_bytes);
}

size_t cjson_impl_string_stream_size(const CJsonImplStringStream* const this) {
    size_t size = 0;
    StringStreamBlock* current = this->head;
    while(current != NULL) {
        size += current->size;
        current = current->next;
    }
    return size;
}

char* cjson_impl_string_stream_str(const CJsonImplStringStream* const this) {
    const size_t size = cjson_impl_string_stream_size(this);
    char* buff = (char*) cjson_alloc(this->allocator, (size + 1) * sizeof(char));
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

CJsonStringStream* cjson_string_stream_new(CJsonAllocator* allocator) {
    allocator = cjson_allocator_or_default(allocator);
    CJsonStringStream* stream = (CJsonStringStream*) cjson_alloc(allocator, sizeof(CJsonStringStream));
    stream->_impl = cjson_impl_string_stream_new(allocator);
    stream->_allocator = allocator;
    return stream;
}

void cjson_string_stream_free(CJsonStringStream* this) {
    cjson_impl_string_stream_free(this->_impl);
    cjson_dealloc(this->_allocator, this);
}

void cjson_string_stream_write_bytes(CJsonStringStream* this, const char* data, size_t bytes) {
    cjson_impl_string_stream_write_bytes(this->_impl, data, bytes);
}

void cjson_string_stream_write(CJsonStringStream* this, const char* data) {
    cjson_string_stream_write_bytes(this, data, strlen(data));
}

void cjson_string_stream_write_double(CJsonStringStream* this, const double* val) {
    cjson_impl_string_stream_write_double(this->_impl, val);
}

char* cjson_string_stream_str(const CJsonStringStream* const this) {
    return cjson_impl_string_stream_str(this->_impl);
}
