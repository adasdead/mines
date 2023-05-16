/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#ifndef GAME_CELL_H
#define GAME_CELL_H

#include "util/basic_types.h"

enum cell_states {
    CELL_STATE_CLOSED,
    CELL_STATE_OPENED,
    CELL_STATE_FLAGGED,
    CELL_STATE_QUESTIONED,
};

enum cell_types {
    CELL_TYPE_EMPTY,
    CELL_TYPE_1,
    CELL_TYPE_2,
    CELL_TYPE_3,
    CELL_TYPE_4,
    CELL_TYPE_5,
    CELL_TYPE_6,
    CELL_TYPE_7,
    CELL_TYPE_8,
    CELL_TYPE_BOMB,
    CELL_TYPE_NO_BOMB,
    CELL_TYPE_BOMB_E,
    CELL_TYPE_CLOSE,
    CELL_TYPE_QUESTION,
    CELL_TYPE_FLAG,
};

struct cell {
    byte                type  : 4;
    byte                state : 4;
};

typedef struct cell *cell_t;

#endif /* GAME_CELL_H */