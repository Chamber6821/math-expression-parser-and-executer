#include <malloc.h>
#include <assert.h>
#include "node.h"

void free_node(node_t *node) { // NOLINT(misc-no-recursion)
    switch (node->type) {
        case NT_ADD:
        case NT_MULTIPLY: {
            free_node(node->children[0]);
            free_node(node->children[1]);
            free(node);
            break;
        }
        case NT_TOKEN: {
            free(node);
            break;
        }
    }
}

enum node_type operatorToNodeType(enum token_type operator) {
    switch (operator) {
        case BIN_ADD: return NT_ADD;
        case BIN_MULTIPLY: return NT_MULTIPLY;
        default: {
            assert(!"NOT DEFINED");
            abort();
        }
    }
}

node_t *makeOperator(enum token_type type, node_t *left, node_t *right) {
    node_t *new = malloc(sizeof(node_t));
    new->type = operatorToNodeType(type);
    new->children[0] = left;
    new->children[1] = right;
    return new;
}

node_t *nodeFromToken(token_t token) {
    node_t *new = malloc(sizeof(node_t));
    new->type = NT_TOKEN;
    new->token = token;
    return new;
}
