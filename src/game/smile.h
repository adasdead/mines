#ifndef GAME_SMILE_H
#define GAME_SMILE_H

#include "graphics/renderer.h"

#include "util/matrix4x4.h"
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
    enum smile_state    state;
    float               x;
    
    void (*click_callback)(void);

    mat4                model;
    struct renderer     renderer;
};

typedef struct smile *smile_t;

smile_t smile_create(void (*click_callback)(void));

void smile_update_model_matrix(smile_t smile);
void smile_render(const smile_t smile, mat4 projection);
void smile_handle_mouse(smile_t smile, uint x, uint y, bool press);
void smile_free(smile_t smile);

#endif /* GAME_SMILE_H */