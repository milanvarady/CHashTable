#ifndef CHASHTABLE_HASH_TABLE_H
#define CHASHTABLE_HASH_TABLE_H

#include <stdbool.h>

static const size_t HT_INITIAL_SIZE = 53;
static const double HT_LOAD_THRESHOLD = 0.75;

typedef struct entry {
    int key;
    int value;
    struct entry *next;
} Entry;

typedef struct hash_table {
    Entry **buckets;
    size_t size;
    size_t count;
    size_t load_threshold_count;
} HashTable;

HashTable* hash_table_create(void);
void hash_table_destroy(HashTable* table);
void hash_table_insert(HashTable* table, int key, int value);
int hash_table_get(const HashTable* table, int key, int default_value);
bool hash_table_delete(HashTable* table, int key);
void hash_table_foreach(const HashTable* table, void (*callback)(int key, int value, void*), void* user_data);
void hash_table_save(const HashTable* table, const char* filename);
HashTable* hash_table_load(const char* filename);
void hash_table_print(const HashTable* table, bool print_empty_buckets);

#endif //CHASHTABLE_HASH_TABLE_H