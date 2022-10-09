#include "token.h"

token_t onlyType(enum token_type type) {
    return (token_t) {.type = type};
}

token_t number(int number) {
    return (token_t) {.type = NUMBER, .number = number};
}

token_t binOperator(enum bin_operator operator) {
    return (token_t) {
            .type = BIN_OPERATOR,
            .binOperator = operator
    };
}
