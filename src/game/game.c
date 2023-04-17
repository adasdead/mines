#include "game/game.h"

#include <time.h>

#include "game/window.h"
#include "game/field.h"
#include "game/smile.h"
#include "game/difficulty.h"
#include "game/counter.h"

#include "util/logger.h"

#include "definitions.h"

enum game_state {
    GAME_STATE_STARTED,
    GAME_STATE_IDLE,
    GAME_STATE_LOSE,
    GAME_STATE_WON
};

static enum game_state state = GAME_STATE_IDLE;

static game_difficulty_t cur_difficulty = NULL;

static usize opened_cells = 0;
static time_t start_time;

/* objects */
static field_t field = NULL;
static smile_t smile = NULL;
static counter_t mine_counter = NULL;
static counter_t time_counter = NULL;

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

        smile->state = SMILE_STATE_DEAD;
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

        smile->state = SMILE_STATE_COOL;
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

void game_new(void)
{
    u32 width = FIELD_LX + FIELD_RX;
    u32 height = FIELD_LY + FIELD_RY;
    
    field_free(field);

    field = field_create(cur_difficulty->field_width,
                         cur_difficulty->field_height,
                         cur_difficulty->mines_count);

    window_resize(width + field->width, height + field->height);

    smile_update_width(smile, field->width);
    smile->state = SMILE_STATE_DEFAULT;

    mine_counter->value = field->mines;
    time_counter->value = 0;

    counter_update_x(time_counter, field->width - 2);

    state = GAME_STATE_IDLE;
    opened_cells = 0;
}

void game_init(void)
{
    smile = smile_create(SMILE_STATE_DEFAULT);
    mine_counter = counter_create();
    counter_update_x(mine_counter, 1);
    time_counter = counter_create();
    game_toggle_difficulty();
}

void game_loop(void)
{
    cell_t cell;
    double mouse_x, mouse_y;
    mat4 projection = window_projection();

    glfwGetCursorPos(window_glfw(), &mouse_x, &mouse_y);

    if (state == GAME_STATE_STARTED) {
        time_counter->value = time(NULL) - start_time;
    }

    field_render(field, projection, mouse_x, mouse_y);
    smile_render(smile, projection);
    counter_render(mine_counter, projection);
    counter_render(time_counter, projection);
}

void game_on_left_click(i32 x, i32 y, bool press)
{
    smile_mouse(smile, x, y, press);

    field_normalize_mouse_pos(field, x, y);

    if (x < 0 || y < 0) return;

    if (press && state != GAME_STATE_LOSE) {
        if (state == GAME_STATE_IDLE) {
            field_generate(field, x, y);
            start_time = time(NULL);
            state = GAME_STATE_STARTED;
        }

        open_cell(x, y);
    }
}

void game_on_right_click(i32 x, i32 y, bool press)
{
    cell_t cell;

    if (state == GAME_STATE_LOSE) return;
    if (state == GAME_STATE_WON) return;

    field_normalize_mouse_pos(field, x, y);

    if (x < 0 || y < 0) return;

    if (press) {
        cell = field_cell(field, x, y);

        switch (cell->state) {
        case CELL_STATE_CLOSED:
            cell->state = CELL_STATE_FLAGGED;
            mine_counter->value -= 1;
            return;
        case CELL_STATE_FLAGGED:
            cell->state = CELL_STATE_QUESTIONED;
            mine_counter->value += 1;
            return;
        case CELL_STATE_QUESTIONED:
            cell->state = CELL_STATE_CLOSED;
            return;
        }
    }
}

void game_toggle_difficulty(void)
{
    usize difficulty = 0;
    usize total = sizeof game_difficulties / sizeof *game_difficulties;

    if (cur_difficulty != NULL) {
        if ((cur_difficulty->id + 1) != total) {
            difficulty = cur_difficulty->id + 1;
        }
    }

    cur_difficulty = &game_difficulties[difficulty];
    logger_info("Difficulty changed to %s", cur_difficulty->name);
    game_new();
}

void game_free(void)
{
    field_free(field);
    smile_free(smile);
    counter_free(mine_counter);
    counter_free(time_counter);
}