//
//  cjson_reader.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 25/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_reader_h
#define cjson_reader_h

#include "cjson_value.h"


typedef struct CJsonAllocator CJsonAllocator;

CJsonValue* cjson_read(char* data, CJsonAllocator* allocator);

#endif /* cjson_reader_h */
