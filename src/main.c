#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "to-string.h"
#include "source.h"
#include "parsing.h"
#include "stack.h"
#include "build_result.h"
#include "node.h"
#include "print.h"
#include "builder.h"
#include "custom_builder.h"


void test(const char *input, const char *expected, builder_t **rules);

stack_t *parseAllTokens(const char *input);

builder_t **createRules(builder_t **rules) {
    rules[NN_EXPRESSION] = seq(2,
                               node(NN_TERM, rules),
                               zero_or_more(seq(2,
                                                token(BIN_ADD),
                                                node(NN_TERM, rules))));

    rules[NN_TERM] = seq(2,
                         node(NN_FACTOR, rules),
                         zero_or_more(seq(2,
                                          token(BIN_MULTIPLY),
                                          node(NN_FACTOR, rules))));

    rules[NN_FACTOR] = node(NN_PRIMARY, rules);

    rules[NN_PRIMARY] = or(2,
                           token(NUMBER),
                           seq(3,
                               token(OPEN_PAR),
                               node(NN_EXPRESSION, rules),
                               token(CLOSE_PAR)));

    return rules;
}

build_result_t buildByRules(builder_t **rules, stack_t *source) {
    builder_t *builder = node(NN_EXPRESSION, rules);
    return builder->build(builder, source);
}

int main() {
    builder_t *rules[NN_COUNT_OF_NAMES] = {};
    createRules(rules);

    test("1 + 2      ", "[[[[1]]] + [[[2]]]]", rules);
    test("1 + 2 * 3  ", "[[[[1]]] + [[[2]] * [[3]]]]", rules);
    test("1 + (2 * 3)", "[[[[1]]] + [[[( [[[[2]] * [[3]]]] )]]]]", rules);
    test("(1 + 2) * 3", "[[[[( [[[[1]]] + [[[2]]]] )]] * [[3]]]]", rules);
    test("1 + 2 + 3  ", "[[[[1]]] + [[[2]]] + [[[3]]]]", rules);
    test("1 + (2 + 3)", "[[[[1]]] + [[[( [[[[2]]] + [[[3]]]] )]]]]", rules);
    test("1 + 2 + 3) ", "error", rules);
    test("1 + (2 + 3 ", "error", rules);
    test("1 + 2 3    ", "error", rules);
    test("1 * 2 3    ", "error", rules);
}

static int testNumber = 0;

char *rebuildWithAst(const char *input, char *buffer, int bufferSize, builder_t **rules) {
    stack_t *stack = parseAllTokens(input);

    build_result_t result = buildByRules(rules, stack);

    if (stack->size == 1) {
        FILE *out = tmpfile();
        printNode(result.node, out);
        rewind(out);
        fgets(buffer, bufferSize, out);
        fclose(out);
        free_node(result.node);
    } else {
        strcpy(buffer, "error");
    }

    free_stack(stack);

    return buffer;
}

char *lightTrimRight(char *str) {
    for (int i = (int) strlen(str) - 1; i >= 0; i--) {
        if (!isspace(str[i])) break;
        str[i] = 0;
    }
    return str;
}

void test(const char *input, const char *expected, builder_t **rules) {
    char out[100];
    printf("Test %2d: ", testNumber);
    rebuildWithAst(input, out, _countof(out), rules);
    lightTrimRight(out);

    if (strcmp(out, expected) == 0) {
        printf("PASS\n");
    } else {
        printf("%s -> %s expected: %s FAILURE\n", input, out, expected);
    }

    testNumber++;
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
