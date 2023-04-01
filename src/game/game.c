#include "game/game.h"

#include "game/window.h"
#include "game/field.h"

#include "util/logger.h"

#include "definitions.h"

enum difficulty {
    DIF_BEGGINER,
    DIF_INTERMEDIATE,
    DIF_EXPERT,
    DIF_TOTAL,
};

enum game_state {
    GAME_STATE_STARTED,
    GAME_STATE_IDLE,
    GAME_STATE_LOSE,
    GAME_STATE_WON
};

struct game_difficulty_info {
    enum difficulty dif;
    u32 field_width, field_height;
    usize mines_count;

} *cur_difficulty = NULL;

static struct game_difficulty_info game_difficulties[] = {
    { DIF_BEGGINER,       9,      9,    10,},
    { DIF_INTERMEDIATE,  16,     16,    40 },
    { DIF_EXPERT,        30,     16,    99 }
};

static enum game_state state = GAME_STATE_IDLE;
static usize opened_cells = 0;
static field_t field = NULL;

static void check_lose(cell_t cell)
{
    i32 x, y;

    if (cell->type == CELL_TYPE_BOMB) {
        cell->state = CELL_STATE_OPENED;
        cell->type = CELL_TYPE_BOMB_E;

        for (x = 0; x < field->width; x++) {
            for (y = 0; y < field->height; y++) {
                cell = field_cell(field, x, y);

                if (cell->state == CELL_STATE_FLAGGED &&
                    cell->type != CELL_TYPE_BOMB) {
                    
                    cell->state = CELL_STATE_OPENED;
                    cell->type = CELL_TYPE_NO_BOMB;
                    continue;
                }

                cell->state = CELL_STATE_OPENED;
            }
        }

        state = GAME_STATE_LOSE;

        logger_info("Lose");
    }
}

static void check_won(void)
{
    i32 x, y;
    cell_t cell;

    if (state == GAME_STATE_LOSE) return;

    if (field->mines == (field->size - opened_cells)) {
        for (x = 0; x < field->width; x++) {
            for (y = 0; y < field->height; y++) {
                cell = field_cell(field, x, y);

                if (cell->state == CELL_STATE_CLOSED) {
                    cell->state = CELL_STATE_FLAGGED;
                }
            }
        }

        state = GAME_STATE_WON;

        logger_info("Won");
    }
}

static void open_cell(i32 x, i32 y)
{
    i32 i, j;
    cell_t cell = field_cell(field, x, y);

    if (cell == NULL) return;
    if (cell->state == CELL_STATE_OPENED) return;
    if (cell->state == CELL_STATE_FLAGGED) return;
    if (state == GAME_STATE_LOSE) return;
    if (state == GAME_STATE_WON) return;

    cell->state = CELL_STATE_OPENED;
    opened_cells++;

    if (field_adjacent_mines(field, x, y) == 0) {
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;
                open_cell(x + i, y + j);
            }
        }
    }

    check_lose(cell);
    check_won();
}

static void new_game(void)
{
    u32 width = FIELD_LX + FIELD_RX;
    u32 height = FIELD_LY + FIELD_RY;
    
    field_free(field);

    field = field_create(cur_difficulty->field_width,
                         cur_difficulty->field_height,
                         cur_difficulty->mines_count);

    window_resize(width + field->width, height + field->height);

    state = GAME_STATE_IDLE;
    opened_cells = 0;
}

void game_init(void)
{
    game_toggle_difficulty();
}

void game_loop(void)
{
    cell_t cell;
    i32 mouse_x, mouse_y;
    mat4 projection = window_projection();

    window_mouse_pos(&mouse_x, &mouse_y);

    field_render(field, projection, mouse_x, mouse_y);
}

void game_on_left_click(i32 x, i32 y, bool press)
{
    field_normalize_mouse_pos(field, x, y);

    if (x < 0 || y < 0) return;

    if (press && state != GAME_STATE_LOSE) {
        if (state == GAME_STATE_IDLE) {
            field_generate(field, x, y);
            state = GAME_STATE_STARTED;
        }

        open_cell(x, y);
    }
}

void game_on_right_click(i32 x, i32 y, bool press)
{
    cell_t cell;

    field_normalize_mouse_pos(field, x, y);

    if (press && state == GAME_STATE_STARTED) {
        cell = field_cell(field, x, y);

        switch (cell->state) {
        case CELL_STATE_CLOSED:
            cell->state = CELL_STATE_FLAGGED;
            return;
        case CELL_STATE_FLAGGED:
            cell->state = CELL_STATE_QUESTIONED;
            return;
        case CELL_STATE_QUESTIONED:
            cell->state = CELL_STATE_CLOSED;
            return;
        }
    }
}

void game_toggle_difficulty(void)
{
    enum difficulty difficulty = DIF_BEGGINER;

    static const char *difficulty_string[] = {
        "BEGGINER", "INTERMEDIATE", "EXPERT"
    };

    if (cur_difficulty != NULL) {
        if ((cur_difficulty->dif + 1) == DIF_TOTAL) {
            difficulty = DIF_BEGGINER;
        }
        else {
            difficulty = cur_difficulty->dif + 1;
        }

        logger_info("Difficulty changed to %s",
                    difficulty_string[difficulty]);
    }

    cur_difficulty = &game_difficulties[difficulty];

    new_game();
}

void game_free(void)
{
    field_free(field);
}