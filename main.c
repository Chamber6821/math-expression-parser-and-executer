#include "parsing.h"
#include "stack.h"
#include "node.h"
#include "print.h"
#include "builder.h"
#include "calculate.h"
#include "rules.h"


int main() {
    builder_t *rules[NN_COUNT_OF_NAMES] = {};
    createRules(rules);

    char input[100];
    gets(input);

    stack_t *tokenStack = parseAllTokens(input);
    node_t *expr = buildByRules(rules, tokenStack).node;
    token_number_t result = calculate(expr);

    printf("%s = %lf\n", input, result);
}
