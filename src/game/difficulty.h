#ifndef GAME_DIFFICULTY
#define GAME_DIFFICULTY

#include "util/basic_types.h"

struct game_difficulty {
    usize id;
    const char name[32];
    u32 field_width, field_height;
    usize mines_count;
};

typedef struct game_difficulty *game_difficulty_t;

static struct game_difficulty game_difficulties[] = {
    { 0,    "BEGGINER",          9,      9,    10 },
    { 1,    "INTERMEDIATE",     16,     16,    40 },
    { 2,    "EXPERT",           30,     16,    99 },
};

#endif /* GAME_DIFFICULTY */