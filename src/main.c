/**
* @file main.c
 * @brief Interactive program to play with the HashTable
 */

#include <stdio.h>
#include <stdlib.h>

#include "hash_table/hash_table.h"
#include "interactive_mode/interactive_mode.h"
#include "fprintf_color/fprintf_color.h"

static constexpr char WELCOME_MESSAGE[] =
    "CHashTable Interactive mode\n"
    "Type 'help' for available commands\n";

static constexpr char HELP_MESSAGE[] =
    "Available commands:\n"
    "- `add {key} {value}`: adds a key-value pair to the table\n"
    "- `get {key}`: get a value associated to the key\n"
    "- `del {key}`: deletes an entry\n"
    "- `save`: saves the current table\n"
    "- `load`: loads the last saved table\n"
    "- `print`: prints the table\n"
    "- `exit`: exits interactive mode\n";

static constexpr char SAVE_FILENAME[] = "interactive_mode_table.txt";

int main(void) {
    HashTable* table = hash_table_create();
    if (table == NULL) {
        fprintf_color(stdout, ERROR_COLOR, "Failed to create hash table\n");
        exit(1);
    }

    printf("%s\n", WELCOME_MESSAGE);

    bool is_running = true;
    while (is_running) {
        const ParsedInput input = get_input(stdin, stdout);

        switch (input.cmd) {
            case CMD_ADD:
                hash_table_insert(table, input.arg_key, input.arg_value);
                break;

            case CMD_GET:
                const Entry *entry = hash_table_get(table, input.arg_key);
                if (entry == NULL) fprintf_color(stdout, ERROR_COLOR, "Entry doesn't exist\n");
                else fprintf_color(stdout, OUTPUT_COLOR, "%d\n", entry->value);
                break;

            case CMD_DEL:
                bool delete_success = hash_table_delete(table, input.arg_key);
                if (!delete_success)
                    fprintf_color(stdout, ERROR_COLOR, "Entry doesn't exist\n");
                break;

            case CMD_SAVE:
                bool save_success = hash_table_save(table, SAVE_FILENAME);
                if (!save_success)
                    fprintf_color(stdout, ERROR_COLOR, "Failed to save\n");
                break;

            case CMD_LOAD:
                HashTable *loaded_table = NULL;
                HashTable_LoadError error = hash_table_load(SAVE_FILENAME, &loaded_table);
                if (error != HT_LOAD_OK) {
                    fprintf_color(
                        stdout,
                        ERROR_COLOR,
                        "Failed to load hash table: %s\n", hash_table_error_string(error)
                    );
                    break;
                }
                HashTable *to_destory = table;
                table = loaded_table;
                hash_table_destroy(to_destory);
                break;

            case CMD_PRINT:
                hash_table_print(table, false);
                break;

            case CMD_EXIT:
                is_running = false;
                break;

            case CMD_HELP:
                printf("%s\n", HELP_MESSAGE);
                break;

            case CMD_INVALID:
                // get_input() prints error messages
                break;

            default:
                fprintf_color(stdout, ERROR_COLOR, "Unknown error\n");
                break;
        }
    }

    hash_table_destroy(table);
    remove(SAVE_FILENAME);

    return 0;
}
