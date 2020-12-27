//
//  cjson_reader.c
//  cjson
//
//  Created by Jean-Edouard BOULANGER on 25/12/2020.
//  Copyright © 2020 Jean-Edouard BOULANGER. All rights reserved.
//

#include "cjson_array.h"
#include "cjson_assert.h"
#include "cjson_object.h"
#include "cjson_reader.h"
#include "cjson_str.h"

#include <stdlib.h>
#include <string.h>


bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

typedef enum TokenType {
    cjson_null_token = 0,
    cjson_number_token,
    cjson_str_token,
    cjson_bool_token,
    cjson_left_brace_token,
    cjson_right_brace_token,
    cjson_left_bracket_token,
    cjson_right_bracket_token,
    cjson_comma_token,
    cjson_colon_token
} TokenType;

const char* const TOKEN_NAMES[] = {
    "NULL_TOKEN",
    "NUMBER_TOKEN",
    "STR_TOKEN",
    "BOOL_TOKEN",
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

Token* token_new(TokenType type, char* data, char* end) {
    Token* token = (Token*) malloc(sizeof(Token));
    token->type = type;
    token->data = data;
    token->end = end;
    return token;
}

void token_free(Token* token) {
    free(token);
}

typedef struct TokenizerContext {
    char* cursor;
} TokenizerContext;

TokenizerContext* tokenizer_new(char* data) {
    TokenizerContext* ctx = (TokenizerContext*) malloc(sizeof(TokenizerContext));
    ctx->cursor = data;
    return ctx;
}

void tokenizer_advance_cursor(TokenizerContext* this, size_t bytes) {
    for(size_t i = 0; i < bytes; ++i) {
        if(*this->cursor == '\0') {
            return;
        }
        ++this->cursor;
    }
}

bool tokenizer_advance_if_matches(TokenizerContext* this, char* what) {
    char* ptr = this->cursor;
    size_t read_bytes = 0;
    for(;;) {
        if(*what == '\0') {
            tokenizer_advance_cursor(this, read_bytes);
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
            CJSON_ASSERT(read_bytes >= 2);
            const size_t str_size = read_bytes - 2;
            char* buffer = (char*) malloc((str_size + 1) * sizeof(char));
            buffer[str_size] = '\0';
            memcpy(buffer, this->cursor + 1, str_size);
            tokenizer_advance_cursor(this, read_bytes);
            return token_new(cjson_str_token, buffer, ptr + 1);
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
    if(!is_digit(*ptr)) { return NULL; }
    while(is_digit(*ptr)) { ++ptr; }
    
    if(*ptr == '.') {
        ++ptr;
        if(!is_digit(*ptr)) {
            return NULL;
        }
        while(is_digit(*ptr)) { ++ptr; }
    }
    
    if(*ptr == 'e' || *ptr == 'E') {
        ++ptr;
        if(*ptr == '-' || *ptr == '+') { ++ptr; }
        if(!is_digit(*ptr)) {
            return NULL;
        }
        while(is_digit(*ptr)) { ++ptr; }
    }

    const size_t read_bytes = (ptr - this->cursor);
    char* buffer = (char*) malloc((read_bytes + 1) * sizeof(char));
    buffer[read_bytes] = '\0';
    memcpy(buffer, this->cursor, read_bytes);
    tokenizer_advance_cursor(this, read_bytes);
    return token_new(cjson_number_token, buffer, this->cursor);
}

Token* tokenizer_try_tokenize_bool(TokenizerContext* this) {
    if(tokenizer_advance_if_matches(this, "true")) {
        return token_new(cjson_bool_token, "true", this->cursor);
    }
    if(tokenizer_advance_if_matches(this, "false")) {
        return token_new(cjson_bool_token, "false", this->cursor);
    }
    return NULL;
}

Token* tokenizer_try_tokenize_null(TokenizerContext* this) {
    if(tokenizer_advance_if_matches(this, "null")) {
        return token_new(cjson_null_token, "null", this->cursor);
    }
    return NULL;
}

Token* tokenizer_try_tokenize_punctuation(TokenizerContext* this) {
    Token* token = NULL;
    char* end = this->cursor + 1;
    switch(*this->cursor) {
        case ',': { token = token_new(cjson_comma_token, ",", end); break; }
        case ':': { token = token_new(cjson_colon_token, ":", end); break; }
        case '[': { token = token_new(cjson_left_bracket_token, "[", end); break; }
        case ']': { token = token_new(cjson_right_bracket_token, "]", end); break; }
        case '{': { token = token_new(cjson_left_brace_token, "{", end); break; }
        case '}': { token = token_new(cjson_right_brace_token, "}", end); break; }
        default: return NULL;
    }
    tokenizer_advance_cursor(this, 1);
    return token;
}

void tokenizer_skip_blank(TokenizerContext* this) {
    for(;;) {
        char c = *this->cursor;
        if(c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            return;
        }
        tokenizer_advance_cursor(this, 1);
    }
}

Token* tokenizer_get_next(TokenizerContext* this) {
    TokenizerContext new_ctx = {.cursor = this->cursor};
    
    tokenizer_skip_blank(&new_ctx);
    Token* token = NULL;

    token = tokenizer_try_tokenize_punctuation(&new_ctx);
    if(token != NULL) { return token; }

    token = tokenizer_try_tokenize_string(&new_ctx);
    if(token != NULL) { return token; }

    token = tokenizer_try_tokenize_number(&new_ctx);
    if(token != NULL) { return token; }

    token = tokenizer_try_tokenize_bool(&new_ctx);
    if(token != NULL) { return token; }

    token = tokenizer_try_tokenize_null(&new_ctx);
    if(token != NULL) { return token; }

    return NULL;
}

Token* tokenizer_consume_next(TokenizerContext* this) {
    Token* token = tokenizer_get_next(this);
    if(token == NULL) {
        return NULL;
    }
    this->cursor = token->end;
    return token;
}

void tokenizer_consume_token(TokenizerContext* this, const Token* token) {
    this->cursor = token->end;
}

CJsonValue* cjson_read_value(TokenizerContext* ctx);

CJsonObject* cjson_read_object(TokenizerContext* ctx) {
    CJsonObject* object = cjson_object_new();
    bool has_trailing_comma = false;
    for(;;) {
        Token* token = tokenizer_consume_next(ctx);
        if(token == NULL) {
            return NULL;
        }

        if(token->type == cjson_right_brace_token) {
            token_free(token);
            if(has_trailing_comma) {
                return NULL;
            }
            return object;
        }
        if(token->type == cjson_comma_token) {
            token_free(token);
            if(has_trailing_comma) {
                return NULL;
            }
            has_trailing_comma = true;
            continue;
        }
        if(token->type != cjson_str_token) {
            token_free(token);
            return NULL;
        }
        const char* key = token->data;
        {
            Token* colon_token = tokenizer_consume_next(ctx);
            const TokenType colon_token_type = colon_token->type;
            token_free(colon_token);
            if(colon_token_type != cjson_colon_token) {
                token_free(token);
                return NULL;
            }
        }
        CJsonValue* val = cjson_read_value(ctx);
        if(val == NULL) {
            token_free(token);
            return NULL;
        }
        cjson_object_set(object, key, val);
        has_trailing_comma = false;
    }
}

CJsonArray* cjson_read_array(TokenizerContext* ctx) {
    CJsonArray* array = cjson_array_new();
    bool has_trailing_comma = false;
    for(;;) {
        Token* token = tokenizer_get_next(ctx);
        if(token == NULL) {
            return NULL;
        }
        if(token->type == cjson_right_bracket_token) {
            token_free(token);
            tokenizer_consume_token(ctx, token);
            if(has_trailing_comma) {
                return NULL;
            }
            return array;
        }
        if(token->type == cjson_comma_token) {
            tokenizer_consume_token(ctx, token);
            token_free(token);
            if(has_trailing_comma) {
                return NULL;
            }
            has_trailing_comma = true;
            continue;
        }
        token_free(token);
        CJsonValue* val = cjson_read_value(ctx);
        if(val == NULL) {
            return NULL;
        }
        cjson_array_push(array, val);
        has_trailing_comma = false;
    }
}

CJsonValue* cjson_read_value(TokenizerContext* ctx) {
    Token* token = tokenizer_consume_next(ctx);
    if(token == NULL) {
        return NULL;
    }

    CJsonValue* value = NULL;
    const TokenType token_type = token->type;
    switch(token_type) {
        case cjson_null_token: { value = cjson_value_new_as_null(); break; }
        case cjson_str_token: {
            CJsonStr* str = cjson_str_new_from_raw(token->data);
            value = cjson_value_new_as_str(str);
            break;
        }
        case cjson_number_token: {
            const double number_val = strtod(token->data, NULL);
            value = cjson_value_new_as_number(number_val);
            break;
        }
        case cjson_bool_token: {
            const bool bool_value = strcmp(token->data, "true") == 0;
            value = cjson_value_new_as_bool(bool_value);
            break;
        }
        case cjson_left_brace_token: {
            CJsonObject* object = cjson_read_object(ctx);
            if(object == NULL) { return NULL; }
            value = cjson_value_new_as_object(object);
            break;
        }
        case cjson_left_bracket_token: {
            CJsonArray* array = cjson_read_array(ctx);
            if(array == NULL) { return NULL; }
            value = cjson_value_new_as_array(array);
            break;
        }
        default:
            break;
    }
    token_free(token);
    return value;
}

CJsonValue* cjson_read(char* data) {
    TokenizerContext* ctx = tokenizer_new(data);
    return cjson_read_value(ctx);
}
