/**
 * @file hash_table.h
 * @brief Public API for HashTable
 */

#ifndef CHASHTABLE_HASH_TABLE_H
#define CHASHTABLE_HASH_TABLE_H

#include <stdbool.h>

/**
 * @brief A hash table entry object
 *
 * Holds a key-value pair
 */
typedef struct entry {
    int key;
    int value;
    struct entry *next;
} Entry;

/**
 * @brief An opaque handle to a hash table object
 * @relates hash_table
 *
 * This is the public-facing type. All functions in this
 * API (like hash_table_create(), hash_table_insert(), etc.)
 * operate on a pointer to this type (HashTable*).
 *
 * The internal structure is hidden from the user.
 */
typedef struct hash_table HashTable;

/**
 * @brief Creates a new empty HashTable object
 *
 * The HashTable is dynamically allocated on the heap. Free it with hash_table_destroy().
 *
 * @return Pointer to empty HashTable
 */
HashTable* hash_table_create(void);

/**
 * @brief Frees a HashTable from memory
 *
 * Completely frees all buckets and the hash table itself from memory
 *
 * @param table Hash table to free
 */
void hash_table_destroy(HashTable* table);

/**
 * @brief Inserts a key-value pair into a HashTable
 *
 * If the entry already exists the value will be updated
 *
 * @param table Pointer to HashTable object
 * @param key The key to insert
 * @param value The value to insert
 */
void hash_table_insert(HashTable* table, int key, int value);

/**
 * @brief Get element from HashTable
 * @param table Pointer to HashTable object
 * @param key Key to retrieve
 * @return Entry object or NULL if the element wasn't found
 */
const Entry* hash_table_get(const HashTable* table, int key);

/**
 * @brief Delete entry from HashTable
 * @param table Pointer to HashTable object
 * @param key The key of the entry to delete
 * @return true if the deletion was successful
 */
bool hash_table_delete(HashTable* table, int key);

/**
 * @brief Iterates through each key-value pair in a HashTable
 * @param table Pointer to HashTable object
 * @param callback A callback function that will be run for every key value pair
 * @param user_data Generic user data that is injected into the callback
 */
void hash_table_foreach(const HashTable* table, void (*callback)(int key, int value, void*), void* user_data);

/**
 * @brief Serializes a HashTable object into a .txt file
 *
 * Can be loaded with hash_table_load()
 *
 * @param table Pointer to HashTable object
 * @param filename Save file name
 */
void hash_table_save(const HashTable* table, const char* filename);

/**
 * @brief Loads a HashTable object saved with hash_table_save()
 * @param filename File to load the data from
 * @return Loaded HashTable or NULL
 */
HashTable* hash_table_load(const char* filename);

/**
 * @brief Print a HashTable for debugging
 *
 * Prints the table size, item count, and each bucket (key-value pairs)
 *
 * @param table Pointer to HashTable object
 * @param print_empty_buckets If false empty buckets are replaced with "...", more concise
 */
void hash_table_print(const HashTable* table, bool print_empty_buckets);

#endif //CHASHTABLE_HASH_TABLE_H