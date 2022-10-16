#pragma once

#include <stdio.h>
#include "token.h"
#include "node.h"

void printToken(token_t token, FILE *out);

void printNode(node_t *node, FILE *out);
