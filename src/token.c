#include "token.h"

token_t onlyType(enum token_type type) {
    return (token_t) {.type = type};
}

token_t number(int number) {
    return (token_t) {.type = NUMBER, .number = number};
}
