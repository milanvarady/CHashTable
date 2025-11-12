/**
 * @file argument_parser_internal.h
 * @brief Argument parser internal API exposed for testing
 */

#ifndef CHASHTABLE_ARGUMENT_PARSER_INTERNAL_H
#define CHASHTABLE_ARGUMENT_PARSER_INTERNAL_H

#include "interactive_mode.h"
#include "../printf_color/fprintf_color.h"

#define MAX_CMD_LEN 128
#define DELIMITERS " \t\r\n"
#define CMD_COUNT 7

static const ColorCode PROMPT_COLOR = COLOR_BLUE;

typedef struct {
    const char *cmd_name;
    Command result_value;
} CommandMap;

extern const CommandMap command_table[CMD_COUNT];

#endif //CHASHTABLE_ARGUMENT_PARSER_INTERNAL_H