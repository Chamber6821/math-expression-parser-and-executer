#include "print.h"
#include "to-string.h"

void printToken(token_t token, FILE *out) {
    char buffer[100];
    fprintf(out, "%s", tokenToString(token, buffer));
}

void printNode(node_t *node, FILE *out) { // NOLINT(misc-no-recursion)
    switch (node->type) {
        case NT_ADD: {
            fprintf(out, "+ ");
            printNode(node->children[0], out);
            printNode(node->children[1], out);
            break;
        }
        case NT_MULTIPLY: {
            fprintf(out, "* ");
            printNode(node->children[0], out);
            printNode(node->children[1], out);
            break;
        }
        case NT_TOKEN: {
            printToken(node->token, out);
            fprintf(out, " ");
            break;
        }
    }
}

