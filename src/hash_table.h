#ifndef CHASHTABLE_HASH_TABLE_H
#define CHASHTABLE_HASH_TABLE_H

#include <stdbool.h>

typedef struct entry Entry;
typedef struct hash_table HashTable;

HashTable* hash_table_create(void);
void hash_table_destroy(HashTable* table);
void hash_table_insert(const HashTable* table, int key, int value);
int hash_table_get(const HashTable* table, int key, int default_value);
bool hash_table_delete(const HashTable* table, int key);
void hash_table_foreach(const HashTable* table, void (*callback)(Entry*, void*), void* user_data);
void hash_table_save(const HashTable* table, const char* filename);
void hash_table_print(const HashTable* table, bool print_empty_buckets);

#endif //CHASHTABLE_HASH_TABLE_H