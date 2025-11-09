# Specification

## What is a hash table?

A hash table stores data in **key-value pairs**. This means that each piece of data (value) has an associated key.
The main benefit of a hash table is that, if the key is known, we can retrieve its associated value close to `O(1)` (constant) time. 
This is much faster than searching through an array to find something which has a time complexity of `O(n)`.
However, this advantage comes at the cost of wasted memory.

A good hash table implementation should:
- Insert, retrieve, and delete data close to `O(1)` time
- Waste as little memory as possible and keep the unused table slots at a minimum
- Distribute the data evenly across the table
- Have a good resizing strategy

## Implementation details

### Hash function

For simplicity, I've chosen the **division method**, which works by dividing the key by the table size and taking the remainder.
`hash = key % table_size`.

### Collision handling

A collision occurs when two different keys produce the same hash. To handle this, I have chosen the method of using **linked lists**.
Each table slot contains a pointer to a linked list. Colliding entries are appended to the list. When we want to retrieve or delete an
element, we traverse through the linked list until we find the key.

Worst case, this produces an `O(n)` search time but with a good hash function the list should be relatively short. 
This provides a close to constant search time.

### Table size, resizing

The table size should always be a **prime number**. This prevents the table size and the keys from having a common factor.
This gives an even distribution across the table. The initial size is **53**.

When the table starts to fill up, it needs to be resized. To decide when to resize, we calculate the **load factor**:
`load_factor = total_number_of_entries / current_table_size`. 
When load factor exceeds **0.75** after an insertion, the table is immediately resized.
The next table size is calculated by first doubling the current size, then finding the closest prime that is larger than the new size.

All existing entries must be rehashed into the new table using the new table size.

### Memory allocation

Because the table size can be increased, the hash table must be dynamically allocated on the **heap**.
When increasing the size, the old table array is freed after all entries are migrated to new table.

### Load factor calculation

Formula: `load_factor = total_number_of_entries / current_table_size`

Calculated after each insertion.

### Key-value storage

- Key data type: `int`
- Value data type: `int`

### Insert, search, delete behavior

- When inserting first we check if the key exists. If so the existing entry is updated. Otherwise the new entry is prepended to the head of the list.
- The deletion function should return whether it was able to delete the data entry.
- The get method returns a pointer to the entry or `NULL` if it wasn't found.
- The hash table should be able to completely free itself from memory. Including all linked list nodes, and the buckets array.

### Iteration

The hash table has a foreach function that iterates over all key-value pairs. 
The iteration order is not guaranteed (depends on hash function and bucket order).
The foreach function accepts a callback function with signature: 
`void callback(int key, int value, void* user_data)`.
The callback is invoked once for each key-value pair in the table.

## Data persistence

The hash table can be saved and loaded into a `.txt` file. 

**File format**
```
count
key1 value1
key2 value2
key3 value3
...
```

The `count` is the total number of key-value pairs.

### Serialization

The foreach function iterates over all key-value pairs and writes them to the file.

### Deserialization

1. Read the count from the first line
2. Create a new empty hash table with initial size: `next_prime(count / 0.75)` (optimization to avoid resizing during load)
3. Read and insert each key-value pair using the standard insert function
4. Return the newly created hash table