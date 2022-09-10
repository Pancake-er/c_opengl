#include <render.h>
#include <glad/glad.h>
#include <matrix4f.h>
#include <shader.h>
#include <util.h>
#include <texture.h>
#include <string.h>
GLuint render_create_and_use_vao() {

	GLuint vao;
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    return vao;
}
struct RenderHandles render_init(const char *shader_path, 
    size_t max_quad_count, int window_width, int window_height) {

    struct RenderHandles render_handles;

    size_t max_index_count = max_quad_count * 6;
    render_handles.max_index_count = max_index_count;
    render_handles.vertices_size = (max_quad_count * 4) * ((sizeof(float) * 5) 
        + sizeof(GLuint64));
    render_handles.vertices = malloc_or_die(render_handles.vertices_size);
    render_handles.vertices_current_position = 0;

    render_handles.vao = render_create_and_use_vao();

    struct ShaderStrings shader_strings = shader_parse_file(shader_path);
    render_handles.vertex_shader = shader_compile_shader(VERTEX, 
        shader_strings.vertex);
    render_handles.fragment_shader = shader_compile_shader(FRAGMENT, 
        shader_strings.fragment);
    render_handles.program = shader_create_program(render_handles.vertex_shader,
        render_handles.fragment_shader);
    shader_use_program(render_handles.program);

    struct Matrix4f projection = matrix4f_initialize();
    matrix4f_orthographic(&projection, 0.0f, (float)window_width, 
        (float)window_height, 0.0f, 0.0f, 10.0f);
    float matrix_array[16];
    matrix4f_place_into_array_with_a_size_of_sixteen_floats(projection, 
        matrix_array);
    glUniformMatrix4fv(0, 1, GL_FALSE, matrix_array);

    glGenBuffers(1, &render_handles.vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, render_handles.vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, render_handles.vertices_size, 
        NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ((sizeof(float) * 5) 
        + sizeof(GLuint64)), (const void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, ((sizeof(float) * 5) 
        + sizeof(GLuint64)), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribLPointer(2, 1, GL_UNSIGNED_INT64_ARB, ((sizeof(float) * 5) 
        + sizeof(GLuint64)), (const void *)(5 * sizeof(float)));

    int *indices = calloc_or_die(max_index_count, sizeof(int));
    int index_offset = 0;
    for (int i = 0; i < max_index_count; i += 6) {
        indices[i] = 0 + index_offset;
        indices[i + 1] = 1 + index_offset;
        indices[i + 2] = 2 + index_offset;
        indices[i + 3] = 2 + index_offset;
        indices[i + 4] = 3 + index_offset;
        indices[i + 5] = 0 + index_offset;
        
        index_offset += 4;
    }

    glGenBuffers(1, &render_handles.index_buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_handles.index_buffer);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_index_count * sizeof(int), 
        indices, GL_STATIC_DRAW);
    
    free(indices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return render_handles;
}
void render_add_vertex(struct RenderHandles *render_handles, float x_position, 
    float y_position, float z_position, float x_texture_coordinate, float y_texture_coordinate, 
    GLuint64 texture_handle) {
    memcpy(render_handles->vertices + render_handles->vertices_current_position,
        &x_position, sizeof(float));
    render_handles->vertices_current_position += sizeof(float);
    memcpy(render_handles->vertices + render_handles->vertices_current_position,
        &y_position, sizeof(float));
    render_handles->vertices_current_position += sizeof(float);
    memcpy(render_handles->vertices + render_handles->vertices_current_position,
        &z_position, sizeof(float));
    render_handles->vertices_current_position += sizeof(float);
    memcpy(render_handles->vertices + render_handles->vertices_current_position,
        &x_texture_coordinate, sizeof(float));
    render_handles->vertices_current_position += sizeof(float);
    memcpy(render_handles->vertices + render_handles->vertices_current_position,
        &y_texture_coordinate, sizeof(float));
    render_handles->vertices_current_position += sizeof(float);
    memcpy(render_handles->vertices + render_handles->vertices_current_position,
        &texture_handle, sizeof(GLuint64));
    render_handles->vertices_current_position += sizeof(GLuint64);
}
void render_add_quad(struct RenderHandles *render_handles, float x_position, 
    float y_position, float width, float height, float x_texture_coordinate, 
    float y_texture_coordinate, float texture_width, float texture_height, 
    struct Texture texture) {
        
    render_add_vertex(render_handles, x_position, y_position, 0.0f,
        x_texture_coordinate, y_texture_coordinate, texture.bindless_handle);
    render_add_vertex(render_handles, x_position + width, y_position, 0.0f, 
        x_texture_coordinate + texture_width, y_texture_coordinate, 
        texture.bindless_handle);
    render_add_vertex(render_handles, x_position + width, y_position + height, 
        0.0f, x_texture_coordinate + texture_width, y_texture_coordinate 
        + texture_height, texture.bindless_handle);
    render_add_vertex(render_handles, x_position, y_position + height, 0.0f,
        x_texture_coordinate, y_texture_coordinate + texture_height, 
        texture.bindless_handle);

    if (render_handles->vertices_current_position
        == render_handles->vertices_size) {
        
        render_flush(render_handles);
    };
}
void render_flush(struct RenderHandles *render_handles) {
    if (render_handles->vertices_current_position != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, render_handles->vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_handles->index_buffer);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, 
            render_handles->vertices_current_position, render_handles->vertices);

        render_handles->vertices_current_position = 0;

        glDrawElements(GL_TRIANGLES, render_handles->max_index_count, 
            GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
void render_clean_up(struct RenderHandles *render_handles) {
    free(render_handles->vertices);
}