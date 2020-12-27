//
//  cjson_assert.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_assert_h
#define cjson_assert_h

#include <stdio.h>
#include <stdbool.h>

#ifdef CJSON_ENABLE_STACKTRACE
#define CJSON_DEBUG_STACKTRACE cjson_impl_print_stacktrace()
#else
#define CJSON_DEBUG_STACKTRACE
#endif

#define CJSON_ABORT(msg) \
    fprintf(stderr, msg); \
    CJSON_DEBUG_STACKTRACE \
    abort();

#define CJSON_ASSERT_MSG(x, m) \
    do { \
        if(!(x)) { \
            CJSON_ABORT(m ": " #x "\n"); \
        } \
    } while (false)

#define CJSON_ASSERT(x) CJSON_ASSERT_MSG(x, "assertion failed")

#define CJSON_CHECK_ALLOC(x) CJSON_ASSERT_MSG(x != NULL, "bad alloc")

#define CJSON_STATIC_ASSERT(x) _Static_assert(x, "static assertion failed: " #x)

void cjson_impl_print_stacktrace(void);

#endif /* cjson_assert_h */
