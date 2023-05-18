/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#ifndef GAME_DIFFICULTY
#define GAME_DIFFICULTY

#include "util/basic_types.h"

#define DIFFICULTY(n)   (&difficulties[n % DIFFICULTY_TOTAL])

enum difficulty_id {
    DIFFICULTY_BEGGINER,
    DIFFICULTY_INTERMEDIATE,
    DIFFICULTY_EXPERT,
    DIFFICULTY_TOTAL,
};

struct difficulty {
    enum difficulty_id      id;
    uint                    field_width;
    uint                    field_height;
    uint                    mines_count;
    
} static const difficulties[] = {
    { DIFFICULTY_BEGGINER,      9,  9, 10 },
    { DIFFICULTY_INTERMEDIATE, 16, 16, 40 },
    { DIFFICULTY_EXPERT,       30, 16, 99 },
};

typedef const struct difficulty *difficulty_t;

static const char *difficulty_name(const difficulty_t difficulty)
{
    switch (difficulty->id)
    {
        case DIFFICULTY_BEGGINER: return "BEGGINER";
        case DIFFICULTY_INTERMEDIATE: return "INTERMEDIATE";
        case DIFFICULTY_EXPERT: return "EXPERT";
        default: return "UNKNOWN";
    }
}

#endif /* GAME_DIFFICULTY */