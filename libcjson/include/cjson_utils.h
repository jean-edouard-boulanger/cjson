//
//  cjson_utils.h
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 23/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#ifndef cjson_utils_h
#define cjson_utils_h

#include <stdarg.h>

#define CJSON_NUMARGS(...) (sizeof((void*[]){__VA_ARGS__})/sizeof(void*))

#define CJSON_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CJSON_MAX(a, b) (((a) > (b)) ? (a) : (b))

int cjson_mod(int x, int n);

#endif /* cjson_utils_h */
