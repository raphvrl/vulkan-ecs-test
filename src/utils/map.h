#pragma once

#include <string.h>

#include "core/types.h"

#define INITAL_CAPACITY 8
#define GROWTH_FACTOR 2

typedef struct pair {
    char *key;
    void *value;
    bool used;
} pair_t;

// main struct
typedef struct map {
    pair_t *pairs;
    usize size;
    usize capacity;
} map_t;

// constructor and destructor
map_t *map_create();
void map_destroy(map_t *map);

// action
void map_put(map_t *map, const char *key, void *value);
void map_remove(map_t *map, const char *key);
void *map_get(map_t *map, const char *key);

#define map_pair(map, i) (&(map)->pairs[i])

// print
void map_print(map_t *map);