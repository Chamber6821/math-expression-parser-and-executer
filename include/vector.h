#pragma once

#include "types.h"

typedef struct vector vector_t;
struct vector {
    int size;
    int capacity;
    vector_element_t *items;
};

vector_t *new_vector();
void free_vector(vector_t *self);

void vector_reserve(vector_t *self, int newCapacity);
void push_back(vector_t *self, vector_element_t element);
