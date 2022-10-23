#include <stdio.h>
#include <ctype.h>
#include "parsing.h"

source_t *skipSpaces(source_t *source) {
    while (!isEnd(source) && isspace(*source->marker))
        source->marker++;
    return source;
}

token_t parseToken(source_t *source) {
    skipSpaces(source);

    if (isEnd(source)) return onlyType(END);

    char ch = get(source);

    source->marker++;
    switch (ch) {
        case '(': return onlyType(OPEN_PAR);
        case ')': return onlyType(CLOSE_PAR);
        case '+': return onlyType(BIN_ADD);
        case '*': return onlyType(BIN_MULTIPLY);
        default: {
            source->marker--;
            break;
        }
    }

    int value, read = 0;
    int scanned = sscanf_s(source->marker, "%i%n", &value, &read);
    if (scanned >= 1) {
        source->marker += read;
        return number(value);
    }

    return onlyType(END);
}
