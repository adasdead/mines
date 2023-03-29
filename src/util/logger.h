#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

#include <stdlib.h>
#include <stdarg.h>

#include "basic_types.h"

#define logger_info(format, ...)                                            \
    logger_log(LOGGER_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define logger_warn(format, ...)                                            \
    logger_log(LOGGER_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define logger_fatal(format, ...)                                           \
    do {                                                                    \
        logger_log(LOGGER_FATAL, __FILE__, __LINE__,                        \
                   format, ##__VA_ARGS__);                                  \
        exit(-1);                                                           \
    } while (0)

enum logger_level {
    LOGGER_INFO, LOGGER_WARN, LOGGER_FATAL,
};

void logger_log(enum logger_level level, const char *file_name,
                usize line_no, const char *__restrict format, ...);

#endif /* UTIL_LOGGER_H */