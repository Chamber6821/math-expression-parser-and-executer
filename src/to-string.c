#include <stdio.h>
#include "to-string.h"

const char *binOperatorToString(enum bin_operator operator) {
    switch (operator) {
        case ADD: return "+";
        case MULTIPLY: return "*";
        default: return "NOT-DEFINED-BIN";
    }
}

const char *tokenToString(token_t token, char *buffer) {
    switch (token.type) {
        case NUMBER: {
            sprintf(buffer, "%i", token.number);
            return buffer;
        }
        case BIN_OPERATOR: return binOperatorToString(token.binOperator);
        case OPEN_PAR: return "(";
        case CLOSE_PAR: return ")";
        case END: return "<-";
        default: "NOT-DEFINED-TOKEN-TYPE";
    }
}
