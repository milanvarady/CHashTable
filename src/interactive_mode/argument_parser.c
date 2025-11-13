/**
 * @file argument_parser.c
 * @brief Argument parser API for interactive mode
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argument_parser_internal.h"

/** @brief Result type for str_to_int() */
typedef struct {
    int num;
    bool success;
} StrToIntResult;

/**
 * @brief Converts string to int with error reporting
 * @param str String to convert
 * @return StrToIntResult object with the int and success
 */
static StrToIntResult str_to_int(const char *str) {
    StrToIntResult result = {
        .num = 0,
        .success = false
    };

    if (str == NULL) return result;

    char *endptr;
    const int num = (int)strtol(str, &endptr, 10);

    // No digits were found at the beginning of the string
    if (endptr == str) return result;

    result.num = num;
    result.success = true;
    return result;
}

/**
 * @brief Prints red-colored error
 * @param stream Stream to print to
 * @param err Error string
 */
static void print_error(FILE *stream, const char *err) {
    fprintf_color(stream, ERROR_COLOR, "%s\n", err);
}

ParsedInput get_input(FILE *input_stream, FILE *output_stream) {
    ParsedInput result = {
        .cmd = CMD_INVALID,
        .arg_key = -1,
        .arg_value = -1
    };

    // Print prompt
    fprintf_color(output_stream, PROMPT_COLOR, "> ");

    // Get input from input stream
    char line[MAX_CMD_LEN];
    if (fgets(line, sizeof(line), input_stream) == NULL) {
        print_error(output_stream, "Failed to read input");
        return result;
    }

    // Try to parse command
    const char *cmd_str = strtok(line, DELIMITERS);
    if (cmd_str == NULL) {
        print_error(output_stream, "No command");
        return result;
    }

    for (int i = 0; i < CMD_COUNT; i++) {
        if (strcmp(cmd_str, command_table[i].cmd_name) == 0) {
            result.cmd = command_table[i].result_value;
        }
    }

    if (result.cmd == CMD_INVALID) {
        print_error(output_stream, "Command not found");
        return result;
    }

    const bool cmd_has_key_param = (result.cmd == CMD_ADD | result.cmd == CMD_GET | result.cmd == CMD_DEL);
    const bool cmd_has_value_param = result.cmd == CMD_ADD;

    // If the command has a key param try to parse it
    if (!cmd_has_key_param) return result;

    const char *arg_key_str = strtok(NULL, DELIMITERS);
    StrToIntResult key_conversion = str_to_int(arg_key_str);
    if (!key_conversion.success) {
        result.cmd = CMD_INVALID;
        print_error(output_stream, "Key argument not provided or invalid");
        return result;
    }

    result.arg_key = key_conversion.num;

    // If the command has a value param try to parse it
    if (!cmd_has_value_param) return result;

    const char *arg_value_str = strtok(NULL, DELIMITERS);
    StrToIntResult value_conversion = str_to_int(arg_value_str);
    if (!value_conversion.success) {
        result.cmd = CMD_INVALID;
        print_error(output_stream, "Value argument not provided or invalid");
        return result;
    }

    result.arg_value = value_conversion.num;

    return result;
}