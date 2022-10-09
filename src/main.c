#include <malloc.h>
#include <stdio.h>
#include <ctype.h>
#include "stack.h"

const char *
binary_operator_to_string(enum binary_operator operator) {
    switch (operator) {
        case SUM: return "+";
        case MULTIPLY: return "*";
        default: return "NOT-DEFINED";
    }
}

void print_stack_element(const stack_element_t *element) {
    switch (element->type) {
        case NUMBER: {
            printf("%d", element->number);
            break;
        }
        case BINARY_OPERATOR: {
            printf("%s", binary_operator_to_string(
                    element->binaryOperator));
            break;
        }
    }
}

void print_stack(stack_t *self) {
    printf("Stack:");
    for (int i = 0; i < self->size; i++) {
        printf(" ");
        print_stack_element(&self->_items[i]);
    }
    printf("\n");
}

stack_t *to_stack(stack_t *buffer, stack_element_t *elements, int size) {
    for (int i = 0; i < size; i++) {
        push(buffer, elements[i]);
    }
    return buffer;
}

lex_t parse_lex(const char *source) {
    while (source[0] != 0 && isspace(source[0])) source++;
    if ()
}

int main() {
    stack_t *stack = construct_stack(malloc(sizeof(stack_t)));

    print_stack(stack);
    for (int i = 0; i < 40; i++) {
        lex_t lex;
        if (i % 2) {
            lex.type = NUMBER;
            lex.number = 5;
        } else {
            lex.type = BINARY_OPERATOR;
            lex.binaryOperator = SUM;
        }

        push(stack, lex);
        print_stack(stack);
    }

    while (stack->size > 0) {
        stack_element_t element = pop(stack);
        print_stack_element(&element);
        printf(", ");
        print_stack(stack);
    }

    free_stack(stack);
}
