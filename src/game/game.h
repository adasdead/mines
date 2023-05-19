/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "util/matrix4x4.h"
#include "util/basic_types.h"

void game_reset(void);
void game_toggle_difficulty(void);

void game_initialize(void);
void game_loop(void);

void game_on_left_click(int x, int y, bool press);
void game_on_right_click(int x, int y, bool press);

void game_free(void);

#endif /* GAME_GAME_H */