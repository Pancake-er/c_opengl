#ifndef RENDER_INCLUDED
#define RENDER_INCLUDED

#include <glad/glad.h>
#include <stdlib.h>
#include "texture.h"

struct RenderHandles {
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint vao;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;
    void *vertices;
    size_t vertices_size;
    size_t vertices_current_position;
    size_t max_index_count;
    GLuint64 *textures;
    size_t textures_current_position;
};
GLuint render_create_and_use_vao();
struct RenderHandles render_init(const char *shader_path, int window_width,
    int window_height);
void render_add_vertex(struct RenderHandles *render_handles, float x_position, 
    float y_position, float z_position, float x_texture_coord, 
    float y_texture_coord, int texture_index);
void render_add_quad(struct RenderHandles *render_handles, float x_position, 
    float y_position, float half_width, float half_height,
    float x_texture_coordinate, float y_texture_coordinate, float texture_width,
    float texture_height, struct Texture texture);
void render_flush(struct RenderHandles *render_handles);
void render_free(struct RenderHandles *render_handles);

#endif