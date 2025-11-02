#include <stdio.h>
#include <stdbool.h>

#include "hash_table.h"

int main(void) {
    HashTable* table = hash_table_create();

    for (int i = 0; i < 20; i++) {
        hash_table_insert(table, i, i * 10);
    }
    hash_table_print(table, false);

    printf("\n\n");

    for (int i = 20; i < 40; i++) {
        hash_table_insert(table, i, i * 10);
    }
    hash_table_print(table, false);

    hash_table_destroy(table);

    return 0;
}