#pragma once

#include "stack.h"
#include "build_result.h"

typedef struct builder builder_t;
struct builder {
    build_result_t (*build)(builder_t *self, stack_t *source);
};
