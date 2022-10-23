#pragma once

#include <stdbool.h>
#include "token.h"
#include "types.h"
#include "vector.h"

typedef struct node node_t;
struct node {
    enum node_name name;
    bool isToken;
    union {
        token_t token;
        vector_t *children;
    };
};

node_t *nodeWithChildren(enum node_name name);

node_t *nodeFromToken(enum node_name name, token_t token);

void free_node(node_t *node);
void free_nodeWithOutChildren(node_t *node);

bool hasToken(node_t *self);
