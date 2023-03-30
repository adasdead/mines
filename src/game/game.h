#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "math/matrix4x4.h"
#include "util/basic_types.h"

enum game_difficulty {
    GAME_DIF_BEGGINER,
    GAME_DIF_INTERMEDIATE,
    GAME_DIF_EXPERT,
    GAME_DIF_TOTAL,
};

void game_start(void);
void game_loop(m4x4 projection);
void game_toggle_difficulty(void);
void game_free(void);

#endif /* GAME_GAME_H */