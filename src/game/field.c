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
    int tmp;
    size_t i, j;

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

    if (field == NULL) {
        logger_fatal("Failed to allocate memory for field");
    }

    field->height = height;
    field->width = width;
    field->mines = mines;
    field->size = width * height;
    field->cells = calloc(width * height, sizeof *(field->cells));

    /*** Gen OpenGL buffers ***/

    glGenBuffers(1, &field->render.VBO);
    glGenVertexArrays(1, &field->render.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, field->render.VBO);
    glBufferData(GL_ARRAY_BUFFER, width * height, NULL, GL_STATIC_DRAW);

    glBindVertexArray(field->render.VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_BYTE, sizeof(uint8_t), NULL);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);

    logger_info("Field(%ux%u | %u mines) created!", width, height, mines);

    return field;
}

bool field_is_include(const field_t field, int x, int y)
{
    return (x < field->width) && (y < field->height) &&
           (x >= 0)           && (y >= 0);
}

cell_t field_cell(field_t field, int x, int y)
{
    if (field != NULL && x >= 0 && x < field->width &&
                         y >= 0 && y < field->height) {

        return field->cells + (field->width * y + x);
    }

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
    int off_x, off_y;

    for (off_x = -1; off_x <= 1; ++off_x) {
        for (off_y = -1; off_y <= 1; ++off_y) {
            if (off_x == 0 && off_y == 0) {
                continue;
            }
            
            cell = field_cell(field, x + off_x, y + off_y);

            if (cell != NULL) {
                mines += (cell->type == CELL_TYPE_BOMB);
            }
        }
    }

    return mines;
}

void field_render(field_t field, mat4 projection)
{
    window_t window = window_instance();
    shader_t shader = resources_shader(RS_SHADER_FIELD);
    int cur_x = window->cursor.x, cur_y = window->cursor.y;

    if (field == NULL || projection == NULL) return;

    field_normalize_pos(&cur_x, &cur_y);
    
    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2i(shader, "u_field_size", field->width,
                                                  field->height);
    shader_set_uniform_2i(shader, "u_field_pos", FIELD_LX, FIELD_LY);
    shader_set_uniform_2i(shader, "u_mouse_pos", cur_x, cur_y);

    texture_bind(resources_texture_atlas());

    glBindBuffer(GL_ARRAY_BUFFER, field->render.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, field->size * sizeof(GLubyte),
                    field->cells);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(field->render.VAO);
    glDrawArrays(GL_POINTS, 0, field->size);
    glBindVertexArray(GL_NONE);
}

void field_clear(field_t field)
{
    if (field != NULL) {
        memset(field->cells, CELL_STATE_CLOSED, field->size);
    }
}

void field_free(field_t field)
{
    if (field != NULL) {
        OPENGL_RENDER_FREE(field);
        free(field->cells);
        free(field);
    }
}