#include "rules.h"
#include "custom_builder.h"

builder_t **createRules(builder_t **rules) {
    rules[NN_EXPRESSION] = seq(2,
                               node(NN_TERM, rules),
                               zero_or_more(seq(2,
                                                or(2,
                                                   token(BIN_ADD),
                                                   token(BIN_SUB)),
                                                node(NN_TERM, rules))));

    rules[NN_TERM] = seq(2,
                         node(NN_FACTOR, rules),
                         zero_or_more(seq(2,
                                          or(2,
                                             token(BIN_MULTIPLY),
                                             token(BIN_DIVIDE)),
                                          node(NN_FACTOR, rules))));

    rules[NN_FACTOR] = node(NN_PRIMARY, rules);

    rules[NN_PRIMARY] = or(3,
                           token(NUMBER),
                           node(NN_FUNCTION, rules),
                           seq(3,
                               token(OPEN_PAR),
                               node(NN_EXPRESSION, rules),
                               token(CLOSE_PAR)));

    rules[NN_FUNCTION] = seq(4,
                             token(FUNCTION_NAME),
                             token(OPEN_PAR),
                             node(NN_EXPRESSION, rules),
                             token(CLOSE_PAR));

    return rules;
}

build_result_t buildByRules(builder_t **rules, stack_t *source) {
    builder_t *builder = node(NN_EXPRESSION, rules);
    return builder->build(builder, source);
}
