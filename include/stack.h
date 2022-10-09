#pragma once

#include <stdbool.h>
#include "types.h"


typedef struct stack stack_t;

struct stack {
    int size;
    int capacity;
    stack_element_t *_items;
};

stack_t *construct_stack(stack_t *self);
void free_stack(stack_t *self);

bool push(stack_t *self, stack_element_t element);
stack_element_t pop(stack_t *self);

stack_element_t *front(stack_t *self);
bool reserve(stack_t *self, int capacity);
