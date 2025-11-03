#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "hash_table.h"
#include "debugmalloc.h"

// Internal function prototypes
static size_t calc_load_threshold_count(size_t size);
static HashTable* hash_table_create_with_size(size_t size);
static Entry** create_buckets(size_t size);
static void clear_buckets(Entry** buckets, size_t size);
static void hash_table_resize(HashTable* table);
static size_t hash_function(int key, size_t table_size);
static bool is_prime(size_t n);
static size_t next_prime(size_t n);

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

int hash_table_get(const HashTable* table, int key, int default_value) {
    const size_t hash = hash_function(key, table->size);
    const Entry* bucket = table->buckets[hash];

    for (const Entry* entry = bucket; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            return entry->value;
        }
    }

    return default_value;
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

// Internal struct methods

static Entry** create_buckets(size_t size) {
    Entry **buckets = (Entry **)malloc(sizeof(Entry *) * size);
    if (buckets == NULL) return NULL;

    // Set all buckets to NULL
    for (size_t i = 0; i < size; i++) {
        buckets[i] = NULL;
    }

    return buckets;
}

static HashTable* hash_table_create_with_size(size_t size) {
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

static void clear_buckets(Entry** buckets, size_t size) {
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

static size_t calc_load_threshold_count(size_t size) {
    return (size_t)((double)size * HT_LOAD_THRESHOLD);
}

static void hash_table_resize(HashTable* table) {
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

// Utilities

static size_t hash_function(int key, size_t table_size) {
    // Sanity check
    assert(table_size <= INT_MAX);
    int mod = key % (int)table_size;
    if (mod < 0) {
        mod += (int)table_size;
    }
    return (size_t)mod;
}

static bool is_prime(size_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;

    // Check if divisible by 2 or 3
    if (n % 2 == 0 || n % 3 == 0) return false;

    // Optimized checking for 6k-1 6k+1 integers
    for (size_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

static size_t next_prime(size_t n) {
    if (n % 2 == 0) n += 1;
    else            n += 2;

    while (true) {
        if (is_prime(n)) return n;
        n += 2;
    }
}
