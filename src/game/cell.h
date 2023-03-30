#ifndef GAME_CELL_H
#define GAME_CELL_H

#include "util/basic_types.h"

#define CELL_STATE_CLOSED               0
#define CELL_STATE_OPENED               1
#define CELL_STATE_FLAGGED              2
#define CELL_STATE_QUESTIONED           3

#define CELL_TYPE_EMPTY                 0
#define CELL_TYPE_1                     1
#define CELL_TYPE_2                     2
#define CELL_TYPE_3                     3
#define CELL_TYPE_4                     4
#define CELL_TYPE_5                     5
#define CELL_TYPE_6                     6
#define CELL_TYPE_7                     7
#define CELL_TYPE_8                     8
#define CELL_TYPE_BOMB                  9
#define CELL_TYPE_NO_BOMB               10
#define CELL_TYPE_BOMB_E                11
#define CELL_TYPE_CLOSE                 12
#define CELL_TYPE_QUESTION              13
#define CELL_TYPE_FLAG                  14

union cell {
    u8 byte;

    struct {
        u8 type  : 4;
        u8 state : 4;
    };
};

#endif /* GAME_CELL_H */