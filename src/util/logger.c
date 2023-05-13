#include "logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define ANSI_RESET                              "\033[0m"
#define ANSI_RED_BACKGROUND                     "\033[0;101m"
#define ANSI_YLW_BACKGROUND                     "\033[0;103m"
#define ANSI_GRN_BACKGROUND                     "\033[0;102m"
#define PATH_SEPARATOR                          '/'

#define FORMAT                                  "[%s:%zu] %s%s"ANSI_RESET" "

static void get_color_and_status(enum logger_level level, const char **color,
                                 const char **status)
{
    switch (level) {
    case LOGGER_WARN:
        *color = ANSI_YLW_BACKGROUND;
        *status = "WARN";
        break;
        
    case LOGGER_FATAL:
        *color = ANSI_RED_BACKGROUND;
        *status = "!FATAL!";
        break;

    default:
    case LOGGER_INFO:
        *color = ANSI_GRN_BACKGROUND;
        *status = "INFO";
    }
}

void logger_log(enum logger_level level, const char *file_name,
                size_t line_no, const char *restrict format, ...)
{
    const char *background_color, *status;
    FILE *stream = stdout;
    va_list args_list;

    va_start(args_list, format);

    if (strrchr(file_name, PATH_SEPARATOR) != NULL) {
        file_name = strrchr(file_name, PATH_SEPARATOR) + 1;
    }

    if (level == LOGGER_WARN || level == LOGGER_FATAL) {
        stream = stderr;
    }

    get_color_and_status(level, &background_color, &status);

    fprintf(stream, FORMAT, file_name, line_no, background_color, status);
    vfprintf(stream, format, args_list);
    fputc('\n', stream);

    va_end(args_list);
}