#include <assert.h>
#include <stdlib.h>
#include "calculate.h"


int calculate(node_t *node) { // NOLINT(misc-no-recursion)
    assert(node != NULL);
    switch (node->name) {
        case NN_EXPRESSION: return calculateExpression(node);
        case NN_TERM: return calculateTerm(node);
        case NN_FACTOR: return calculateFactor(node);
        case NN_PRIMARY: return calculatePrimary(node);
        default: {
            assert(!"Calculation for the node not implemented");
            abort();
        }
    }
}

int calculateExpression(node_t *node) { // NOLINT(misc-no-recursion)
    int acc = calculate(node->children->items[0]);

    for (int i = 1; i < node->children->size; i += 2) {
        enum token_type operator = node->children->items[i]->token.type;
        int number = calculate(node->children->items[i + 1]);

        switch (operator) {
            case BIN_ADD: {
                acc += number;
                break;
            }
            case BIN_SUB: {
                acc -= number;
                break;
            }
            default: assert(!"Unexpected token type");
        }
    }

    return acc;
}

int calculateTerm(node_t *node) { // NOLINT(misc-no-recursion)
    int acc = calculate(node->children->items[0]);

    for (int i = 1; i < node->children->size; i += 2) {
        enum token_type operator = node->children->items[i]->token.type;
        int number = calculate(node->children->items[i + 1]);

        switch (operator) {
            case BIN_MULTIPLY: {
                acc *= number;
                break;
            }
            case BIN_DIVIDE: {
                acc /= number;
                break;
            }
            case BIN_MODE: {
                acc %= number;
                break;
            }
            default: assert(!"Unexpected token type");
        }
    }

    return acc;
}

int calculateFactor(node_t *node) { // NOLINT(misc-no-recursion)
    return calculate(node->children->items[0]);
}

int calculatePrimary(node_t *node) { // NOLINT(misc-no-recursion)
    if (node->children->size == 1) {
        return node->children->items[0]->token.number;
    }

    return calculate(node->children->items[1]);
}
