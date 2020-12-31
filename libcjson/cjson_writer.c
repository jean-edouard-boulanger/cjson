//
//  cjson_writer.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 24/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_allocator.h"
#include "cjson_stringstream.h"
#include "cjson_value.h"
#include "cjson_writer.h"

#include <stdio.h>


char* cjson_to_str(const CJsonValue* const value, CJsonAllocator* allocator) {
    CJsonStringStream* stream = cjson_string_stream_new(allocator);
    cjson_value_fmt(stream, value);
    char* buff = cjson_string_stream_str(stream);
    cjson_string_stream_free(stream);
    return buff;
}

void cjson_print(const CJsonValue* const value, CJsonAllocator* allocator) {
    char* buff = cjson_to_str(value, allocator);
    printf("%s\n", buff);
    free(buff);
}
