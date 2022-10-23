#include <malloc.h>
#include "vector.h"

vector_t *new_vector() {
    vector_t *new = malloc(sizeof(vector_t));
    new->size = 0;
    new->capacity = 0;
    new->items = NULL;
    return new;
}

void free_vector(vector_t *self) {
    free(self->items);
    free(self);
}

void vector_reserve(vector_t *self, int newCapacity) {
    if (self->capacity >= newCapacity) return;
    self->capacity = newCapacity;
    self->items = realloc(self->items, newCapacity * sizeof(vector_element_t));
}

void push_back(vector_t *self, vector_element_t element) {
    int newSize = self->size + 1;

    int newCapacity = 1;
    while (newCapacity < newSize) newCapacity *= 2;

    vector_reserve(self, newCapacity);
    self->items[self->size++] = element;
}
