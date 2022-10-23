#include "rules.h"
#include "custom_builder.h"

builder_t **createRules(builder_t **rules) {
    rules[NN_EXPRESSION] = seq(2,
                               node(NN_TERM, rules),
                               zero_or_more(seq(2,
                                                token(BIN_ADD),
                                                node(NN_TERM, rules))));

    rules[NN_TERM] = seq(2,
                         node(NN_FACTOR, rules),
                         zero_or_more(seq(2,
                                          token(BIN_MULTIPLY),
                                          node(NN_FACTOR, rules))));

    rules[NN_FACTOR] = node(NN_PRIMARY, rules);

    rules[NN_PRIMARY] = or(2,
                           token(NUMBER),
                           seq(3,
                               token(OPEN_PAR),
                               node(NN_EXPRESSION, rules),
                               token(CLOSE_PAR)));

    return rules;
}

build_result_t buildByRules(builder_t **rules, stack_t *source) {
    builder_t *builder = node(NN_EXPRESSION, rules);
    return builder->build(builder, source);
}
