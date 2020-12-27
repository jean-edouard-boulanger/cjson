//
//  cjson_assert.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 13/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_assert.h"

#include <stdlib.h>
#include <execinfo.h>


void cjson_impl_print_stacktrace() {
    void *array[10];
    char **strings;
    int size, i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);
    if (strings != NULL)
    {
        fprintf(stderr, "Obtained %d stack frames.\n", size);
        for (i = 0; i < size; i++) {
            fprintf(stderr, "%s\n", strings[i]);
        }
    }
    free(strings);
}
