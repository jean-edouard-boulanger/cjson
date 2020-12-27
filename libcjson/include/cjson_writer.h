//
//  cjson_writer.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 24/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_writer_h
#define cjson_writer_h


typedef struct CJsonValue CJsonValue;

void cjson_print(const CJsonValue* value);

char* cjson_to_str(const CJsonValue* value);

#endif /* cjson_writer_h */
