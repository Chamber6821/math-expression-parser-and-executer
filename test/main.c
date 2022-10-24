#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "builder.h"
#include "rules.h"
#include "parsing.h"
#include "print.h"
#include "calculate.h"

char *rebuildWithAst(const char *input, char *buffer, int bufferSize, builder_t **rules);
char *lightTrimRight(char *str);
void test(const char *input, const char *expected, builder_t **rules);
void testCalc(const char *input, token_number_t expected, builder_t **rules);

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
    test("sin(0)", "[[[[[sin ( [[[[0]]]] )]]]]]", rules);
    test("cos(1 * 8)", "[[[[[cos ( [[[[1]] * [[8]]]] )]]]]]", rules);

    testCalc("2+3", 5, rules);
    testCalc("7*8 - 120/6 + 4", 40, rules);
    testCalc("10/(6-2)+8", 10.5, rules);
    testCalc("sin(3.14/2)+cos(2-6/3)", 2, rules);
    testCalc("three plus five", NAN, rules);
    testCalc("6.12e3+2e-2", 6120.02, rules);
    testCalc("-1", NAN, rules);
    testCalc("12. + Sin(7)", 12.656987, rules);
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
    printf("Test %2d: ", ++testNumber);
    rebuildWithAst(input, out, _countof(out), rules);
    lightTrimRight(out);

    if (strcmp(out, expected) == 0) {
        printf("PASS\n");
    } else {
        printf("%s -> %s expected: %s FAILURE\n", input, out, expected);
    }
}

void testCalc(const char *input, token_number_t expected, builder_t **rules) {
    printf("Test %2d: ", ++testNumber);

    stack_t *tokenStack = parseAllTokens(input);
    node_t *expr = buildByRules(rules, tokenStack).node;

    if (isnan(expected) && (expr == NULL || tokenStack->size != 1)) {
        // ожидалась ошибка
        printf("PASS\n");
        return;
    }

    if (expr == NULL) {
        printf("NULL FAILURE\n");
        return;
    }

    if (tokenStack->size != 1) {
        printf("stack not empty FAILURE\n");
        return;
    }

    token_number_t answer = calculate(expr);
    if (fabs(answer - expected) > 0.001) {
        printf("%s -> %lf expected %lf\n", input, answer, expected);
        return;
    }

    printf("PASS\n");
}
