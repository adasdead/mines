#ifndef GAME_DIFFICULTY
#define GAME_DIFFICULTY

#include "util/basic_types.h"

#define DIFFICULTY_TOTAL                                \
    (sizeof difficulties / sizeof *difficulties)

#define DIFFICULTY(n) (&difficulties[n])

struct difficulty {
    const char          *name;
    uint                 field_width;
    uint                 field_height;
    size_t               mines_count;
} static const difficulties[] = {
    { "BEGGINER",          9,      9,    10 },
    { "INTERMEDIATE",     16,     16,    40 },
    { "EXPERT",           30,     16,    99 },
};

typedef const struct difficulty *difficulty_t;

#endif /* GAME_DIFFICULTY */