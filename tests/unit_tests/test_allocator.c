//
// Created by Jean-Edouard BOULANGER on 01/01/2021.
//

#include "cases.h"

#include <cjson_allocator.h>

#include <stdbool.h>


typedef struct TestAllocatorContext {
    size_t alloc_calls;
    size_t last_alloc_call_size_arg;
    void* last_alloc_call_return;
    size_t realloc_calls;
    void* last_realloc_call_address_arg;
    size_t last_realloc_call_size_arg;
    void* last_realloc_call_return;
    size_t dealloc_calls;
    void* last_dealloc_call_address_arg;
} TestAllocatorContext;

void* test_allocator_alloc(void* ctx, size_t size) {
    static const char dummy[] = "dummy";
    TestAllocatorContext* this = (TestAllocatorContext*) ctx;
    void* dummy_alloc = (void*) dummy;
    this->alloc_calls += 1;
    this->last_alloc_call_size_arg = size;
    this->last_alloc_call_return = dummy_alloc;
    return dummy_alloc;
}

void* test_allocator_realloc(void* ctx, void* address, size_t size) {
    static const char dummy[] = "dummy";
    TestAllocatorContext* this = (TestAllocatorContext*) ctx;
    void* dummy_alloc = (void*) dummy;
    this->realloc_calls += 1;
    this->last_realloc_call_address_arg = address;
    this->last_realloc_call_size_arg = size;
    this->last_realloc_call_return = dummy_alloc;
    return dummy_alloc;
}

void test_allocator_dealloc(void* ctx, void* address) {
    TestAllocatorContext* this = (TestAllocatorContext*) ctx;
    this->dealloc_calls += 1;
    this->last_dealloc_call_address_arg = address;
}

void reset_test_allocator_context(TestAllocatorContext* context) {
    context->alloc_calls = 0;
    context->last_alloc_call_size_arg = 0;
    context->last_alloc_call_return = NULL;
    context->dealloc_calls = 0;
    context->last_dealloc_call_address_arg = NULL;
    context->realloc_calls = 0;
    context->last_realloc_call_address_arg = NULL;
    context->last_realloc_call_size_arg = 0;
    context->last_realloc_call_return = NULL;
}

TestAllocatorContext* get_test_allocator_context(bool reset) {
    static TestAllocatorContext ctx = {};
    if(reset) {
        reset_test_allocator_context(&ctx);
    }
    return &ctx;
}

CJsonAllocator* get_test_allocator(bool reset) {
    static CJsonAllocator allocator = {
        .context = NULL,
        .alloc = test_allocator_alloc,
        .dealloc = test_allocator_dealloc,
        .realloc = test_allocator_realloc
    };
    if(allocator.context == NULL || reset) {
        allocator.context = get_test_allocator_context(reset);
    }
    return &allocator;
}


START_TEST(test) {
    CJsonAllocator* allocator = get_test_allocator(true);
    ck_assert_ptr_nonnull(allocator);
    TestAllocatorContext* ctx = (TestAllocatorContext*) allocator->context;
    ck_assert_ptr_nonnull(ctx);

    void* ptr1 = cjson_alloc(allocator, 10);
    ck_assert_ptr_nonnull(ptr1);
    ck_assert_int_eq(ctx->alloc_calls, 1);
    ck_assert_int_eq(ctx->last_alloc_call_size_arg, 10);
    ck_assert_ptr_eq(ctx->last_alloc_call_return, ptr1);

    void* ptr2 = cjson_realloc(allocator, ptr1, 20);
    ck_assert_ptr_nonnull(ptr2);
    ck_assert_int_eq(ctx->realloc_calls, 1);
    ck_assert_ptr_eq(ctx->last_realloc_call_address_arg, ptr1);
    ck_assert_int_eq(ctx->last_realloc_call_size_arg, 20);
    ck_assert_ptr_eq(ctx->last_realloc_call_return, ptr2);

    cjson_dealloc(allocator, ptr2);
    ck_assert_int_eq(ctx->dealloc_calls, 1);
    ck_assert_ptr_eq(ctx->last_dealloc_call_address_arg, ptr2);
}

void allocator_case_setup(Suite* suite) {
    TCase* allocator_case = tcase_create("allocator");
    suite_add_tcase(suite, allocator_case);

    tcase_add_test(allocator_case, test);
}
