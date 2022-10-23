#include <malloc.h>
#include "builder.h"
#include "custom_builder.h"

static void simplifyChildren(node_t *node);

builder_t *
new_custom_builder(build_method_f buildMethod, int count, va_list builders) { // NOLINT(readability-non-const-parameter)
    custom_builder_t *new = malloc(sizeof(custom_builder_t));
    new->parent.build = (real_build_method_f) buildMethod;
    new->count = count;
    new->elements = calloc(count, sizeof(builder_t *));

    builder_t **elements = new->elements;
    for (int i = 0; i < count; i++) {
        elements[i] = va_arg(builders, builder_t*);
    }

    return &new->parent;
}

static build_result_t impl_or_builder_build(custom_builder_t *self, stack_t *source) {
    for (int i = 0; i < self->count; i++) {
        builder_t *builder = self->elements[i];
        build_result_t new = builder->build(builder, source);
        if (isSuccess(new)) return new;
    }
    return failure();
}

static build_result_t impl_seq_builder_build(custom_builder_t *self, stack_t *source) {
    build_result_t result = failure();

    node_t *node = nodeWithChildren(NN_COMPOSITE);
    for (int i = 0; i < self->count; i++) {
        builder_t *builder = self->elements[i];
        build_result_t new = builder->build(builder, source);

        if (!isSuccess(new)) {
            revert(source, result);
            free_node(node);
            return failure();
        }

        add(&result, new);
        push_back(node->children, new.node);
    }

    result.node = node;
    return result;
}

builder_t *or(int count, ...) {
    va_list builders;
    va_start(builders, count);
    builder_t *new = new_custom_builder(impl_or_builder_build, count, builders);
    va_end(builders);
    return new;
}

builder_t *seq(int count, ...) {
    va_list builders;
    va_start(builders, count);
    builder_t *new = new_custom_builder(impl_seq_builder_build, count, builders);
    va_end(builders);
    return new;
}

static build_result_t impl_zero_or_more(builder_t *self, stack_t *source) {
    builder_t *builder = ((zero_or_more_builder_t *) self)->builder;

    build_result_t result = failure();
    result.node = nodeWithChildren(NN_COMPOSITE);

    while (source->size > 0) {
        build_result_t new = builder->build(builder, source);
        if (!isSuccess(new)) break;
        push_back(result.node->children, new.node);
        add(&result, new);
    }

    return result;
}

builder_t *zero_or_more(builder_t *builder) {
    zero_or_more_builder_t *new = malloc(sizeof(zero_or_more_builder_t));
    new->parent.build = impl_zero_or_more;
    new->builder = builder;
    return &new->parent;
}

static build_result_t impl_token_builder_build(builder_t *self, stack_t *source) {
    if (source->size == 0) return failure();

    token_t token = *front(source);
    if (token.type != ((token_builder_t *) self)->type) return failure();

    pop(source);
    return success(1, nodeFromToken(NN_TOKEN, token));
}

builder_t *token(enum token_type type) {
    token_builder_t *new = malloc(sizeof(token_builder_t));
    new->parent.build = impl_token_builder_build;
    new->type = type;
    return &new->parent;
}

static build_result_t impl_node_builder_build(builder_t *self, stack_t *source) {
    node_builder_t *this = (node_builder_t *) self;
    builder_t *builder = this->catalog[this->name];

    build_result_t result = builder->build(builder, source);
    if (!isSuccess(result)) return failure();

    node_t *root = nodeWithChildren(this->name);
    push_back(root->children, result.node);
    result.node = root;

    if (!hasToken(result.node)) simplifyChildren(result.node);

    return result;
}

builder_t *node(enum node_name name, builder_t **catalog) {
    node_builder_t *new = malloc(sizeof(node_builder_t));
    new->parent.build = impl_node_builder_build;
    new->name = name;
    new->catalog = catalog;
    return &new->parent;
}

static void simplifyChildren(node_t *node) { // NOLINT(misc-no-recursion)
    if (hasToken(node)) return;

    vector_t *oldChildren = node->children;
    node->children = new_vector();

    for (int i = 0; i < oldChildren->size; i++) {
        node_t *child = oldChildren->items[i];
        if (!child) continue;

        simplifyChildren(child);

        if (child->name != NN_COMPOSITE) {
            push_back(node->children, child);
            continue;
        }

        for (int j = 0; j < child->children->size; j++) {
            push_back(node->children, child->children->items[j]);
        }
        free_nodeWithOutChildren(child);
    }

    free_vector(oldChildren);
}
