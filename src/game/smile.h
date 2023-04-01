#ifndef GAME_SMILE_H
#define GAME_SMILE_H

#include "math/matrix4x4.h"

#include "util/basic_types.h"

enum smile_state {
    SMILE_STATE_DEFAULT,
    SMILE_STATE_CLICK,
    SMILE_STATE_O,
    SMILE_STATE_DEAD,
    SMILE_STATE_COOL,
    SMILE_STATE_TOTAL
};

struct smile {
    u32 field_width;
    enum smile_state state;
    bool clicked;
};

typedef struct smile *smile_t;

smile_t smile_create(u32 field_width, enum smile_state state);
void smile_render(const smile_t smile, mat4 projection);
void smile_mouse(smile_t smile, u32 x, u32 y);
void smile_free(smile_t smile);

#endif /* GAME_SMILE_H */