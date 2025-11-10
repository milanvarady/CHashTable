/**
* @file hash_table_resize.c
 * @brief Internal methods for creating and resizing a HashTable
 */

#include <stdlib.h>

#include "hash_table.h"
#include "hash_table_internal.h"
#include "../debugmalloc/debugmalloc.h"

Entry** create_buckets(size_t size) {
    Entry **buckets = (Entry **)malloc(sizeof(Entry *) * size);
    if (buckets == NULL) return NULL;

    // Set all buckets to NULL
    for (size_t i = 0; i < size; i++) {
        buckets[i] = NULL;
    }

    return buckets;
}

HashTable* hash_table_create_with_size(size_t size) {
    // Allocate memory
    Entry **buckets = create_buckets(size);
    if (buckets == NULL) return NULL;

    HashTable *hash_table = (HashTable *)malloc(sizeof(HashTable));
    if (hash_table == NULL) return NULL;

    // Create table with default size
    *hash_table = (HashTable){
        .buckets = buckets,
        .size = size,
        .count = 0,
        .load_threshold_count = calc_load_threshold_count(size)
    };

    return hash_table;
}

void clear_buckets(Entry** buckets, size_t size) {
    for (size_t i = 0; i < size; i++) {
        Entry* head = buckets[i];

        while (head != NULL){
            Entry *tmp = head;
            head = head->next;
            free(tmp);
        }

        buckets[i] = NULL;
    }
}

size_t calc_load_threshold_count(size_t size) {
    return (size_t)((double)size * HT_LOAD_THRESHOLD);
}

void hash_table_resize(HashTable* table) {
    const size_t new_size = next_prime(table->size * 2);
    const size_t old_size = table->size;

    // Try to allocate new buckets
    Entry** new_buckets = create_buckets(new_size);
    // Silently fail
    if (new_buckets == NULL) return;

    // Swap buckets
    Entry** old_buckets = table->buckets;
    table->buckets = new_buckets;
    table->size = new_size;
    table->count = 0;
    table->load_threshold_count = calc_load_threshold_count(new_size);

    // Rehash entries into new buckets
    for (size_t i = 0; i < old_size; i++) {
        Entry* bucket = old_buckets[i];
        for (Entry* entry = bucket; entry != NULL; entry = entry->next) {
            hash_table_insert(table, entry->key, entry->value);
        }
    }

    clear_buckets(old_buckets, old_size);
    free(old_buckets);
}