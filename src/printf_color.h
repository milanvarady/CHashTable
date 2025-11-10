//
// Created by Milán Várady on 2025.11.10.
//

#ifndef CHASHTABLE_PRINTF_COLOR_H
#define CHASHTABLE_PRINTF_COLOR_H

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
void printf_color(ColorCode code, const char* format, ...);

#endif //CHASHTABLE_PRINTF_COLOR_H