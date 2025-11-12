/**
 * @file interactive_mode.h
 * @brief Argument parser API for interactive mode
 */

#ifndef CHASHTABLE_INTERACTIVE_MODE_H
#define CHASHTABLE_INTERACTIVE_MODE_H

#include <stdint.h>

/**
 * @brief Interactive mode commands
 */
typedef enum {
    CMD_INVALID,
    CMD_ADD,
    CMD_GET,
    CMD_DEL,
    CMD_SAVE,
    CMD_LOAD,
    CMD_PRINT,
    CMD_EXIT
} Command;

/**
 * @brief Interactive mode command line input
 *
 * Returned by get_input().
 */
typedef struct {
    Command cmd;        /**< Command*/
    int arg_key;        /**< Argument 1, hash table key*/
    int arg_value;      /**< Argument 2, hash table value */
} ParsedInput;

/**
 * @brief Prompts user for interactive mode command and arguments
 * @param input_stream Stream to get the input from
 * @param output_stream Stream to print to
 * @return ParsedInput object with parsed command and argument
 */
ParsedInput get_input(FILE *input_stream, FILE *output_stream);

#endif //CHASHTABLE_INTERACTIVE_MODE_H