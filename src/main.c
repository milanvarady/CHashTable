#include <stdio.h>
#include <stdbool.h>

#include "hash_table.h"

int main(void) {
    HashTable* table = hash_table_create();

    hash_table_insert(table, 1, 10);
    hash_table_insert(table, 2, 20);
    hash_table_insert(table, 54, 540);      // 53 + 1
    hash_table_insert(table, 107, 1070);    // 2 * 53 + 1

    hash_table_print(table, false);

    hash_table_delete(table, 2);

    int r1 = hash_table_get(table, 1, -1);
    int r2 = hash_table_get(table, 2, -1);
    int r3 = hash_table_get(table, 3, -1);
    int r4 = hash_table_get(table, 107, -1);

    printf("%d\n", r1);
    printf("%d\n", r2);
    printf("%d\n", r3);
    printf("%d\n", r4);

    hash_table_destroy(table);

    return 0;
}