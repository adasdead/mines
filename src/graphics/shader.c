#include "graphics/shader.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "logger.h"

static char *read_strings_from_file(const char *file_path)
{
    FILE *fp = fopen(file_path, "rb");
    char *buffer = NULL;
    usize buffer_size;

    if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        buffer_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        buffer = malloc(buffer_size + 1);
        fread(buffer, buffer_size, 1, fp);
        fclose(fp);

        buffer[buffer_size] = 0;
    }
    else {
        logger_warn("IO Error [%s]: %s", file_path, strerror(errno));
    }

    return buffer;
}

static bool shader_check(GLuint id, const char *file_path)
{
    int status;
    char info_buffer[1024];

    if (glIsShader(id)) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            glGetShaderInfoLog(id, 1024, NULL, info_buffer);
            logger_warn("OpenGL shader error [%s]:\n %s", file_path,
                        info_buffer);
            return false;
        }
    }
    else if (glIsProgram(id)) {
        glGetProgramiv(id, GL_LINK_STATUS, &status);

        if (status == GL_FALSE) {
            glGetProgramInfoLog(id, 1024, NULL, info_buffer);
            logger_warn("OpenGL shader program error:\n %s", info_buffer);
            return false;
        }
    }

    return true;
}

static GLuint shader_create(const char *file_path, GLenum type)
{
    GLuint shader;
    char *source_buffer;

    if (file_path != NULL) {
        shader = glCreateShader(type);
        source_buffer = read_strings_from_file(file_path);

        glShaderSource(shader, 1, (const char**) &source_buffer, NULL);
        glCompileShader(shader);

        free(source_buffer);

        return shader_check(shader, file_path) ? shader : 0;
    }

    return GL_NONE;
}

shader_t shader_load_g(const char *vertex_path, const char *fragment_path,
                       const char *geometry_path)
{
    GLuint frag = shader_create(fragment_path, GL_FRAGMENT_SHADER);
    GLuint vert = shader_create(vertex_path, GL_VERTEX_SHADER);
    GLuint geom = shader_create(geometry_path, GL_GEOMETRY_SHADER);
    GLuint program = glCreateProgram();

    glAttachShader(program, frag);
    glAttachShader(program, vert);
    
    if (geom != GL_NONE) {
        glAttachShader(program, geom);
    }

    glLinkProgram(program);

    shader_check(program, "");

    glDeleteShader(geom);
    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

shader_t shader_load(const char *vertex_path, const char *fragment_path)
{
    return shader_load_g(vertex_path, fragment_path, NULL);
}

void shader_use(shader_t shader)
{
    glUseProgram(shader);
}

void shader_set_uniform_1i(shader_t shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void shader_set_uniform_2i(shader_t shader, const char *name, i32 x, i32 y)
{
    glUniform2i(glGetUniformLocation(shader, name), x, y);
}

void shader_set_uniform_m4fv(shader_t shader, const char *name, m4x4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE,
                       matrix);
}

void shader_free(shader_t shader)
{
    glDeleteProgram(shader);
}