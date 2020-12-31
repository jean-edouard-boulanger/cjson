//
//  cjson_stringstream.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 23/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_stringstream_h
#define cjson_stringstream_h

#include <stdlib.h>


typedef struct CJsonImplStringStream CJsonImplStringStream;
typedef struct CJsonAllocator CJsonAllocator;

typedef struct CJsonStringStream {
    CJsonImplStringStream* _impl;
    CJsonAllocator* _allocator;
} CJsonStringStream;

CJsonStringStream* cjson_string_stream_new(CJsonAllocator* allocator);
void cjson_string_stream_free(CJsonStringStream* this);
void cjson_string_stream_write(CJsonStringStream* this, const char* data);
void cjson_string_stream_write_double(CJsonStringStream* this, const double* val);
void cjson_string_stream_write_bytes(CJsonStringStream* this, const char* data, size_t bytes);
char* cjson_string_stream_str(const CJsonStringStream* this);

#endif /* cjson_stringstream_h */
