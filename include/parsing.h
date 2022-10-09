#pragma once

#include "source.h"
#include "token.h"

source_t *skipSpaces(source_t *source);
token_t parseToken(source_t *source);
