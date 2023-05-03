#include "game/game.h"

#include <time.h>

#include "game/window.h"
#include "game/field.h"
#include "game/smile.h"
#include "game/difficulty.h"
#include "game/counter.h"

#include "util/logger.h"

#include "definitions.h"

#define GAME_IS_OVER                                                        \
    ((state == GAME_STATE_LOSE) || (state == GAME_STATE_WON))

static time_t start_time;
static size_t opened_cells = 0;

static uint current_difficulty = 0;

/* objects */
static field_t field = NULL;
static smile_t smile = NULL;
static counter_t mine_counter = NULL;
static counter_t time_counter = NULL;

enum game_state {
    GAME_STATE_STARTED, GAME_STATE_IDLE, GAME_STATE_LOSE, GAME_STATE_WON
} static state = GAME_STATE_IDLE;

/****************************************************************************/

static void check_lose(cell_t cell)
{
    int x, y;

    if (cell->type == CELL_TYPE_BOMB) {
        cell->state = CELL_STATE_OPENED;
        cell->type = CELL_TYPE_BOMB_E;

        for (x = 0; x < field->width; ++x) {
            for (y = 0; y < field->height; ++y) {
                cell = field_cell(field, x, y);

                if (cell->state == CELL_STATE_FLAGGED) {
                    if (cell->type != CELL_TYPE_BOMB) {
                        cell->state = CELL_STATE_OPENED;
                        cell->type = CELL_TYPE_NO_BOMB;
                        continue;
                    }

                    if (cell->type == CELL_TYPE_BOMB) {
                        continue;
                    }
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
    int x, y;
    cell_t cell;

    if (state == GAME_STATE_LOSE) return;

    if (field->mines == (field->size - opened_cells)) {
        for (x = 0; x < field->width; ++x) {
            for (y = 0; y < field->height; ++y) {
                cell = field_cell(field, x, y);

                if (cell->state == CELL_STATE_CLOSED) {
                    cell->state = CELL_STATE_FLAGGED;
                }
            }
        }

        smile->state = SMILE_STATE_COOL;
        state = GAME_STATE_WON;
        mine_counter->value = 0;

        logger_info("Won");
    }
}

static void open_cell(int x, int y)
{
    int i, j;
    cell_t cell = field_cell(field, x, y);

    if (cell != NULL) {
        if (cell->state == CELL_STATE_OPENED) return;
        if (cell->state == CELL_STATE_FLAGGED) return;
        if (GAME_IS_OVER) return;

        cell->state = CELL_STATE_OPENED;
        opened_cells++;

        if (field_adjacent_mines(field, x, y) == 0) {
            for (i = -1; i <= 1; ++i) {
                for (j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) continue;
                    open_cell(x + i, y + j);
                }
            }
        }

        check_lose(cell);
        check_won();
    }
}

/****************************************************************************/

void game_new(void)
{
    float width, x_offset;

    field_free(field);

    field = field_create(DIFFICULTY(current_difficulty)->field_width,
                         DIFFICULTY(current_difficulty)->field_height,
                         DIFFICULTY(current_difficulty)->mines_count);
    
    width = FIELD_LX + FIELD_RX + field->width;

    window_normalized_resize(width, FIELD_LY + FIELD_RY + field->height);

    x_offset = COUNTER_NUMBERS * COUNTER_WIDTH + COUNTER_OFFSET_LRX;

    counter_update_x(time_counter, width - x_offset - FIELD_RX);

    smile_update_width(smile, width);

    smile->state = SMILE_STATE_DEFAULT;
    mine_counter->value = field->mines;
    time_counter->value = opened_cells = 0;
    state = GAME_STATE_IDLE;
}

void game_init(void)
{
    smile = smile_create(game_new);
    mine_counter = counter_create();
    counter_update_x(mine_counter, FIELD_LX + COUNTER_OFFSET_LRX);
    time_counter = counter_create();
    game_new();
}

void game_loop(void)
{
    window_t window = window_instance();

    if (state == GAME_STATE_STARTED) {
        time_counter->value = time(NULL) - start_time;
    }

    field_render(field, window->projection);
    smile_render(smile, window->projection);
    counter_render(mine_counter, window->projection);
    counter_render(time_counter, window->projection);
}

void game_on_left_click(int x, int y, bool press)
{
    smile_mouse(smile, x, y, press);

    if (!field_normalize_pos(field, &x, &y)) return;

    if (press && state != GAME_STATE_LOSE) {
        if (state == GAME_STATE_IDLE) {
            field_generate(field, x, y);
            start_time = time(NULL);
            state = GAME_STATE_STARTED;
        }

        open_cell(x, y);
    }
}

void game_on_right_click(int x, int y, bool press)
{
    cell_t cell;

    if (!GAME_IS_OVER) {
        if (!field_normalize_pos(field, &x, &y)) return;

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
}

void game_toggle_difficulty(void)
{
    const char *difficulty_name;

    current_difficulty = (current_difficulty + 1) % DIFFICULTY_TOTAL;
    
    difficulty_name = DIFFICULTY(current_difficulty)->name;

    logger_info("Difficulty changed to %s", difficulty_name);
    
    game_new();
}

void game_free(void)
{
    field_free(field);
    smile_free(smile);
    counter_free(mine_counter);
    counter_free(time_counter);
}