//
//  cjson_reader.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 25/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_allocator.h"
#include "cjson_array.h"
#include "cjson_object.h"
#include "cjson_reader.h"
#include "cjson_str.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


const size_t k_default_buffer_size = 16384;

const char PUNCTUATION_TOKEN_MAP[] = {
 /* 000 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 016 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 032 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0,
 /* 048 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,
 /* 064 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 080 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  9,  0,  0,
 /* 096 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 112 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  7,  0,  0,
 /* 128 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 144 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 160 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 176 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 192 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 208 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 224 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 240 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

const char BLANK_TOKEN_MAP[] = {
 /* 000 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  0,  0,
 /* 016 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 032 */  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 048 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 064 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 080 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 096 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 112 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 128 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 144 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 160 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 176 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 192 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 208 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 224 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 /* 240 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

typedef enum TokenType {
    cjson_null_token = 1,
    cjson_number_token = 2,
    cjson_str_token = 3,
    cjson_true_token = 4,
    cjson_false_token = 5,
    cjson_left_brace_token = 6,
    cjson_right_brace_token = 7,
    cjson_left_bracket_token = 8,
    cjson_right_bracket_token = 9,
    cjson_comma_token = 10,
    cjson_colon_token = 11
} TokenType;

const char* const TOKEN_NAMES[] = {
    "BAD_TOKEN",
    "NULL_TOKEN",
    "NUMBER_TOKEN",
    "STR_TOKEN",
    "TRUE_TOKEN",
    "FALSE_TOKEN",
    "LEFT_BRACE_TOKEN",
    "RIGHT_BRACE_TOKEN",
    "LEFT_BRACKET_TOKEN",
    "RIGHT_BRACKET_TOKEN",
    "COMMA_TOKEN",
    "COLON_TOKEN"
};

typedef struct Token {
    TokenType type;
    char* data;
    char* end;
} Token;

void token_print(Token* token) {
    if(token == NULL) {
        printf("NULL token\n");
        return;
    }
    printf("Token(type=%s, data='%s', end=%p)\n", TOKEN_NAMES[token->type], token->data, token->end);
}

typedef struct TokenizerContext {
    char* cursor;
    Token* token;
    char* buffer;
    struct CJsonAllocator* allocator;
} TokenizerContext;

TokenizerContext* tokenizer_new(char* data, size_t buffer_sz, CJsonAllocator* allocator) {
    allocator = cjson_allocator_or_default(allocator);
    TokenizerContext* ctx = (TokenizerContext*) cjson_alloc(allocator, sizeof(TokenizerContext));
    ctx->allocator = allocator;
    ctx->cursor = data;
    ctx->buffer = (char*) cjson_alloc(allocator, buffer_sz * sizeof(char));
    memset(ctx->buffer, 0, buffer_sz * sizeof(char));
    Token* token = (Token*) cjson_alloc(allocator, sizeof(Token));
    token->type = cjson_null_token;
    token->data = ctx->buffer;
    token->end = ctx->cursor;
    ctx->token = token;
    return ctx;
}

void tokenizer_free(TokenizerContext* this) {
    cjson_dealloc(this->allocator, this->token);
    cjson_dealloc(this->allocator, this->buffer);
    cjson_dealloc(this->allocator, this);
}

void tokenizer_advance(TokenizerContext* this, size_t bytes) {
    this->cursor += bytes;
}

Token* tokenizer_make_token(TokenizerContext* this, TokenType type, const char* data, size_t bytes, char* end) {
    Token* token = this->token;
    token->type = type;
    memcpy(this->buffer, data, bytes * sizeof(char));
    this->buffer[bytes] = '\0';
    token->end = end;
    return token;
}

Token* tokenizer_make_simple_token(TokenizerContext* this, TokenType type, char* end) {
    Token* token = this->token;
    token->type = type;
    this->buffer[0] = '\0';
    token->end = end;
    return token;
}

bool tokenizer_match(TokenizerContext* this, char* what) {
    char* ptr = this->cursor;
    size_t read_bytes = 0;
    for(;;) {
        if(*what == '\0') {
            tokenizer_advance(this, read_bytes);
            return true;
        }
        if(*what != *ptr) { return false; }
        ++read_bytes;
        ++what;
        ++ptr;
    }
}

Token* tokenizer_try_tokenize_string(TokenizerContext* this) {
    char* ptr = this->cursor;
    if(*ptr++ != '"') { return NULL; }
    for(;;) {
        char c = *ptr;
        if(c == '\0') {
            break;
        }
        else if(c == '\\' && *(ptr + 1) == '"') {
            ptr += 2;
        }
        else if(c == '"') {
            const size_t read_bytes = (ptr - this->cursor) + 1;
            const size_t str_size = read_bytes - 2;
            Token* token = tokenizer_make_token(this, cjson_str_token, this->cursor + 1, str_size, ptr + 1);
            tokenizer_advance(this, read_bytes);
            return token;
        }
        else {
            ++ptr;
        }
    }
    return NULL;
}

Token* tokenizer_try_tokenize_number(TokenizerContext* this) {
    char* ptr = this->cursor;
    if(*ptr == '-') { ++ptr; }
    if(!isdigit(*ptr++)) { return NULL; }
    while(isdigit(*ptr)) { ++ptr; }
    
    if(*ptr == '.') {
        ++ptr;
        if(!isdigit(*ptr++)) {
            return NULL;
        }
        while(isdigit(*ptr)) { ++ptr; }
    }
    
    if(*ptr == 'e' || *ptr == 'E') {
        ++ptr;
        if(*ptr == '-' || *ptr == '+') { ++ptr; }
        if(!isdigit(*ptr++)) {
            return NULL;
        }
        while(isdigit(*ptr)) { ++ptr; }
    }

    const size_t read_bytes = (ptr - this->cursor);
    Token* token = tokenizer_make_token(this, cjson_number_token, this->cursor, read_bytes, ptr + 1);
    tokenizer_advance(this, read_bytes);
    return token;
}

Token* tokenizer_try_tokenize_true(TokenizerContext* this) {
    if(tokenizer_match(this, "true")) {
        return tokenizer_make_simple_token(this, cjson_true_token, this->cursor);
    }
    return NULL;
}

Token* tokenizer_try_tokenize_false(TokenizerContext* this) {
    if(tokenizer_match(this, "false")) {
        return tokenizer_make_simple_token(this, cjson_false_token, this->cursor);
    }
    return NULL;
}

Token* tokenizer_try_tokenize_null(TokenizerContext* this) {
    if(tokenizer_match(this, "null")) {
        return tokenizer_make_simple_token(this, cjson_null_token, this->cursor);
    }
    return NULL;
}

Token* tokenizer_try_tokenize_punctuation(TokenizerContext* this) {
    const int token_type = PUNCTUATION_TOKEN_MAP[(int)*this->cursor];
    if(token_type == 0) {
        return NULL;
    }
    Token* token = tokenizer_make_simple_token(this, (TokenType)token_type, this->cursor + 1);
    ++this->cursor;
    return token;
}

void tokenizer_skip_blank(TokenizerContext* this) {
    while(BLANK_TOKEN_MAP[(int)(*this->cursor)] == 1) {
        ++this->cursor;
    }
}

Token* tokenizer_consume_next(TokenizerContext* this) {
    tokenizer_skip_blank(this);
    char hint = *this->cursor;

    if(PUNCTUATION_TOKEN_MAP[(int)hint] != 0) {
        return tokenizer_try_tokenize_punctuation(this);
    }

    if(hint == '"') {
        return tokenizer_try_tokenize_string(this);
    }

    if(isdigit(hint) || hint == '-') {
        return tokenizer_try_tokenize_number(this);
    }

    if(hint == 't') {
        return tokenizer_try_tokenize_true(this);
    }

    if(hint == 'f') {
        return tokenizer_try_tokenize_false(this);
    }

    if(hint == 'n') {
        return tokenizer_try_tokenize_null(this);
    }

    return NULL;
}

Token* tokenizer_get_next(TokenizerContext* this) {
    TokenizerContext ctx_copy = {
        .cursor = this->cursor,
        .token = this->token,
        .buffer = this->buffer
    };
    return tokenizer_consume_next(&ctx_copy);
}

void tokenizer_consume_token(TokenizerContext* this, const Token* token) {
    this->cursor = token->end;
}

CJsonValue* cjson_read_value(TokenizerContext* ctx, CJsonAllocator* allocator);

CJsonObject* cjson_read_object(TokenizerContext* ctx, CJsonAllocator* allocator) {
    CJsonObject* object = cjson_object_new(allocator);
    bool has_trailing_comma = false;
    for(;;) {
        Token* token = tokenizer_consume_next(ctx);
        if(token == NULL) {
            return NULL;
        }

        if(token->type == cjson_right_brace_token) {
            if(has_trailing_comma) {
                return NULL;
            }
            return object;
        }
        if(token->type == cjson_comma_token) {
            if(has_trailing_comma) {
                return NULL;
            }
            has_trailing_comma = true;
            continue;
        }
        if(token->type != cjson_str_token) {
            return NULL;
        }
        char* key = cjson_raw_str_copy(token->data, allocator);
        {
            Token* colon_token = tokenizer_consume_next(ctx);
            const TokenType colon_token_type = colon_token->type;
            if(colon_token_type != cjson_colon_token) {
                return NULL;
            }
        }
        CJsonValue* val = cjson_read_value(ctx, allocator);
        if(val == NULL) {
            return NULL;
        }
        cjson_object_set(object, key, val);
        cjson_dealloc(allocator, key);
        has_trailing_comma = false;
    }
}

CJsonArray* cjson_read_array(TokenizerContext* ctx, CJsonAllocator* allocator) {
    CJsonArray* array = cjson_array_new(allocator);
    bool has_trailing_comma = false;
    for(;;) {
        Token* token = tokenizer_get_next(ctx);
        if(token == NULL) {
            return NULL;
        }

        if(token->type == cjson_right_bracket_token) {
            tokenizer_consume_token(ctx, token);
            if(has_trailing_comma) {
                return NULL;
            }
            return array;
        }
        if(token->type == cjson_comma_token) {
            tokenizer_consume_token(ctx, token);
            if(has_trailing_comma) {
                return NULL;
            }
            has_trailing_comma = true;
            continue;
        }
        CJsonValue* val = cjson_read_value(ctx, allocator);
        if(val == NULL) {
            return NULL;
        }
        cjson_array_push(array, val);
        has_trailing_comma = false;
    }
}

CJsonValue* cjson_read_value(TokenizerContext* ctx, CJsonAllocator* allocator) {
    Token* token = tokenizer_consume_next(ctx);
    if(token == NULL) {
        return NULL;
    }

    CJsonValue* value = NULL;
    const TokenType token_type = token->type;
    switch(token_type) {
        case cjson_null_token: { value = cjson_value_new_as_null(allocator); break; }
        case cjson_str_token: {
            CJsonStr* str = cjson_str_new_from_raw(token->data, allocator);
            value = cjson_value_new_as_str(str, allocator);
            break;
        }
        case cjson_number_token: {
            const double number_val = strtod(token->data, NULL);
            value = cjson_value_new_as_number(number_val, allocator);
            break;
        }
        case cjson_true_token: {
            value = cjson_value_new_as_bool(true, allocator);
            break;
        }
        case cjson_false_token: {
            value = cjson_value_new_as_bool(false, allocator);
            break;
        }
        case cjson_left_brace_token: {
            CJsonObject* object = cjson_read_object(ctx, allocator);
            if(object == NULL) { return NULL; }
            value = cjson_value_new_as_object(object, allocator);
            break;
        }
        case cjson_left_bracket_token: {
            CJsonArray* array = cjson_read_array(ctx, allocator);
            if(array == NULL) { return NULL; }
            value = cjson_value_new_as_array(array, allocator);
            break;
        }
        default:
            break;
    }
    if(value == NULL) {

    }
    return value;
}

CJsonValue* cjson_read_impl(char* data, CJsonAllocator* allocator) {
    TokenizerContext* ctx = tokenizer_new(data, k_default_buffer_size, allocator);
    CJsonValue* value = cjson_read_value(ctx, allocator);
    tokenizer_free(ctx);
    return value;
}

CJsonValue* cjson_read_test(char* data, CJsonAllocator* allocator) {
    TokenizerContext* ctx = tokenizer_new(data, k_default_buffer_size, allocator);
    for(;;) {
        Token* token = tokenizer_consume_next(ctx);
        token_print(token);
        if(token == NULL) {
            break;
        }
    }
    return NULL;
}

CJsonValue* cjson_read(char* data, CJsonAllocator* allocator) {
    return cjson_read_impl(data, allocator);
    //return cjson_read_test(data);
}
