#include <malloc.h>
#include "node.h"

node_t *new_node(enum node_name name) {
    node_t *new = malloc(sizeof(node_t));
    new->name = name;
    new->isToken = false;
    new->children = NULL;
    return new;
}

node_t *nodeWithChildren(enum node_name name) {
    node_t *new = new_node(name);
    new->isToken = false;
    new->children = new_vector();
    return new;
}

node_t *nodeFromToken(enum node_name name, token_t token) {
    node_t *new = new_node(name);
    new->token = token;
    new->isToken = true;
    return new;
}

void free_node(node_t *node) { // NOLINT(misc-no-recursion)
    if (node == NULL) return;

    if (hasToken(node)) {
        free(node);
        return;
    }

    for (int i = 0; i < node->children->size; i++) {
        free_node(node->children->items[i]);
    }
    free_vector(node->children);
    free(node);
}

void free_nodeWithOutChildren(node_t *node) {
    if (node == NULL) return;

    if (!hasToken(node)) free_vector(node->children);
    free(node);
}

bool hasToken(node_t *self) {
    return self->isToken;
}
