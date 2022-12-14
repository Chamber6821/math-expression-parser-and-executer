#pragma once

#include "utils.h"

enum token_type {
    NUMBER,
    OPEN_PAR,
    CLOSE_PAR,
    END,
    BIN_ADD,
    BIN_SUB,
    BIN_MULTIPLY,
    BIN_DIVIDE,
    FUNCTION_NAME,
};

typedef double token_number_t;

typedef struct token token_t;
struct token {
    enum token_type type;
    union {
        token_number_t number;
        char function[STATIC_MAX(4, sizeof(token_number_t))];
    };
};

token_t onlyType(enum token_type type);
token_t number(token_number_t number);
