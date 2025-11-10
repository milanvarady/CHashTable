/**
 * @file hash_table_utils.c
 * @brief Internal utility functions for HashTable
 */

#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "hash_table_internal.h"

size_t hash_function(int key, size_t table_size) {
    // Sanity check
    assert(table_size <= INT_MAX);
    int mod = key % (int)table_size;
    if (mod < 0) {
        mod += (int)table_size;
    }
    return (size_t)mod;
}

bool is_prime(size_t n) {
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

size_t next_prime(size_t n) {
    if (n % 2 == 0) n += 1;
    else            n += 2;

    while (true) {
        if (is_prime(n)) return n;
        n += 2;
    }
}