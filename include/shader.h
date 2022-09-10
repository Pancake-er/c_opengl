#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED

#include <glad/glad.h>

struct ShaderStrings {
    char *vertex;
    char *fragment;
};
enum ShaderType {
    SHARED = 0,
    VERTEX = 1,
    FRAGMENT = 2,
};
struct ShaderStrings shader_parse_file(const char *filename);
GLuint shader_compile_shader(enum ShaderType TYPE, const char *source);
GLuint shader_create_program(GLuint vertex_shader, GLuint fragment_shader);
void shader_use_program(GLuint program);
void shader_delete_program(GLuint program);

#endif