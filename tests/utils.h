#ifndef CHASHTABLE_UTILS_H
#define CHASHTABLE_UTILS_H

#include "munit.h"
#include "../src/hash_table/hash_table.h"
#include "../src/hash_table/hash_table_internal.h"

static inline void* hash_table_setup(const MunitParameter params[], void* user_data) {
    return hash_table_create();
}

static inline void hash_table_teardown(void* fixture) {
    hash_table_destroy((HashTable*)fixture);
}

#endif //CHASHTABLE_UTILS_H