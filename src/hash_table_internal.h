/**
 * @file hash_table_internal.h
 * @brief Internal HashTable API and utilities
 */

#ifndef CHASHTABLE_HASH_TABLE_INTERNAL_H
#define CHASHTABLE_HASH_TABLE_INTERNAL_H

#include <stddef.h>
#include "hash_table.h"

/** @brief Initial hash table size, always a prime number */
extern const size_t HT_INITIAL_SIZE;
/** @brief The hash table grows if the count/size ratio exceeds this threshold */
extern const double HT_LOAD_THRESHOLD;

/**
 * @biref Internal implementation of the hash table
 * Holds the reference to the bucket array and other table information
 */
struct hash_table {
    Entry **buckets;                /**< Bucket array consists of linked lists */
    size_t size;                    /**< Table size. Always a prime number */
    size_t count;                   /**< Item count */
    size_t load_threshold_count;    /**< If the count exceeds this threshold, the table size will be increased */
};

/**
 * @brief Precalculates the load threshold count
 *
 * To reduce calculations, this method can be used to precalculate the threshold count.
 * This way the threshold is only calculated if the table size changes.
 *
 * @param size Table size
 * @return Threshold count
 */
size_t calc_load_threshold_count(size_t size);

/**
 * @brief Creates a new dynamically allocated HashTable object with a set size
 * @param size Table size
 * @return Pointer to a dynamically allocated HashTable object
 */
HashTable* hash_table_create_with_size(size_t size);

/**
 * @brief Creates a dynamically allocated bucket array with a set size
 * @param size Table size
 * @return Pointer to the bucket array
 */
Entry** create_buckets(size_t size);

/**
 * @brief Frees all entries inside a bucket array
 * @param buckets Bucket array
 * @param size Size of the bucket array
 */
void clear_buckets(Entry** buckets, size_t size);

/**
 * @brief Resizes the HashTable
 *
 * Steps:
 * -# Calculate new bucket array size. It is the next prime after the double of the current size, found using next_prime()
 * -# Create new buckets and swap out the old one
 * -# Rehash all entries into new buckets using hash_table_insert()
 * -# Free old bucket
 *
 * @param table Pointer to HashTable object
 */
void hash_table_resize(HashTable* table);

/**
 * @brief Hash function using division method
 *
 * The hash is `hash = key % table_size`. The hash is always positive.
 *
 * @param key Key to hash
 * @param table_size Hash table size
 * @return Hashed key
 */
size_t hash_function(int key, size_t table_size);

/** @brief Is prime function
 *
 * Using an optimized trial division method with the 6k ± 1 rule
 */
bool is_prime(size_t n);

/**
 * @brief Finds the next prime number after n
 *
 * Uses the is_prime() function.
 *
 * @param n The prime is searched after n
 * @return Prime number
 */
size_t next_prime(size_t n);

#endif //CHASHTABLE_HASH_TABLE_INTERNAL_H