/**
* @file main.c
 * @brief Interactive program to play with the HashTable
 */

#include <stdio.h>

#include "hash_table/hash_table.h"
#include "interactive_mode/interactive_mode.h"

int main(void) {
    // HashTable* table = hash_table_create();

    while (true) {
        InteractiveModeInput input = get_input(stdin, stdout);
        printf("%d %d\n", input.arg_key, input.arg_value);
        if (input.cmd == CMD_EXIT) break;
    }

    return 0;
}