#pragma once

#include "node.h"

token_number_t calculate(node_t *node);

token_number_t calculateExpression(node_t *node);
token_number_t calculateTerm(node_t *node);
token_number_t calculateFactor(node_t *node);
token_number_t calculatePrimary(node_t *node);
