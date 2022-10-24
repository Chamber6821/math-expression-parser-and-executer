#include <stdio.h>
#include <string.h>
#include "to-string.h"

const char *tokenToString(token_t token, char *buffer) {
    switch (token.type) {
        case NUMBER: {
            sprintf(buffer, "%.6lg", token.number);
            return buffer;
        }
        case OPEN_PAR: return "(";
        case CLOSE_PAR: return ")";
        case END: return "<-";
        case BIN_ADD: return "+";
        case BIN_MULTIPLY: return "*";
        case FUNCTION_NAME: return strcpy(buffer, token.function);
        default: return "NOT-DEFINED-TOKEN-TYPE";
    }
}

