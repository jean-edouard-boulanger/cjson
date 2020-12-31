//
// Created by Jean-Edouard BOULANGER on 28/12/2020.
//

#ifndef CJSON_CJSON_ALLOCATOR_H
#define CJSON_CJSON_ALLOCATOR_H

#include <stdlib.h>


typedef struct CJsonAllocator {
    void* (*alloc)(void* context, size_t size);
    void* (*realloc)(void* context, void* address, size_t size);
    void (*dealloc)(void* context, void* address);
    void* context;
} CJsonAllocator;

CJsonAllocator* cjson_allocator_get_default();

CJsonAllocator* cjson_allocator_or_default(CJsonAllocator* allocator);

CJsonAllocator* cjson_allocator_stack_new(size_t size);

void cjson_allocator_stack_free(CJsonAllocator* allocator);

void* cjson_alloc(CJsonAllocator* this, size_t size);

void* cjson_realloc(CJsonAllocator* this, void* address, size_t size);

void cjson_dealloc(CJsonAllocator* this, void* address);

#endif //CJSON_CJSON_ALLOCATOR_H
