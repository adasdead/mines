#include "logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define ANSI_RESET                      "\033[0m"
#define ANSI_RED_BACKGROUND             "\033[0;101m"
#define ANSI_YLW_BACKGROUND             "\033[0;103m"
#define ANSI_GRN_BACKGROUND             "\033[0;102m"

void logger_log(enum logger_level level, const char *file_name,
                size_t line_no,  const char *__restrict format, ...)
{
    const char *color, *status;
    va_list args_list;
    FILE *stream = stdout;

    va_start(args_list, format);

    if (strrchr(file_name, '/') != NULL) {
        file_name = strrchr(file_name, '/') + 1;
    }

    switch (level) {
    case LOGGER_WARN:
        color = ANSI_YLW_BACKGROUND;
        status = "WARN";
        stream = stderr;
        break;
        
    case LOGGER_FATAL:
        color = ANSI_RED_BACKGROUND;
        status = "!FATAL!";
        stream = stderr;
        break;

    default:
    case LOGGER_INFO:
        color = ANSI_GRN_BACKGROUND;
        status = "INFO";
    }

    fprintf(stream, "[%s:%zu] %s%s"ANSI_RESET" ", file_name, line_no,
           color, status);
    vfprintf(stream, format, args_list);
    fprintf(stream, "\n");

    va_end(args_list);
}