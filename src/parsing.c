#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "parsing.h"

source_t *skipSpaces(source_t *source) {
    while (!isEnd(source) && isspace(*source->marker))
        source->marker++;
    return source;
}

token_t parseToken(source_t *source) {
    skipSpaces(source);

    if (isEnd(source)) return onlyType(END);

    char ch = get(source);

    source->marker++;
    switch (ch) {
        case '(': return onlyType(OPEN_PAR);
        case ')': return onlyType(CLOSE_PAR);
        case '+': return onlyType(BIN_ADD);
        case '-': return onlyType(BIN_SUB);
        case '*': return onlyType(BIN_MULTIPLY);
        case '/': return onlyType(BIN_DIVIDE);
        case '%': return onlyType(BIN_MODE);
        default: {
            source->marker--;
            break;
        }
    }

    int value, read = 0;
    int scanned = sscanf_s(source->marker, "%i%n", &value, &read);
    if (scanned >= 1) {
        source->marker += read;
        return number(value);
    }

    return onlyType(END);
}

stack_t *parseAllTokens(const char *input) {
    source_t *source = construct_source(malloc(sizeof(stack_t)), input);

    stack_t *stack = construct_stack(malloc(sizeof(stack_t)));
    do {
        push(stack, parseToken(source));
    } while (front(stack)->type != END);
    reverse_stack(stack);

    free(source);

    return stack;
}
