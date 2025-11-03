#ifndef CHASHTABLE_HASH_TABLE_INTERNAL_H
#define CHASHTABLE_HASH_TABLE_INTERNAL_H

#include "hash_table.h"

const size_t HT_INITIAL_SIZE;
const double HT_LOAD_THRESHOLD;

struct entry {
    int key;
    int value;
    struct entry *next;
};

struct hash_table {
    Entry **buckets;
    size_t size;
    size_t count;
    size_t load_threshold_count;
};

// Internal function prototypes
size_t calc_load_threshold_count(size_t size);
HashTable* hash_table_create_with_size(size_t size);
Entry** create_buckets(size_t size);
void clear_buckets(Entry** buckets, size_t size);
void hash_table_resize(HashTable* table);
size_t hash_function(int key, size_t table_size);
bool is_prime(size_t n);
size_t next_prime(size_t n);

#endif //CHASHTABLE_HASH_TABLE_INTERNAL_H