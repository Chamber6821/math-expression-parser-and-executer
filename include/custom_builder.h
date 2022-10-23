#pragma once

#include <stdarg.h>
#include "builder.h"
#include "build_result.h"

/*
 * То к чему стремимся
 *
 * rule_catalog_t *catalog = ...;
 *
 * addRule(catalog, PRIMARY,
 *          or(2
 *              token(NUMBER),
 *              seq(3
 *                  token(OPEN_PAR),
 *                  node(EXPRESSION, catalog),
 *                  token(CLOSE_PAR),
 *              )
 *          ));
 *
 *
 * catalog[NN_EXPRESSION] = seq(2,
 *                              node(NN_TERM, catalog),
 *                              zero_or_more(seq(2,
 *                                  or(2, token(BIN_ADD), token(BIN_SUB)),
 *                                  node(NN_TERM, catalog))));
 */

typedef struct custom_builder custom_builder_t;
struct custom_builder {
    builder_t parent;
    int count;
    builder_t **elements;
};

typedef struct token_builder token_builder_t;
struct token_builder {
    builder_t parent;
    enum token_type type;
};

typedef struct node_builder node_builder_t;
struct node_builder {
    builder_t parent;
    enum node_name name;
    builder_t **catalog;
};

typedef struct zero_or_more_builder zero_or_more_builder_t;
struct zero_or_more_builder {
    builder_t parent;
    builder_t *builder;
};

typedef build_result_t (*build_method_f)(custom_builder_t *self, stack_t *source);
typedef build_result_t (*real_build_method_f)(builder_t *self, stack_t *source);

builder_t *new_custom_builder(build_method_f buildMethod, int count, va_list builders);

builder_t *or(int count, ...);
builder_t *seq(int count, ...);
builder_t *zero_or_more(builder_t *builder);

builder_t *token(enum token_type type);
builder_t *node(enum node_name name, builder_t **catalog);
