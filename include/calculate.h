#pragma once

#include "node.h"

int calculate(node_t *node);

int calculateExpression(node_t *node);
int calculateTerm(node_t *node);
int calculateFactor(node_t *node);
int calculatePrimary(node_t *node);
