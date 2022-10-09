#include <memory.h>
#include <stdlib.h>
#include "stack.h"
#include "utils.h"

const int START_CAPACITY = 1;
const int CAPACITY_FACTOR = 2;

stack_t *construct_stack(stack_t *self) {
    memset(self, 0, sizeof(stack_t));
    return self;
}

void free_stack(stack_t *self) {
    free(self->_items);
    free(self);
}

bool push(stack_t *self, stack_element_t element) {
    if (!reserve(self, self->size + 1)) return false;

    self->_items[self->size] = element;
    self->size++;
    return true;
}

stack_element_t pop(stack_t *self) {
    if (self->size == 0)
        exit(1);

    stack_element_t copy = *front(self);
    self->size--;
    return copy;
}

stack_element_t *front(stack_t *self) {
    if (self->size == 0) return NULL;
    return &self->_items[self->size - 1];
}

bool reserve(stack_t *self, int capacity) {
    if (self->capacity >= capacity) return true;

    int newCapacity = max(self->capacity, START_CAPACITY);
    while (newCapacity < capacity)
        newCapacity *= CAPACITY_FACTOR;

    size_t memorySize = newCapacity * sizeof(stack_element_t);
    stack_element_t *newBuffer = realloc(self->_items,
                                         memorySize);

    if (newBuffer == NULL) return false;

    self->_items = newBuffer;
    self->capacity = newCapacity;
    return true;
}
