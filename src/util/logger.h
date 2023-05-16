/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

#include <stdlib.h>
#include <stdarg.h>

#include "basic_types.h"

enum logger_level {
    LOGGER_INFO, LOGGER_WARN, LOGGER_FATAL,
};

#if DEBUG

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

void
logger_log(enum logger_level level, const char *file_name, size_t line_no,
           const char *restrict format, ...);

#else
#define logger_info(format, ...)
#define logger_warn(format, ...)
#define logger_fatal(format, ...) exit(-1)

static void
logger_log(enum logger_level level, const char *file_name, size_t line_no,
           const char *restrict format, ...)
{
}
#endif /* DEBUG */

#endif /* UTIL_LOGGER_H */