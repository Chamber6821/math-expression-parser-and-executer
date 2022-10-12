#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "to-string.h"
#include "source.h"
#include "parsing.h"
#include "stack.h"

enum node_type {
    NT_NUMBER, NT_ADD, NT_MULTIPLY,
};

typedef struct expression_node expression_node_t;
struct expression_node {
    enum node_type type;
    union {
        int number;
        expression_node_t *children[2];
    };
};

void free_node(expression_node_t *node) {
    switch (node->type) {
        case NT_NUMBER: {
            free(node);
            break;
        }
        case NT_ADD:
        case NT_MULTIPLY: {
            free_node(node->children[0]);
            free_node(node->children[1]);
            free(node);
            break;
        }
    }
}

enum node_type operatorToNodeType(enum token_type operator) {
    switch (operator) {
        case BIN_ADD: return NT_ADD;
        case BIN_MULTIPLY: return NT_MULTIPLY;
        default: {
            assert(!"NOT DEFINED");
            abort();
        }
    }
}

typedef struct build_result build_result_t;
struct build_result {
    int read;
    bool success;
    expression_node_t *node;
};

build_result_t success(int read, expression_node_t *node) {
    return (build_result_t) {
            .read = read,
            .success = true,
            .node = node,
    };
}

build_result_t failure() {
    return (build_result_t) {
            .read = 0,
            .success = false,
            .node = NULL
    };
}

bool isSuccess(build_result_t result) {
    return result.success;
}

build_result_t tryBuildExpression(stack_t *source);
build_result_t tryBuildTerm(stack_t *source);
build_result_t tryBuildFactor(stack_t *source);
build_result_t tryBuildPrimary(stack_t *source);

build_result_t tryBuildNumber(stack_t *source);
bool tryMatchByType(stack_t *source, enum token_type type, token_t *out);

build_result_t tryBuildExpression(stack_t *source) {
    build_result_t result;

    result = tryBuildTerm(source);
    if (!isSuccess(result)) return failure();

    while (source->size != 0) {
        int read = 0;

        token_t token;
        if (!tryMatchByType(source, BIN_ADD, &token)) break;
        read += 1;

        build_result_t right = tryBuildTerm(source);
        read += right.read;
        if (!isSuccess(right)) {
            rollBack(source, read);
            break;
        }

        expression_node_t *newNode = malloc(sizeof(expression_node_t));
        newNode->type = operatorToNodeType(token.type);

        newNode->children[0] = result.node;
        newNode->children[1] = right.node;
        result.node = newNode;
        result.read += right.read + 1;
    }

    return result;
}

build_result_t tryBuildTerm(stack_t *source) {
    build_result_t result;

    result = tryBuildFactor(source);
    if (!isSuccess(result)) return failure();

    while (source->size != 0) {
        token_t token;
        if (!tryMatchByType(source, BIN_MULTIPLY, &token)) break;

        build_result_t right = tryBuildFactor(source);
        if (!isSuccess(right)) {
            rollBack(source, 1);
            break;
        }

        expression_node_t *newNode = malloc(sizeof(expression_node_t));
        newNode->type = operatorToNodeType(token.type);

        newNode->children[0] = result.node;
        newNode->children[1] = right.node;
        result.node = newNode;
        result.read += right.read + 1;
    }

    return result;
}

build_result_t tryBuildFactor(stack_t *source) {
    return tryBuildPrimary(source);
}

build_result_t tryBuildPrimary(stack_t *source) {
    build_result_t result;

    result = tryBuildNumber(source);
    if (isSuccess(result)) return result;

    if (!tryMatchByType(source, OPEN_PAR, NULL)) return failure();

    result = tryBuildExpression(source);
    if (!isSuccess(result)) {
        rollBack(source, 1);
        return failure();
    }

    if (!tryMatchByType(source, CLOSE_PAR, NULL)) {
        rollBack(source, result.read + 1);
        return failure();
    }

    return success(result.read + 2, result.node);
}

build_result_t tryBuildNumber(stack_t *source) {
    token_t token;
    if (!tryMatchByType(source, NUMBER, &token)) return failure();

    expression_node_t *node = malloc(sizeof(expression_node_t));
    node->type = NT_NUMBER;
    node->number = token.number;
    return success(1, node);
}

bool tryMatchByType(stack_t *source, enum token_type type, token_t *out) {
    if (source->size == 0 || front(source)->type != type) return false;

    if (out != NULL) *out = *front(source);
    pop(source);
    return true;
}

void printToken(token_t token) {
    char buffer[100];
    printf("%s", tokenToString(token, buffer));
}

void printNode(expression_node_t *node, FILE *out) {
    switch (node->type) {
        case NT_NUMBER: {
            fprintf(out, "%d ", node->number);
            break;
        }
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
    }
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
    } else {
        strcpy(buffer, "error");
    }

    free_node(result.node);
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
    printf("Test %d: ", testNumber);
    rebuildToPolish(input, out, _countof(out));
    lightTrimRight(out);


    if (strcmp(out, expected) == 0) {
        printf("PASS\n");
    } else {
        printf("%s -> %s expected: %s FAILURE\n", input, out, expected);
    }

    testNumber++;
}


#pragma clang diagnostic pop