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

HashTable* hash_table_create(void) {
    return hash_table_create_with_size(HT_INITIAL_SIZE);
}

bool hash_table_destroy(HashTable* table) {
    if (table == nullptr) return false;

    // Free all entries
    clear_buckets(table->buckets, table->size);

    // Free buckets array
    free(table->buckets);

    // Free table
    free(table);

    return true;
}

bool hash_table_insert(HashTable* table, int key, int value) {
    if (table == nullptr) return false;

    const size_t hash = hash_function(key, table->size);
    Entry* bucket = table->buckets[hash];

    // If the key already exists, modify it
    for (Entry* entry = bucket; entry != nullptr; entry = entry->next) {
        if (entry->key == key) {
            entry->value = value;
            return true;
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
    return true;
}

const Entry* hash_table_get(const HashTable* table, int key) {
    if (table == nullptr) return nullptr;

    const size_t hash = hash_function(key, table->size);
    const Entry* bucket = table->buckets[hash];

    for (const Entry* entry = bucket; entry != nullptr; entry = entry->next) {
        if (entry->key == key) {
            return entry;
        }
    }

    return nullptr;
}

bool hash_table_delete(HashTable* table, int key) {
    if (table == nullptr) return false;

    const size_t hash = hash_function(key, table->size);

    for (Entry** indirect = &table->buckets[hash]; *indirect != nullptr; indirect = &(*indirect)->next) {
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

bool hash_table_equal(const HashTable *table1, const HashTable *table2) {
    if (table1->count != table2->count) return false;

    for (size_t i = 0; i < table1->size; i++) {
        Entry* bucket = table1->buckets[i];
        for (Entry* ht_1entry = bucket; ht_1entry != nullptr; ht_1entry = ht_1entry->next) {
            const Entry *ht2_entry = hash_table_get(table2, ht_1entry->key);
            if (ht2_entry == nullptr) return false;
            if (ht_1entry->value != ht2_entry->value) return false;
        }
    }

    return true;
}

static void copy_callback(int key, int value, void *user_data) {
    HashTable *table = (HashTable *)(user_data);
    hash_table_insert(table, key, value);
}

HashTable *hash_table_copy(const HashTable *table) {
    if (table == nullptr) return nullptr;

    HashTable *new_table = hash_table_create_with_size(table->size);
    hash_table_foreach(table, copy_callback, new_table);

    return new_table;
}

void hash_table_foreach(const HashTable* table, void (*callback)(int key, int value, void*), void* user_data) {
    if (table == nullptr) return;

    for (size_t i = 0; i < table->size; i++) {
        Entry* bucket = table->buckets[i];
        for (Entry* entry = bucket; entry != nullptr; entry = entry->next) {
            callback(entry->key, entry->value, user_data);
        }
    }
}