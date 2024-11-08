#pragma once

#include <stdlib.h>

// free a pointer and set it to NULL
#define _free(ptr) do {             \
    if (ptr) {                      \
        free(ptr);                  \
        ptr = NULL;                 \
    }                               \
} while (0)
