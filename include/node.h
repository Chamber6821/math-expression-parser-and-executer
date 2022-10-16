#pragma once

#include "token.h"

enum node_type {
    NT_ADD, NT_MULTIPLY, NT_TOKEN
};

typedef struct node node_t;
struct node {
    enum node_type type;
    union {
        token_t token;
        node_t *children[2];
    };
};

void free_node(node_t *node);

enum node_type operatorToNodeType(enum token_type operator);

node_t *makeOperator(enum token_type type, node_t *left, node_t *right);

node_t *nodeFromToken(token_t token);
