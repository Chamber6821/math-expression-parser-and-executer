#include <stddef.h>
#include "build_result.h"

build_result_t success(int read, node_t *node) {
    return (build_result_t) {.read = read, .node = node};
}

build_result_t failure() {
    return (build_result_t) {.read = 0, .node = NULL};
}

bool isSuccess(build_result_t result) {
    return result.node != NULL;
}

void revert(stack_t *source, build_result_t what) {
    rollBack(source, what.read);
}

void set(build_result_t *acc, build_result_t from) {
    acc->read += from.read;
    acc->node = from.node;
}

void add(build_result_t *acc, build_result_t what) {
    acc->read += what.read;
}
