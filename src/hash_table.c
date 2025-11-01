#include <stdlib.h>

#include "hash_table.h"
#include "debugmalloc.h"

// Internal function prototypes
static int hash_function(int key, size_t table_size);

struct entry {
    int key;
    int value;
    struct entry *next;
};

struct hash_table {
    Entry **buckets;
    size_t size;
    size_t count;
};

HashTable* hash_table_create(void) {
    // Allocate memory
    const int initial_size = 53;

    Entry **buckets = (Entry **)malloc(sizeof(Entry *) * initial_size);
    if (buckets == NULL) return NULL;

    HashTable *hash_table = (HashTable *)malloc(sizeof(HashTable));
    if (hash_table == NULL) return NULL;

    // Create table with default size
    *hash_table = (HashTable){
        .buckets = buckets,
        .size = initial_size,
        .count = 0
    };

    // Set all buckets to NULL
    for (int i = 0; i < initial_size; i++) {
        hash_table->buckets[i] = NULL;
    }

    return hash_table;
}

void hash_table_destroy(HashTable* table) {
    // Free all entries
    for (int i = 0; i < table->size; i++) {
        Entry* head = table->buckets[i];

        Entry* tmp;

        while (head != NULL){
            tmp = head;
            head = head->next;
            free(tmp);
        }
    }

    // Free buckets array
    free(table->buckets);

    // Free table
    free(table);
}

void hash_table_insert(const HashTable* table, int key, int value) {
    const int hash = hash_function(key, table->size);
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
}

int hash_table_get(const HashTable* table, int key, int default_value) {
    const int hash = hash_function(key, table->size);
    const Entry* bucket = table->buckets[hash];

    for (const Entry* entry = bucket; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            return entry->value;
        }
    }

    return default_value;
}

bool hash_table_delete(const HashTable* table, int key) {
    const int hash = hash_function(key, table->size);
    Entry* bucket = table->buckets[hash];

    // If first element, replace bucket
    if (bucket->key == key) {
        table->buckets[hash] = bucket->next;
        free(bucket);
        return true;
    }

    // Else traverse to find the key
    Entry* previous = bucket;
    for (Entry* entry = bucket->next; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            previous->next = entry->next;
            free(entry);
            return true;
        }

        previous = entry;
    }

    return false;
}

void hash_table_foreach(const HashTable* table, void (*callback)(Entry*, void*), void* user_data) {
    for (int i = 0; i < table->size; i++) {
        Entry* bucket = table->buckets[i];
        for (Entry* entry = bucket; entry != NULL; entry = entry->next) {
            callback(entry, user_data);
        }
    }
}

void hash_table_save(const HashTable* table, const char* filename) {

}

HashTable* hash_table_load(const char* filename) {
    return NULL;
}

void hash_table_print(const HashTable* table, bool print_empty_buckets) {
    printf("################\n");
    printf("TABLE\n");
    printf("size: %zu\n", table->size);
    printf("count: %zu\n", table->count);
    printf("----------------\n");

    const size_t size = table->size;
    bool previous_wasnt_empty = false;

    for (int i = 0; i < size; i++) {
        Entry* bucket = table->buckets[i];
        if (bucket == NULL && !print_empty_buckets) {
            if (i != 0 && i != size - 1 && previous_wasnt_empty) {
                printf("...\n");
                previous_wasnt_empty = false;
            }

            continue;
        }

        printf("%d: ", i);

        if (bucket == NULL) {
            printf("NULL\n");
            continue;
        }

        for (Entry* entry = bucket; entry != NULL; entry = entry->next) {
            printf("(%d, %d)", entry->key, entry->value);
            if (entry->next != NULL) printf(" -> ");
        }
        printf("\n");
        previous_wasnt_empty = true;
    }

    printf("################\n");
}

// Internal functions

static int hash_function(int key, size_t table_size) {
    return key % (int)table_size;
}