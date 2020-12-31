//
// Created by Jean-Edouard BOULANGER on 28/12/2020.
//

#include "cjson_allocator.h"

#include <stdlib.h>
#include <string.h>


void* cjson_default_alloc(void* context, size_t size) {
    return malloc(size);
}

void* cjson_default_realloc(void* context, void* address, size_t size) {
    return realloc(address, size);
}

void cjson_default_dealloc(void* context, void* address) {
    free(address);
}

typedef struct CJsonStackAllocatorContext {
    void* pool;
    void* head;
    size_t size;
    size_t capacity;
} CJsonStackAllocatorContext;

typedef struct CJsonStackAllocatorBlockHeader {
    void* start_ptr;
    void* end_ptr;
} CJsonStackAllocatorBlockHeader;

CJsonStackAllocatorContext* cjson_stack_allocator_context_new(size_t pool_size) {
    CJsonStackAllocatorContext* this = (CJsonStackAllocatorContext*) malloc(sizeof(CJsonStackAllocatorContext));
    this->pool = malloc(pool_size);
    this->head = this->pool;
    this->size = 0;
    this->capacity = pool_size;
    return this;
}

void cjson_stack_allocator_context_free(CJsonStackAllocatorContext* this) {
    free(this->pool);
    free(this);
}

void* cjson_stack_allocator_alloc(void* context, size_t size) {
    CJsonStackAllocatorContext* this = (CJsonStackAllocatorContext*) context;
    const size_t block_size = sizeof(CJsonStackAllocatorBlockHeader) + size;
    if(block_size > (this->capacity - this->size)) { return NULL; }
    void* header_ptr = this->head;
    void* alloc_ptr = header_ptr + sizeof(CJsonStackAllocatorBlockHeader);
    CJsonStackAllocatorBlockHeader* block_header = (CJsonStackAllocatorBlockHeader*) header_ptr;
    block_header->start_ptr = alloc_ptr;
    block_header->end_ptr = alloc_ptr + size;
    this->head += block_size;
    return alloc_ptr;
}

void cjson_stack_allocator_dealloc(void* context, void* address) {}

void* cjson_stack_allocator_realloc(void* context, void* address, size_t new_size) {
    CJsonStackAllocatorContext* this = (CJsonStackAllocatorContext*) context;
    CJsonStackAllocatorBlockHeader* header_ptr = address - sizeof(CJsonStackAllocatorBlockHeader);
    const size_t previous_size = header_ptr->end_ptr - header_ptr->start_ptr;
    if(new_size < previous_size) { return header_ptr->start_ptr; }
    else if(this->head == header_ptr->end_ptr) {
        const size_t deficit = new_size - previous_size;
        if(this->capacity < deficit) { return NULL; }
        this->head += (new_size - previous_size);
        header_ptr->end_ptr = this->head;
        return header_ptr->start_ptr;
    }
    void* new_ptr = cjson_stack_allocator_alloc(context, new_size);
    memcpy(new_ptr, header_ptr->start_ptr, previous_size);
    return new_ptr;
}

CJsonAllocator* cjson_allocator_stack_new(size_t size) {
    CJsonAllocator* allocator = (CJsonAllocator*) malloc(sizeof(CJsonAllocator));
    allocator->alloc = cjson_stack_allocator_alloc;
    allocator->dealloc = cjson_stack_allocator_dealloc;
    allocator->realloc = cjson_stack_allocator_realloc;
    allocator->context = cjson_stack_allocator_context_new(size);
    return allocator;
}

void cjson_allocator_stack_free(CJsonAllocator* allocator) {
    cjson_stack_allocator_context_free((CJsonStackAllocatorContext*) allocator->context);
    free(allocator);
}

CJsonAllocator* cjson_allocator_get_default() {
    static CJsonAllocator s_allocator = {
        .alloc = cjson_default_alloc,
        .realloc = cjson_default_realloc,
        .dealloc = cjson_default_dealloc,
        .context = NULL
    };
    return &s_allocator;
}

CJsonAllocator* cjson_allocator_or_default(CJsonAllocator* allocator) {
    if(allocator == NULL) { return cjson_allocator_get_default(); }
    return allocator;
}

void* cjson_alloc(CJsonAllocator* this, size_t size) {
    this = cjson_allocator_or_default(this);
    return this->alloc(this->context, size);
}

void* cjson_realloc(CJsonAllocator* this, void* address, size_t size) {
    this = cjson_allocator_or_default(this);
    return this->realloc(this->context, address, size);
}

void cjson_dealloc(CJsonAllocator* this, void* address) {
    this = cjson_allocator_or_default(this);
    this->dealloc(this->context, address);
}
