#pragma once

#include "token.h"

typedef struct node node_t;

typedef token_t stack_element_t;
typedef node_t *vector_element_t;

enum node_name {
    NN_COMPOSITE,
    NN_TOKEN,
    NN_EXPRESSION,
    NN_TERM,
    NN_FACTOR,
    NN_PRIMARY,
    NN_FUNCTION,
    NN_COUNT_OF_NAMES,
};

