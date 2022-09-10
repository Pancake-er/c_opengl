#include <shader.h>
#include <glad/glad.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util.h>

struct ShaderStrings shader_parse_file(const char *filename) {
    FILE *in_file = fopen(filename, "r");
    if (in_file == NULL) {
        perror("Failed to open shader file");
        exit(EXIT_FAILURE);
    }

    // Gets the string length of the file.
    fseek(in_file, 0L, SEEK_END);
    size_t file_length = ftell(in_file); 
    rewind(in_file);

    /* Allocates each string the file length + 1. The max we could read, to
    prevent buffer overflow. Faster than updating the buffer each time. */
    struct ShaderReaderStrings {
        char *shared;
        char *vertex;
        char *fragment;
    } strings;
    strings.shared = malloc_or_die((file_length + 1) 
        * sizeof(char));
    strings.vertex = malloc_or_die((file_length + 1) 
        * sizeof(char));
    strings.fragment = malloc_or_die((file_length + 1) 
        * sizeof(char));
    strings.shared[0] = '\0';
    strings.vertex[0] = '\0';
    strings.fragment[0] = '\0';
    // Used to track string lengths for later memory optimization reallocation.
    struct ShaderReaderStringLengths {
        size_t shared;
        size_t vertex;
        size_t fragment;
    } string_lengths;
    string_lengths.shared = 0;
    string_lengths.vertex = 0;
    string_lengths.fragment = 0;

    size_t buffer_size = 128;
    char *line = malloc_or_die(buffer_size);

    size_t line_length = 0;

    int shader_type = SHARED;

    while(!feof(in_file)) {
        line_length = get_line_delim(&line, &buffer_size, '\n', in_file);
        // Checks and ignores unnecessary newlines.
        if (line_length == 1 && strchr(line, '\n')) {
            continue;
        }
        if (strstr(line, "#shared") != NULL) {
            shader_type = SHARED;
            continue;
        }
        if (strstr(line, "#vertex shader") != NULL) {
            shader_type = VERTEX;
            continue;
        }
        if (strstr(line, "#fragment shader") != NULL) {
            shader_type = FRAGMENT;
            continue;
        }
        if (shader_type == SHARED) {
            strcat(strings.shared, line);
            string_lengths.shared += line_length;
        }
        if (shader_type == VERTEX) {
            strcat(strings.vertex, line);
            string_lengths.vertex += line_length;
        }
        if (shader_type == FRAGMENT) {
            strcat(strings.fragment, line);
            string_lengths.fragment += line_length;
        }
    };

    /* For the returned strings only the minimal memory required is allocated,
    figured out above for string_lengths. (Because strlen() is expensive.) */
    struct ShaderStrings final_strings;
    final_strings.vertex = malloc_or_die(string_lengths
        .shared + string_lengths.vertex + 1);
    final_strings.fragment = malloc_or_die(string_lengths
        .shared + string_lengths.fragment + 1);
    final_strings.vertex[0] = '\0';
    final_strings.fragment[0] = '\0';
    // Adds the shared string first and then the other one for each char *.
    strcat(final_strings.vertex, strings.shared);
    strcat(final_strings.vertex, strings.vertex);
    strcat(final_strings.fragment, strings.shared);
    strcat(final_strings.fragment, strings.fragment);

    free(line);
    line = NULL;
    free(strings.shared);
    strings.shared = NULL;
    free(strings.vertex);
    strings.vertex = NULL;
    free(strings.fragment);
    strings.fragment = NULL;
    fclose(in_file); 
    in_file = NULL;
    
    return final_strings;
}
GLuint shader_compile_shader(enum ShaderType TYPE, const char *source) {
    GLuint shader;   
    if (TYPE == VERTEX) {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    if (TYPE == FRAGMENT) {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        char *error_log = malloc_or_die(max_length);
        glGetShaderInfoLog(shader, max_length, &max_length, 
            &error_log[0]);

        printf(error_log);

        free(error_log);
        error_log = NULL;
        exit(EXIT_FAILURE);
    }

    return shader;
}
GLuint shader_create_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        char *error_log = malloc_or_die(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, 
            &error_log[0]);
 
        printf(error_log);

        free(error_log);
        error_log = NULL;
        exit(EXIT_FAILURE);
    }

    glValidateProgram(program);
    
    return program;
}
void shader_use_program(GLuint program) {
    glUseProgram(program);
}
void shader_delete_program(GLuint program) {
    glDeleteProgram(program);   
}
void shader_delete_shader(GLuint shader) {
    glDeleteShader(shader);
}