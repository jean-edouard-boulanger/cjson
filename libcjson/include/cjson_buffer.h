//
// Created by Jean-Edouard BOULANGER on 29/12/2020.
//

#ifndef CJSON_CJSON_BUFFER_H
#define CJSON_CJSON_BUFFER_H

#include <stdlib.h>


typedef struct CJsonAllocator CJsonAllocator;

typedef struct CJsonBuffer {
    char* buffer;
    size_t size;

    CJsonAllocator* _allocator;
} CJsonBuffer;


CJsonBuffer* cjson_buffer_new(size_t size, CJsonAllocator* allocator);
void cjson_buffer_free(CJsonBuffer* this);

void cjson_buffer_resize(CJsonBuffer* this, size_t new_size);

#endif //CJSON_CJSON_BUFFER_H
