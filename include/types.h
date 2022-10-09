#pragma once

enum lex_type {
    NUMBER, BINARY_OPERATOR
};
enum binary_operator {
    SUM, MULTIPLY
};

typedef struct lex lex_t;
typedef struct lex stack_element_t;

struct lex {
    enum lex_type type;
    union {
        int number;
        enum binary_operator binaryOperator;
    };
};
