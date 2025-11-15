/**
 * @file fprintf_color.h
 * @brief Header file for fprintf_color() function
 */

#ifndef CHASHTABLE_PRINTF_COLOR_H
#define CHASHTABLE_PRINTF_COLOR_H

/**
 * @brief Print colors for printf_color()
 */
typedef enum {
    COLOR_RESET,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_BOLD_RED
} ColorCode;

/**
 * @brief Prints a formatted string to stdout in a specified color
 * @param code The ColorCode enum value (e.g., COLOR_RED)
 * @param format The printf-style format string
 * @param ...    The variable arguments for the format string.
 */
void fprintf_color(FILE *stream, ColorCode color_code, const char *format, ...);

#endif //CHASHTABLE_PRINTF_COLOR_H
