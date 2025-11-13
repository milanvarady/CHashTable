/**
 * @file hash_table_io.c
 * @brief Persistence and print methods for HashTable
 */

#include <stdio.h>
#include <mach/mach.h>

#include "hash_table.h"
#include "hash_table_internal.h"
#include "../interactive_mode/interactive_mode.h"

static constexpr char VERSION[16] = "1.0";
static constexpr size_t MAX_LINE = 256;
static constexpr char HEADER_PREFIX[] = "CHashTable v";

static void print_entry_to_file(int key, int value, void *file) {
    file = (FILE *)file;
    fprintf(file, "%d=%d\n", key, value);
}

bool hash_table_save(const HashTable* table, const char* filename) {
    if (table == NULL) return false;

    FILE *file = fopen(filename, "w");
    if (file == NULL) return false;

    fprintf(file, "CHashTable v%s\n", VERSION);
    fprintf(file, "%zu\n", table->count);
    hash_table_foreach(table, print_entry_to_file, file);
    fprintf(file, "\n");

    fclose(file);

    return true;
}

/**
 * @brief Loads a hash table from a specified file.
 */
HashTable_LoadError hash_table_load(const char* filename, HashTable** out_table) {
    // Initialize out_table to NULL in case of early failure
    *out_table = NULL;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return HT_LOAD_ERROR_FILE_OPEN;
    }

    char line[MAX_LINE];
    size_t count = 0;
    HashTable_LoadError error_code = HT_LOAD_OK;
    HashTable *table = NULL;

    // 1. Read and validate the header
    if (fgets(line, sizeof(line), file) == NULL) {
        error_code = HT_LOAD_ERROR_EMPTY;
        goto cleanup;
    }

    // 2. Check if the line starts with the required prefix
    if (strncmp(line, HEADER_PREFIX, strlen(HEADER_PREFIX)) != 0) {
        error_code = HT_LOAD_ERROR_INVALID_HEADER;
        goto cleanup;
    }

    // 3. Read count
    if (fgets(line, sizeof(line), file) == NULL) {
        error_code = HT_LOAD_ERROR_MISSING_COUNT;
        goto cleanup;
    }

    // 4. Parse the count from the line
    if (sscanf(line, "%zu", &count) != 1) {
        error_code = HT_LOAD_ERROR_MALFORMED_COUNT;
        goto cleanup;
    }

    // 5. Create the table
    size_t initial_size = (size_t)((double)count / HT_LOAD_THRESHOLD);
    initial_size = initial_size < HT_INITIAL_SIZE ? HT_INITIAL_SIZE : next_prime(initial_size);
    table = hash_table_create_with_size(initial_size);

    if (table == NULL) {
        error_code = HT_LOAD_ERROR_ALLOC_FAILED;
        goto cleanup;
    }

    // 6. Read key-value pairs
    int key;
    int value;
    size_t parsed_lines = 0; // Count of *successfully* parsed lines

    for (size_t i = 0; i < count; i++) {
        // Read the next line
        if (fgets(line, sizeof(line), file) == NULL) {
            // File ended before we read 'count' items
            error_code = HT_LOAD_ERROR_PREMATURE_EOF;
            goto cleanup; // This will destroy the partially-filled table
        }

        int parsed = sscanf(line, " %d = %d ", &key, &value);

        if (parsed == 2) {
            hash_table_insert(table, key, value);
            parsed_lines++; // Increment on success
        }
    }

    // Check if the number of successfully parsed lines matches the expected count
    if (parsed_lines != count) {
        error_code = HT_LOAD_ERROR_MALFORMED_LINE;
        goto cleanup;
    }

    // If we got here, all critical steps passed.
    *out_table = table;
    error_code = HT_LOAD_OK;

cleanup:
    fclose(file);
    if (error_code != HT_LOAD_OK && table != NULL) {
        hash_table_destroy(table);
        *out_table = NULL;
    }

    return error_code;
}

/**
 * @brief Converts a hash table load error code into a static, human-readable string.
 */
const char* hash_table_error_string(HashTable_LoadError error_code) {
    switch (error_code) {
        case HT_LOAD_OK:
            return "Success: Hash table loaded successfully.";
        case HT_LOAD_ERROR_FILE_OPEN:
            return "Error: Could not open the file.";
        case HT_LOAD_ERROR_EMPTY:
            return "Error: File is empty or header is unreadable.";
        case HT_LOAD_ERROR_INVALID_HEADER:
            return "Error: Invalid file header.";
        case HT_LOAD_ERROR_MISSING_COUNT:
            return "Error: File missing item count.";
        case HT_LOAD_ERROR_MALFORMED_COUNT:
            return "Error: Malformed count line.";
        case HT_LOAD_ERROR_ALLOC_FAILED:
            return "Error: Failed to create new hash table object.";
        case HT_LOAD_ERROR_PREMATURE_EOF:
            return "Error: File ended prematurely. Item count mismatch.";
        case HT_LOAD_ERROR_MALFORMED_LINE:
            return "Error: File contains malformed key-value pairs or item count mismatch.";
        default:
            return "Error: Unknown error code.";
    }
}

void hash_table_print(const HashTable* table, bool print_empty_buckets) {
    if (table == NULL) {
        fprintf_color(stdout, ERROR_COLOR, "Table is NULL!");
        return;
    }

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