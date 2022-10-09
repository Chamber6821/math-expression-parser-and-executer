#include <stdio.h>
#include "token.h"
#include "to-string.h"
#include "source.h"
#include "parsing.h"

void printToken(token_t token) {
    char buffer[100];
    printf("%s", tokenToString(token, buffer));
}

int main() {
    char buffer[100];
    gets(buffer);

    source_t source_buffer;
    source_t *source = construct_source(&source_buffer, buffer);

    while (!isEnd(source)) {
        printToken(parseToken(source));
        printf(" ");
    }
}
