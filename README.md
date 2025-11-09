# CHashTable

Hashtable implemented in C for a university project.

## Implementation

A simple hash table implementation in C with integer keys and values,
using linked list chaining for collisions and dynamic resizing for performance.

Read detailed [specification](docs/specification.md).

## Docs

- [API docs]()
- [Interactive mode](docs/interactive_mode.md)

## Example usage

To use the hash table in a project include the `.c` and `.h` files from the `src`
directory. Except `main.c`. Then include the public API header.

```c
#include "hash_table.h"

// Create HashTable
HashTable *table = hash_table_create();

// Insert key-value pair
hash_table_insert(table, 1, 10);

// Get entry
const Entry *entry = hash_table_get(table, 1);
printf("Key: %d\n", entry->key);
printf("Value: %d\n", entry->value);

// Delete entry
bool success = hash_table_delete(table, 1);

// Foreach
void print_all_key_value_pairs(int key, int value, void *user_data) {
    printf("Key: %d\n", key);
    printf("Value: %d\n", value);
}

hash_table_foreach(table, print_all_key_value_pairs, NULL);

// Save to file
hash_table_save(table, "hash_table.txt");

// Load from file
HashTable *loaded_table = hash_table_load("hash_table.txt");

// Print for debugging
hash_table_print(table, false);

// Free HashTable
hash_table_destroy(table);
```

## Compile

Compile using **CMake**.

**Compile:**
```shell
mkdir build
cd build
cmake ..
make
```

**Run interactive mode:**
```shell
./CHashTable
```

**Run tests:**
```shell
./CHashTable_tests
```

## External tools used

- Tests use the [µnit](https://nemequ.github.io/munit/) framework
- Docs generated with [Doxygen](https://www.doxygen.nl/)