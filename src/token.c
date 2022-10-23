#include "token.h"

token_t onlyType(enum token_type type) {
    return (token_t) {.type = type};
}

token_t number(token_number_t number) {
    return (token_t) {.type = NUMBER, .number = number};
}
