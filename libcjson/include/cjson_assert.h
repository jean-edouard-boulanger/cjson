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


#define CJSON_IMPL_NOOP do {} while(false)

#ifdef CJSON_ENABLE_STACKTRACE
#define CJSON_DEBUG_STACKTRACE cjson_impl_print_stacktrace()
#else
#define CJSON_DEBUG_STACKTRACE CJSON_IMPL_NOOP
#endif

#define CJSON_ABORT(...) \
    do { \
        fprintf(stderr, __VA_ARGS__); \
        CJSON_DEBUG_STACKTRACE; \
        abort(); \
    } while (false)

#define CJSON_ASSERT_MSG(x, m) \
    do { \
        if(!(x)) { \
            CJSON_ABORT("%s: %s\n", m, #x); \
        } \
    } while (false)

#ifndef CJSON_CONTRACT
#define CJSON_CONTRACT(x) CJSON_ASSERT_MSG(x, "contract violation")
#endif

#ifndef CJSON_ASSERT
#define CJSON_ASSERT(x) CJSON_ASSERT_MSG(x, "assertion failed")
#endif

#ifdef CJSON_ENABLE_SAFE_ASSERTS
#define CJSON_SAFE_ASSERT(x) CJSON_ASSERT(x)
#else
#define CJSON_SAFE_ASSERT(x) CJSON_IMPL_NOOP
#endif

#define CJSON_STATIC_ASSERT(x) _Static_assert(x, "static assertion failed: " #x)

void cjson_impl_print_stacktrace(void);

#endif /* cjson_assert_h */
