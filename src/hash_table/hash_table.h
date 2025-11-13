/**
 * @file hash_table.h
 * @brief Public API for HashTable
 */

#ifndef CHASHTABLE_HASH_TABLE_H
#define CHASHTABLE_HASH_TABLE_H

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
 * @return true if insertion was successful
 */
bool hash_table_insert(HashTable* table, int key, int value);

/**
 * @brief Get element from HashTable
 * @param table Pointer to HashTable object
 * @param key Key to retrieve
 * @return Entry object or nullptr if the element wasn't found
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
 * @return Success
 */
bool hash_table_save(const HashTable* table, const char* filename);

/**
 * @enum HashTable_LoadError
 * @brief Error codes for the hash_table_load function.
 */
typedef enum {
    HT_LOAD_OK = 0,               // Success
    HT_LOAD_ERROR_FILE_OPEN,      // Could not open the file
    HT_LOAD_ERROR_EMPTY,          // File is empty or header is unreadable
    HT_LOAD_ERROR_INVALID_HEADER, // Invalid file header prefix
    HT_LOAD_ERROR_MISSING_COUNT,  // Item count line is missing
    HT_LOAD_ERROR_MALFORMED_COUNT,// Failed to parse the item count
    HT_LOAD_ERROR_ALLOC_FAILED,   // Failed to allocate memory for the hash table
    HT_LOAD_ERROR_PREMATURE_EOF,  // File ended before all items were read
    HT_LOAD_ERROR_MALFORMED_LINE  // File contains malformed key-value pairs, or item count mismatch.
} HashTable_LoadError;

/**
 * @brief Loads a hash table from a specified file.
 *
 * Instead of returning the table, it returns an error code.
 * The created hash table is returned via the `out_table` pointer.
 * On error, `out_table` is set to nullptr (or an intermediate state).
 *
 * @param filename The path to the file to load.
 * @param out_table A pointer to a (HashTable*) variable. On success,
 * this will be set to point to the newly loaded table.
 * On failure, it will be set to nullptr.
 * @return A HashTable_LoadError code indicating success (HT_LOAD_OK) or the type of failure.
 */
HashTable_LoadError hash_table_load(const char* filename, HashTable** out_table);

/**
 * @brief Converts a hash table load error code into a static, human-readable string.
 *
 * @param error_code The HashTable_LoadError code.
 * @return A constant string describing the error.
 */
const char* hash_table_error_string(HashTable_LoadError error_code);

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