#include <stdlib.h>

#include "hash_table.h"
#include "debugmalloc.h"

static const size_t INITIAL_SIZE = 53;
static const double LOAD_THRESHOLD = 0.75;

// Internal function prototypes
static double hash_table_load_factor(const HashTable* table);
static HashTable* hash_table_create_with_size(size_t size);
static Entry** create_buckets(size_t size);
static void clear_buckets(Entry** buckets, size_t size);
static void hash_table_resize(HashTable* table);
static int hash_function(int key, size_t table_size);
static bool is_prime(size_t n);
static size_t next_prime(size_t n);

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
    return hash_table_create_with_size(INITIAL_SIZE);
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

    table->count++;

    // Grow table if needed
    double load_factor = hash_table_load_factor(table);
    if (load_factor > LOAD_THRESHOLD) {
        hash_table_resize(table);
    }
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

bool hash_table_delete(HashTable* table, int key) {
    const int hash = hash_function(key, table->size);
    Entry* bucket = table->buckets[hash];

    // If first element, replace bucket
    if (bucket->key == key) {
        table->buckets[hash] = bucket->next;
        free(bucket);
        table->count--;
        return true;
    }

    // Else traverse to find the key
    Entry* previous = bucket;
    for (Entry* entry = bucket->next; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            previous->next = entry->next;
            free(entry);
            table->count--;
            return true;
        }

        previous = entry;
    }

    return false;
}

void hash_table_foreach(const HashTable* table, void (*callback)(int key, int value, void*), void* user_data) {
    for (int i = 0; i < table->size; i++) {
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

// Internal struct methods

static Entry** create_buckets(size_t size) {
    Entry **buckets = (Entry **)malloc(sizeof(Entry *) * size);
    if (buckets == NULL) return NULL;

    // Set all buckets to NULL
    for (int i = 0; i < size; i++) {
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
        .count = 0
    };

    return hash_table;
}

static void clear_buckets(Entry** buckets, size_t size) {
    for (int i = 0; i < size; i++) {
        Entry* head = buckets[i];

        while (head != NULL){
            Entry *tmp = head;
            head = head->next;
            free(tmp);
        }

        buckets[i] = NULL;
    }
}

static double hash_table_load_factor(const HashTable* table) {
    return (double)(table->count) / (double)(table->size);
}

static void hash_table_resize(HashTable* table) {
    const size_t new_size = next_prime(table->size * 2);
    const size_t old_size = table->size;

    // Try to allocate new buckets
    Entry** new_buckets = create_buckets(new_size);
    if (new_buckets == NULL) return;

    // Swap buckets
    Entry** old_buckets = table->buckets;
    table->buckets = new_buckets;
    table->size = new_size;
    table->count = 0;

    // Rehash entries into new buckets
    for (int i = 0; i < old_size; i++) {
        Entry* bucket = old_buckets[i];
        for (Entry* entry = bucket; entry != NULL; entry = entry->next) {
            hash_table_insert(table, entry->key, entry->value);
        }
    }

    clear_buckets(old_buckets, old_size);
    free(old_buckets);
}

// Utilities

static int hash_function(int key, size_t table_size) {
    return key % (int)table_size;
}

static bool is_prime(size_t n) {
    // Base cases
    if (n <= 1) return false;
    if (n <= 3) return true;

    for (size_t i = 5; i * i <= n; i += 2) {
        if (n % i == 0) {
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
