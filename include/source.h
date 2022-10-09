#pragma once

#include <stdbool.h>

typedef struct source source_t;
struct source {
    const char *marker;
    const char *end;
};

source_t *construct_source(source_t *buffer, const char *string);

bool isEnd(source_t *source);
char get(source_t *source);
