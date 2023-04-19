#ifndef GAME_SMILE_H
#define GAME_SMILE_H

#include "graphics/opengl.h"

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
    float               x;
    enum smile_state    state;
    mat4                model;
    
    void (*click_callback)(void);

    struct {
        GLuint          VAO;
        GLuint          VBO;
    } render;
};

typedef struct smile *smile_t;

smile_t smile_create(void (*click_callback)(void));
void smile_update_width(smile_t smile, uint width);
void smile_render(const smile_t smile, mat4 projection);
void smile_mouse(smile_t smile, uint x, uint y, bool press);
void smile_free(smile_t smile);

#endif /* GAME_SMILE_H */