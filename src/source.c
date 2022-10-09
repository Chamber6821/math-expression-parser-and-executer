#include <assert.h>
#include <string.h>
#include "source.h"

source_t *construct_source(source_t *buffer, const char *string) {
    buffer->marker = string;
    buffer->end = string + strlen(string);
    return buffer;
}

bool isEnd(source_t *source) {
    return source->marker >= source->end;
}

char get(source_t *source) {
    assert(isEnd(source));
    return *source->marker;
}
