/**
 * @file fprintf_color.c
 * @brief printf function with colored output using ANSI escape codes
 */

#include <stdio.h>
#include <stdarg.h>

#include "fprintf_color.h"

static const char* colors[] = {
    [COLOR_RESET]    = "\033[0m",
    [COLOR_RED]      = "\033[31m",
    [COLOR_GREEN]    = "\033[32m",
    [COLOR_BLUE]     = "\033[34m",
    [COLOR_BOLD_RED] = "\033[1;31m"
};

void fprintf_color(FILE *stream, ColorCode color_code, const char *format, ...) {
    printf("%s", colors[color_code]);

    va_list args;
    va_start(args, format);

    vfprintf(stream, format, args);

    va_end(args);

    printf("%s", colors[COLOR_RESET]);
}
