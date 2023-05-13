#include "game/counter.h"

#include <string.h>

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

static struct
{
    bool                calculated;

    /*
     * Maximum counter value:
     * COUNTER_NUMBERS = 3 => n_max = 999
     */
    long long           n_max;
    
    /* Base divisor to get each digit of the counter */
    long long           p_base[COUNTER_NUMBERS];

} cached_values = {0};

static long long binary_pow(long long n, long long x)
{
    if (x == 0) return 1;
    if (x % 2 == 0) return binary_pow(n * n, x / 2);
    return n * binary_pow(n * n, x / 2);
}

static void calculate_cached_values(void)
{
    size_t i;
    long long base;

    if (!cached_values.calculated) {
        cached_values.n_max = binary_pow(10, COUNTER_NUMBERS) - 1;
        
        for (i = 0; i < COUNTER_NUMBERS; i++) {
            base = binary_pow(10, (COUNTER_NUMBERS - 1) - i);
            cached_values.p_base[i] = base;
        }

        cached_values.calculated = true;
    }
}

static int normalize_counter_value(int value)
{
    return MAX(MIN(cached_values.n_max, value), 0);
}

counter_t counter_create(void)
{
    size_t i;
    counter_t counter = malloc(sizeof *counter);

    if (counter == NULL) {
        logger_fatal("Failed to allocate memory for counter");
    }

    counter->models = calloc(COUNTER_NUMBERS, sizeof *counter->models);

    for (i = 0; i < COUNTER_NUMBERS; i++) {
        counter->models[i] = matrix4x4_allocate();
    }

    OPENGL_RENDER_GEN_BASIC_BUFFERS(counter);

    calculate_cached_values();
    counter_update_model_matrices(counter, COUNTER_DEFAULT);

    return counter;
}

void counter_update_model_matrices(counter_t counter, float start_x)
{
    size_t i;
    mat4 *cur = counter->models;
    
    /* ty - vertical position of counter digit */
    static const float ty = COUNTER_LY + COUNTER_OFFSET_LY;

    for (i = 0; i < COUNTER_NUMBERS; ++i, ++cur) {
        matrix4x4_identity(*cur);
        matrix4x4_scale(*cur, COUNTER_WIDTH, COUNTER_HEIGHT);
        matrix4x4_translate(*cur, start_x + i * COUNTER_WIDTH, ty);
    }
}

void counter_render(const counter_t counter, mat4 projection)
{
    int n = normalize_counter_value(counter->value), i;
    shader_t shader = resources_shader(RS_SHADER_COUNTER);
    long long base;

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(counter->render.VAO);

    for (i = 0; i < COUNTER_NUMBERS; ++i) {
        base = cached_values.p_base[i];

        shader_set_uniform_m4fv(shader, "u_model", counter->models[i]);

        /* 
         * >>> n / base % 10
         *
         * For example, if n = 123 and base = 10,
         * then u_number = 2 for the second position from the right.
         */
        shader_set_uniform_1i(shader, "u_number", n / base % 10);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    }

    glBindVertexArray(0);
}

void counter_free(counter_t counter)
{
    size_t i;

    if (counter != NULL) {
        OPENGL_RENDER_FREE(counter);

        for (i = 0; i < COUNTER_NUMBERS; ++i) {
            matrix4x4_free(counter->models[i]);
        }

        free(counter->models);
        free(counter);
    }
}