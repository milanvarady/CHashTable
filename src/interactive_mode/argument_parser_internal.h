/**
 * @file argument_parser_internal.h
 * @brief Argument parser internal API exposed for testing
 */

#ifndef CHASHTABLE_ARGUMENT_PARSER_INTERNAL_H
#define CHASHTABLE_ARGUMENT_PARSER_INTERNAL_H

#include "interactive_mode.h"
#include "../printf_color/fprintf_color.h"

constexpr size_t MAX_CMD_LEN = 128;
constexpr char DELIMITERS[] = " \t\r\n";
constexpr size_t CMD_COUNT = 7;

/** @brief Maps command string to enum */
typedef struct {
    const char cmd_name[16];
    Command result_value;
} CommandMap;

/** @brief Maps command string to enum */
constexpr CommandMap command_table[CMD_COUNT] = {
    {"add", CMD_ADD},
    {"get", CMD_GET},
    {"del", CMD_DEL},
    {"save", CMD_SAVE},
    {"load", CMD_LOAD},
    {"print", CMD_PRINT},
    {"exit", CMD_EXIT}
};

#endif //CHASHTABLE_ARGUMENT_PARSER_INTERNAL_H