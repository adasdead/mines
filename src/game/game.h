#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "math/matrix4x4.h"

#include "util/basic_types.h"

void game_new(void);
void game_init(void);
void game_loop(void);

void game_on_left_click(i32 x, i32 y, bool press);
void game_on_right_click(i32 x, i32 y, bool press);

void game_toggle_difficulty(void);

void game_free(void);

#endif /* GAME_GAME_H */