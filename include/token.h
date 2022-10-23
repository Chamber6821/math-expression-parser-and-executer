#pragma once

enum token_type {
    NUMBER,
    OPEN_PAR,
    CLOSE_PAR,
    END,
    BIN_ADD,
    BIN_SUB,
    BIN_MULTIPLY,
    BIN_DIVIDE,
    BIN_MODE,
};

typedef struct token token_t;
struct token {
    enum token_type type;
    union {
        int number;
    };
};

token_t onlyType(enum token_type type);
token_t number(int number);
