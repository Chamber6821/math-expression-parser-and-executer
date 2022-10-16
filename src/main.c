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


build_result_t tryBuildExpression(stack_t *source);
build_result_t tryBuildTerm(stack_t *source);
build_result_t tryBuildFactor(stack_t *source);
build_result_t tryBuildPrimary(stack_t *source);

build_result_t tryBuildNumber(stack_t *source);
build_result_t tryBuildByTokenType(stack_t *source, enum token_type type);

build_result_t tryBuildExpression(stack_t *source) { // NOLINT(misc-no-recursion)
    build_result_t result = failure(), new;

    new = tryBuildTerm(source);
    if (!isSuccess(new)) {
        revert(source, result);
        return failure();
    }
    set(&result, new);

    while (source->size != 0) {
        new = tryBuildByTokenType(source, BIN_ADD);
        if (!isSuccess(new)) {
            break;
        }
        add(&result, new);

        enum token_type type = new.node->token.type;

        new = tryBuildTerm(source);
        if (!isSuccess(new)) {
            //FIXME: Не нужно откатывать все изменения
            revert(source, result);
            break;
        }

        result.node = makeOperator(type, result.node, new.node);
        result.read += new.read;
    }

    return result;
}

build_result_t tryBuildTerm(stack_t *source) { // NOLINT(misc-no-recursion)
    build_result_t result = failure(), new;

    new = tryBuildFactor(source);
    if (!isSuccess(new)) {
        revert(source, result);
        return failure();
    }
    set(&result, new);

    while (source->size != 0) {
        new = tryBuildByTokenType(source, BIN_MULTIPLY);
        if (!isSuccess(new)) {
            break;
        }
        add(&result, new);

        enum token_type type = new.node->token.type;

        new = tryBuildTerm(source);
        if (!isSuccess(new)) {
            //FIXME: Не нужно откатывать все изменения
            revert(source, result);
            break;
        }

        result.node = makeOperator(type, result.node, new.node);
        result.read += new.read;
    }

    return result;
}

build_result_t tryBuildFactor(stack_t *source) { // NOLINT(misc-no-recursion)
    return tryBuildPrimary(source);
}

build_result_t tryBuildPrimary(stack_t *source) { // NOLINT(misc-no-recursion)
    build_result_t result = failure(), new;

    new = tryBuildNumber(source);
    if (isSuccess(new)) return new;

    new = tryBuildByTokenType(source, OPEN_PAR);
    if (!isSuccess(new)) {
        revert(source, result);
        return failure();
    }
    add(&result, new);

    new = tryBuildExpression(source);
    if (!isSuccess(new)) {
        revert(source, result);
        return failure();
    }
    set(&result, new);

    new = tryBuildByTokenType(source, CLOSE_PAR);
    if (!isSuccess(new)) {
        revert(source, result);
        return failure();
    }
    add(&result, new);

    return result;
}

build_result_t tryBuildNumber(stack_t *source) {
    build_result_t result = failure(), new;

    new = tryBuildByTokenType(source, NUMBER);
    if (!isSuccess(new)) {
        revert(source, result);
        return failure();
    }
    set(&result, new);

    return result;
}

build_result_t tryBuildByTokenType(stack_t *source, enum token_type type) {
    if (source->size == 0) return failure();

    token_t token = *front(source);
    if (token.type != type) return failure();

    pop(source);
    return success(1, nodeFromToken(token));
}

void test(const char *input, const char *expected);

int main() {
    test("1 + 2      ", "+ 1 2");
    test("1 + 2 * 3  ", "+ 1 * 2 3");
    test("1 + (2 * 3)", "+ 1 * 2 3");
    test("(1 + 2) * 3", "* + 1 2 3");
    test("1 + 2 + 3  ", "+ + 1 2 3");
    test("1 + (2 + 3)", "+ 1 + 2 3");
    test("1 + 2 + 3  ", "+ + 1 2 3");
    test("1 + 2 + 3) ", "error");
    test("1 + (2 + 3 ", "error");
    test("1 + 2 3 ", "error");
    test("1 * 2 3 ", "error");
    test("1 + 2 * 3 ", "+ 1 * 2 3");
}

static int testNumber = 0;

char *rebuildToPolish(const char *input, char *buffer, int bufferSize) {
    source_t *source = construct_source(malloc(sizeof(stack_t)), input);

    stack_t *stack = construct_stack(malloc(sizeof(stack_t)));
    do {
        push(stack, parseToken(source));
        //printToken(*front(stack));
    } while (front(stack)->type != END);
    reverse_stack(stack);

    build_result_t result = tryBuildExpression(stack);

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
    free(source);

    return buffer;
}

char *lightTrimRight(char *str) {
    for (int i = (int) strlen(str) - 1; i >= 0; i--) {
        if (!isspace(str[i])) break;
        str[i] = 0;
    }
    return str;
}

void test(const char *input, const char *expected) {
    char out[100];
    printf("Test %2d: ", testNumber);
    rebuildToPolish(input, out, _countof(out));
    lightTrimRight(out);

    if (strcmp(out, expected) == 0) {
        printf("PASS\n");
    } else {
        printf("%s -> %s expected: %s FAILURE\n", input, out, expected);
    }

    testNumber++;
}
