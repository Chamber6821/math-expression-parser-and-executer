#pragma once

#include "builder.h"

builder_t **createRules(builder_t **rules);
build_result_t buildByRules(builder_t **rules, stack_t *source);
