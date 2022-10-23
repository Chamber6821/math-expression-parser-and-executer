#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "builder.h"
#include "rules.h"
#include "parsing.h"
#include "print.h"

char *rebuildWithAst(const char *input, char *buffer, int bufferSize, builder_t **rules);
char *lightTrimRight(char *str);
void test(const char *input, const char *expected, builder_t **rules);

static int testNumber = 0;

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
