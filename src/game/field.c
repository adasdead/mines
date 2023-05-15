#include "field.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game/window.h"

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

static void shuffle(int *array, size_t size)
{
    size_t i, j;
    int tmp;

    if (array == NULL || size <= 1) return;

    srand(time(NULL));

    for (i = size - 1; i > 0; i--) {
        j = rand() % (i + 1);
        
        tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

field_t field_create(uint width, uint height, uint mines)
{
    field_t field = malloc(sizeof *field);

    if (field == NULL)
        logger_fatal("Failed to allocate memory for field");

    field->height = height;
    field->width = width;
    field->mines = mines;
    field->size = width * height;

    field->cells = calloc(field->size, sizeof *(field->cells));

    renderer_titles_initialize(&field->renderer, width, height);

    logger_info("Field(%ux%u | %u mines) created!", width, height,
                mines);

    return field;
}

cell_t field_cell(field_t field, int x, int y)
{
    if (field != NULL && field_is_within(field, x, y))
        return field->cells + (field->width * y + x);

    return NULL;
}

void field_generate(field_t field, uint x, uint y)
{
    size_t free_cells = field->width * field->height - 1;
    uint *free_cells_array = NULL;
    size_t i, j, k;
    cell_t cell;

    free_cells_array = malloc(free_cells * sizeof(uint));

    if (free_cells_array == NULL) {
        logger_fatal("malloc failed (field_generate)");
        return;
    }

    for (i = 0, k = 0; i < field->width; ++i) {
        for (j = 0; j < field->height; ++j) {
            if (i == x && j == y) {
                continue;
            }

            free_cells_array[k++] = i + j * field->width;
        }
    }

    shuffle(free_cells_array, free_cells);

    for (i = 0; i < field->mines; ++i) {
        j = free_cells_array[i];
        cell = field_cell(field, j % field->width, j / field->width);
        cell->type = CELL_TYPE_BOMB;
    }

    free(free_cells_array);

    for (x = 0; x < field->width; ++x) {
        for (y = 0; y < field->height; ++y) {
            cell = field_cell(field, x, y);

            if (cell->type != CELL_TYPE_BOMB) {
                cell->type = field_adjacent_mines(field, x, y);
            }
        }
    }

    logger_info("Field(%ux%u | %u mines) generated",
                field->width, field->height, field->mines);
}

size_t field_adjacent_mines(field_t field, uint x, uint y)
{
    cell_t cell;
    size_t mines = 0;
    int i, j; /* offsets */

    for (i = -1; i <= 1; ++i) {
        for (j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            
            cell = field_cell(field, x + i, y + j);

            if (cell != NULL && cell->type == CELL_TYPE_BOMB)
                mines += 1;
        }
    }

    return mines;
}

void field_render(field_t field, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_FIELD);
    int cur_x = window_instance()->cursor.x;
    int cur_y = window_instance()->cursor.y;

    if (field == NULL || projection == NULL) return;

    window_normalize_pos(&cur_x, &cur_y);
    field_shift_pos(&cur_x, &cur_y);
    
    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2i(shader, "u_field_size", field->width,
                                                  field->height);
    shader_set_uniform_2i(shader, "u_field_pos", FIELD_LX, FIELD_LY);
    shader_set_uniform_2i(shader, "u_mouse_pos", cur_x, cur_y);

    texture_bind(resources_texture_atlas());

    renderer_titles_draw(&field->renderer, (byte*) field->cells,
                         field->size);
}

void field_clear(field_t field)
{
    if (field != NULL)
        memset(field->cells, CELL_STATE_CLOSED, field->size);
}

void field_free(field_t field)
{
    if (field != NULL) {
        renderer_free(&field->renderer);
        free(field->cells);
        free(field);
    }
}