#include "game/game.h"

#include "game/window.h"
#include "game/field.h"

#include "definitions.h"

static struct game_difficulty_info {
    enum game_difficulty dif;
    u32 field_width, field_height;
    usize mines_count;

} game_difficulties[GAME_DIF_TOTAL] = {
    { GAME_DIF_BEGGINER,      9,  9, 10,},
    { GAME_DIF_INTERMEDIATE, 16, 16, 40 },
    { GAME_DIF_EXPERT,       30, 16, 99 }
};

static struct game_difficulty_info *cur_difficulty = NULL;
static struct field *field = NULL;

static void game_new(void)
{
    u32 width, height;

    width = FIELD_MARGIN_RIGHT + FIELD_MARGIN_LEFT;
    height = FIELD_MARGIN_BOTTOM + FIELD_MARGIN_TOP;
    
    field_free(field);

    field = field_create(cur_difficulty->field_width,
                         cur_difficulty->field_height,
                         cur_difficulty->mines_count);

    window_resize(width + field->width, height + field->height);
}

void game_start(void)
{
    game_toggle_difficulty();
}

void game_loop(m4x4 projection)
{
    field_render(field, projection);
}

void game_toggle_difficulty(void)
{
    enum game_difficulty dif = GAME_DIF_BEGGINER;

    if (cur_difficulty != NULL) {
        if ((cur_difficulty->dif + 1) == GAME_DIF_TOTAL) {
            dif = GAME_DIF_BEGGINER;
        }
        else {
            dif = cur_difficulty->dif + 1;
        }
    }

    cur_difficulty = &game_difficulties[dif];

    game_new();
}

void game_free(void)
{
    field_free(field);
}