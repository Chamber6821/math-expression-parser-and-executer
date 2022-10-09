#pragma once

#include "token.h"

const char *binOperatorToString(enum bin_operator operator);
const char *tokenToString(token_t token, char *buffer);
