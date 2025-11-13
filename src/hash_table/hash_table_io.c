/**
 * @file hash_table_io.c
 * @brief Persistence and print methods for HashTable
 */

#include <stdio.h>

#include "hash_table.h"
#include "hash_table_internal.h"

constexpr char VERSION[16] = "1.0";

void hash_table_save(const HashTable* table, const char* filename) {
    FILE *file_handle = fopen(filename, "w");

    fprintf(file_handle, "CHashTable v%s\n", VERSION);
    fprintf(file_handle, "%zu\n", table->count);

    fclose(file_handle);
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

    for (size_t i = 0; i < size; i++) {
        Entry* bucket = table->buckets[i];
        if (bucket == NULL && !print_empty_buckets) {
            if (i != 0 && i != size - 1 && previous_wasnt_empty) {
                printf("...\n");
                previous_wasnt_empty = false;
            }

            continue;
        }

        printf("%zu: ", i);

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