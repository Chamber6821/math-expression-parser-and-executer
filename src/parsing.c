#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "parsing.h"

source_t *skipSpaces(source_t *source) {
    while (!isEnd(source) && isspace(*source->marker))
        source->marker++;
    return source;
}

static token_t tryParseNumber(source_t *source) {
    token_number_t value;
    int read = 0;
    if (1 == sscanf_s(source->marker, "%lf%n", &value, &read)) {
        source->marker += read;
        return number(value);
    }

    return onlyType(END);
}

token_t tryParseFunction(source_t *source) {
    token_t result = onlyType(FUNCTION_NAME);

    int read = 0;
    while (isalpha(source->marker[read]) && read < _countof(result.function) - 1) {
        result.function[read] = source->marker[read];
        read++;
    }
    result.function[read] = 0;

    if (read == 0) return onlyType(END);

    source->marker += read;
    return result;
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
        default: {
            source->marker--;
            break;
        }
    }

    token_t token;

    token = tryParseNumber(source);
    if (token.type != END) return token;

    token = tryParseFunction(source);
    if (token.type != END) return token;

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
