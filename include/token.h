#pragma once

enum token_type {
    NUMBER,
    BIN_OPERATOR,
    //UNAR_OPERATOR,
    OPEN_PAR,
    CLOSE_PAR,
    END,
};

enum bin_operator {
    ADD, /*SUB,*/ MULTIPLY, /*DIVIDE,*/
};

enum unar_operator {
    NEGATIVE
};

typedef struct token token_t;
struct token {
    enum token_type type;
    union {
        int number;
        enum bin_operator binOperator;
        enum unar_operator unarOperator;
    };
};

token_t onlyType(enum token_type type);
token_t number(int number);
token_t binOperator(enum bin_operator operator);
