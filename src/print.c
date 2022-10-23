#include "print.h"
#include "to-string.h"

void printToken(token_t token, FILE *out) {
    char buffer[100];
    fprintf(out, "%s", tokenToString(token, buffer));
}

void printNode(node_t *node, FILE *out) { // NOLINT(misc-no-recursion)
    if (node == NULL) {
        fprintf(out, "NULL");
        return;
    }

    if (hasToken(node)) {
        printToken(node->token, out);
        return;
    }

    if (node->children->size == 0) {
        fprintf(out, "[]");
        return;
    }

    fprintf(out, "[");
    printNode(node->children->items[0], out);
    for (int i = 1; i < node->children->size; i++) {
        fprintf(out, " ");
        printNode(node->children->items[i], out);
    }
    fprintf(out, "]");
}
