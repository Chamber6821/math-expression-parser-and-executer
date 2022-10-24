#pragma once

#include <ctype.h>
#include "utils.h"


char *lower(char *str) {
    for (int i = 0; str[i] != 0; i++) {
        str[i] = (char) tolower(str[i]);
    }
    return str;
}
