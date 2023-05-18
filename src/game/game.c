/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#include "game/game.h"

#include <time.h>
#include <stdio.h>

#include "game/window.h"
#include "game/field.h"
#include "game/smile.h"
#include "game/difficulty.h"
#include "game/counter.h"
#include "game/border.h"

#include "util/discord.h"
#include "util/logger.h"

#include "definitions.h"

enum game_state {
    GAME_STATE_IDLE,
    GAME_STATE_PLAYS,
    GAME_STATE_LOSE,
    GAME_STATE_WON
};

static struct {
    field_t field;
    smile_t smile;
    counter_t mine_counter;
    counter_t time_counter;
    border_t border;
} objects = { NULL };

static time_t start_time = 0;
static size_t opened_cells = 0;
static uint current_difficulty = 0;

static enum game_state state = GAME_STATE_IDLE;

static inline bool game_is_over(void)
{
    return (state == GAME_STATE_LOSE) || (state == GAME_STATE_WON);
}

static void update_game_activity(void)
{
#if DISCORD
    char difficulty[128] = {0};

    snprintf(difficulty, 127, "Difficulty: %s",
             difficulty_name(DIFFICULTY(current_difficulty)));

    switch (state)
    {
        case GAME_STATE_PLAYS:
            discord_update_activity("Plays", difficulty, time(NULL));
            break;
        
        case GAME_STATE_IDLE:
            discord_update_activity("Idle", NULL, 0);
            break;

        case GAME_STATE_WON:
            discord_update_activity("Won", difficulty, 0);
            break;

        case GAME_STATE_LOSE:
            discord_update_activity("Lose", difficulty, 0);
            break;
    }
#endif /* DISCORD */
}

static void game_lose(void)
{
    uint x, y;
    cell_t cell;

    objects.smile->state = SMILE_STATE_DEAD;

    for (x = 0; x < objects.field->width; ++x) {
        for (y = 0; y < objects.field->height; ++y) {
            cell = field_cell(objects.field, x, y);

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
    
    state = GAME_STATE_LOSE;

    update_game_activity();

    logger_info("Lose");
}

static void game_won(void)
{
    uint x, y;
    cell_t cell;

    for (x = 0; x < objects.field->width; ++x) {
        for (y = 0; y < objects.field->height; ++y) {
            cell = field_cell(objects.field, x, y);

            if (cell->state == CELL_STATE_CLOSED) {
                cell->state = CELL_STATE_FLAGGED;
            }
        }
    }

    objects.smile->state = SMILE_STATE_COOL;
    objects.mine_counter->value = 0;
    state = GAME_STATE_WON;

    update_game_activity();

    logger_info("Won");
}

static void game_start(uint safe_x, uint safe_y)
{
    field_generate(objects.field, safe_x, safe_y);
    start_time = time(NULL);
    state = GAME_STATE_PLAYS;

    update_game_activity();

    logger_info("Start");
}

static void open_cell(int x, int y)
{
    field_t field = objects.field;
    cell_t cell = field_cell(field, x, y);
    int i, j; /* offsets */

    if (cell != NULL) {
        if (cell->state == CELL_STATE_OPENED) return;
        if (cell->state == CELL_STATE_FLAGGED) return;
        if (game_is_over()) return;

        if (cell->type == CELL_TYPE_BOMB) {
            cell->state = CELL_STATE_OPENED;
            cell->type = CELL_TYPE_BOMB_E;
            game_lose();
            return;
        }

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

        if (field->mines == (field->size - opened_cells))
            game_won();
    }
}

static void update_objects(void)
{
    difficulty_t difficulty = DIFFICULTY(current_difficulty);
    uint width, height;

    static const float x_offset = COUNTER_DIGITS * COUNTER_WIDTH +
                                  COUNTER_OFFSET_LRX;

    field_free(objects.field);

    objects.field = field_create(difficulty->field_width,
                                 difficulty->field_height,
                                 difficulty->mines_count);
    
    width = FIELD_LX + objects.field->width + FIELD_RX;
    height = FIELD_LY + objects.field->height + FIELD_RY;

    window_resize_with_normalized_sizes(width, height);

    objects.time_counter->x = width - x_offset - FIELD_RX;
    counter_update_model_matrices(objects.time_counter);

    objects.smile->x = (width / 2.0f) - 1.0f;
    smile_update_model_matrix(objects.smile);

    border_update(objects.border, width, height);
}

void game_new(void)
{
    field_clear(objects.field);
    objects.smile->state = SMILE_STATE_DEFAULT;
    objects.mine_counter->value = objects.field->mines;
    objects.time_counter->value = 0;
    opened_cells = 0;

    state = GAME_STATE_IDLE;

    update_game_activity();

    logger_info("New game");
}

void game_initialize(void)
{
    objects.smile = smile_create(game_new);

    objects.mine_counter = counter_create();
    objects.mine_counter->x = FIELD_LX + COUNTER_OFFSET_LRX;
    counter_update_model_matrices(objects.mine_counter);

    objects.time_counter = counter_create();

    objects.border = border_create();

    update_objects();
    game_new();
}

void game_loop(void)
{
    window_t window = window_get_instance();

    if (state == GAME_STATE_PLAYS)
        objects.time_counter->value = (int) (time(NULL) - start_time);

    border_render(objects.border, window->projection);
    field_render(objects.field, window->projection);
    smile_render(objects.smile, window->projection);
    counter_render(objects.mine_counter, window->projection);
    counter_render(objects.time_counter, window->projection);
}

void game_on_left_click(int x, int y, bool press)
{
    smile_handle_mouse(objects.smile, x, y, press);

    window_normalize_pos(&x, &y);
    field_shift_pos(&x, &y);

    if (!field_is_within(objects.field, x, y)) return;

    if (press && state != GAME_STATE_LOSE) {
        if (state == GAME_STATE_IDLE)
            game_start(x, y);

        open_cell(x, y);
    }
}

void game_on_right_click(int x, int y, bool press)
{
    cell_t cell;

    if (game_is_over() == false) {
        window_normalize_pos(&x, &y);
        field_shift_pos(&x, &y);

        if (!field_is_within(objects.field, x, y))
            return;

        if (press) {
            cell = field_cell(objects.field, x, y);

            switch (cell->state) {
                case CELL_STATE_CLOSED:
                    cell->state = CELL_STATE_FLAGGED;
                    objects.mine_counter->value -= 1;
                    return;
                case CELL_STATE_FLAGGED:
                    cell->state = CELL_STATE_QUESTIONED;
                    objects.mine_counter->value += 1;
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
    uint current = current_difficulty;
    difficulty_t difficulty = DIFFICULTY((current + 1));
    current_difficulty = difficulty->id;

#if DEBUG
    logger_info("Difficulty changed to %s", difficulty_name(difficulty));
#endif /* DEBUG */
    
    update_objects();
    game_new();
}

void game_free(void)
{
    field_free(objects.field);
    smile_free(objects.smile);
    counter_free(objects.mine_counter);
    counter_free(objects.time_counter);
    border_free(objects.border);
}