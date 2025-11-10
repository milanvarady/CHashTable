/**
* @file hash_table_core.c
 * @brief Core HashTable methods.
 *
 * Includes the create, destroy, insert, get, delete, and foreach HashTable methods
 */

#include <stdlib.h>

#include "hash_table.h"
#include "hash_table_internal.h"
#include "../debugmalloc/debugmalloc.h"

const size_t HT_INITIAL_SIZE = 53;
const double HT_LOAD_THRESHOLD = 0.75;

HashTable* hash_table_create(void) {
    return hash_table_create_with_size(HT_INITIAL_SIZE);
}

void hash_table_destroy(HashTable* table) {
    // Free all entries
    clear_buckets(table->buckets, table->size);

    // Free buckets array
    free(table->buckets);

    // Free table
    free(table);
}

void hash_table_insert(HashTable* table, int key, int value) {
    const size_t hash = hash_function(key, table->size);
    Entry* bucket = table->buckets[hash];

    // If the key already exists, modify it
    for (Entry* entry = bucket; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            entry->value = value;
            return;
        }
    }

    // Else prepend a new entry to the head of the bucket
    Entry* new_entry = (Entry *)malloc(sizeof(Entry));
    *new_entry = (Entry){
        .key = key,
        .value = value,
        .next = bucket
    };

    table->buckets[hash] = new_entry;

    table->count++;

    // Grow table if needed
    if (table->count > table->load_threshold_count) {
        hash_table_resize(table);
    }
}

const Entry* hash_table_get(const HashTable* table, int key) {
    const size_t hash = hash_function(key, table->size);
    const Entry* bucket = table->buckets[hash];

    for (const Entry* entry = bucket; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            return entry;
        }
    }

    return NULL;
}

bool hash_table_delete(HashTable* table, int key) {
    const size_t hash = hash_function(key, table->size);

    for (Entry** indirect = &table->buckets[hash]; *indirect != NULL; indirect = &(*indirect)->next) {
        if ((*indirect)->key == key) {
            Entry* to_delete = *indirect;
            *indirect = to_delete->next;
            free(to_delete);
            table->count--;
            return true;
        }
    }

    return false;
}

void hash_table_foreach(const HashTable* table, void (*callback)(int key, int value, void*), void* user_data) {
    for (size_t i = 0; i < table->size; i++) {
        Entry* bucket = table->buckets[i];
        for (Entry* entry = bucket; entry != NULL; entry = entry->next) {
            callback(entry->key, entry->value, user_data);
        }
    }
}