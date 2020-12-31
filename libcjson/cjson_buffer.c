//
// Created by Jean-Edouard BOULANGER on 29/12/2020.
//

#include "cjson_allocator.h"
#include "cjson_buffer.h"

#include <string.h>


CJsonBuffer* cjson_buffer_new(size_t size, CJsonAllocator* allocator) {
    CJsonBuffer* buffer = (CJsonBuffer*) cjson_alloc(allocator, sizeof(CJsonBuffer));
    buffer->size = size;
    buffer->buffer = (char*) cjson_alloc(allocator, size * sizeof(char));
    memset(buffer->buffer, 0, size * sizeof(char));
    buffer->_allocator = allocator;
    return buffer;
}

void cjson_buffer_free(CJsonBuffer* this) {
    cjson_dealloc(this->_allocator, this->buffer);
    cjson_dealloc(this->_allocator, this);
}

void cjson_buffer_resize(CJsonBuffer* this, size_t size) {
    this->size = size;
    this->buffer = cjson_realloc(this->_allocator, this->buffer, size);
}
