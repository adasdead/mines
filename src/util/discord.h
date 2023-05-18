/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#ifndef UTIL_DISCORD_H
#define UTIL_DISCORD_H

#include <time.h>

#if DISCORD

void discord_initialize(void);
void discord_update_activity(const char *details, const char *state,
                             time_t start_time);
void discord_run_callbacks(void);
void discord_free(void);

#else

static void discord_initialize(void) {}
static void discord_update_activity(const char *details,
                                    const char *state,
                                    time_t start_time) {}
static void discord_run_callbacks(void) {}
static void discord_free(void) {}

#endif /* DISCORD */

#endif /* UTIL_DISCORD_H */