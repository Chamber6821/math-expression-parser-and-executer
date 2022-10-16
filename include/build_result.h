#pragma once

#include <stdbool.h>
#include "stack.h"
#include "node.h"

typedef struct build_result build_result_t;
struct build_result {
    int read;
    node_t *node;
};


build_result_t success(int read, node_t *node);
build_result_t failure();

bool isSuccess(build_result_t result);
void revert(stack_t *source, build_result_t what);

void set(build_result_t *acc, build_result_t from);
void add(build_result_t *acc, build_result_t what);
