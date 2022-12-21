#include <render/render.h>
#include <glad/glad.h>
#include <render/matrix4f.h>
#include <render/shader.h>
#include <render/mem_util.h>
#include <render/texture.h>
#include <string.h>
GLuint render_create_and_use_vao() {

	GLuint vao;
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    return vao;
}
struct RenderHandles render_init(const char *shader_path, int window_width, 
    int window_height) {

    /* max_quad_count must be set to the same value as the array size of the 
    u_textures[] uniform is in the fragment shader. */
    const size_t max_quad_count = 100;
    struct RenderHandles render_handles;

    render_handles.max_index_count = max_quad_count * 6;
    render_handles.vertices_size = (max_quad_count * 4) * ((sizeof(float) * 5) 
        + sizeof(int));
    render_handles.vertices_current_position = 0;
    render_handles.vertices = malloc_or_die(render_handles.vertices_size);

    render_handles.textures_current_position = 0;
    render_handles.textures = malloc_or_die(max_quad_count * sizeof(GLuint64));

    render_handles.vao = render_create_and_use_vao();

    struct ShaderStrings shader_strings = shader_parse_file(shader_path);
    render_handles.vertex_shader = shader_compile_shader(VERTEX, 
        shader_strings.vertex);
    render_handles.fragment_shader = shader_compile_shader(FRAGMENT, 
        shader_strings.fragment);
    render_handles.program = shader_create_program(render_handles.vertex_shader,
        render_handles.fragment_shader);
    shader_use_program(render_handles.program);

    matrix4f_init_array16f(render_handles.camera_matrix);
    matrix4f_orthographic(render_handles.camera_matrix, 0.0f, 
        (float)window_width, (float)window_height, 0.0f, 0.0f, 10.0f);
    glUniformMatrix4fv(0, 1, GL_FALSE, render_handles.camera_matrix);

    glGenBuffers(1, &render_handles.vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, render_handles.vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, render_handles.vertices_size, 
        NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ((sizeof(float) * 5) 
        + sizeof(int)), (const void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, ((sizeof(float) * 5) 
        + sizeof(int)), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, ((sizeof(float) * 5) 
        + sizeof(int)), (const void *)(5 * sizeof(float)));

    int *indices = calloc_or_die(render_handles.max_index_count, sizeof(int));
    int index_offset = 0;
    for (int i = 0; i < render_handles.max_index_count; i += 6) {
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

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_handles.max_index_count
        * sizeof(int), indices, GL_STATIC_DRAW);
    
    free(indices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return render_handles;
}
void render_add_vertex(struct RenderHandles *render_handles, float x_position, 
    float y_position, float z_position, float x_texture_coordinate, 
    float y_texture_coordinate, int texture_index) {
    // x_pos y_pos z_pos x_tex_pos y _tex_pos tex_index
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
        &texture_index, sizeof(int));
    render_handles->vertices_current_position += sizeof(int);
}
void render_add_quad(struct RenderHandles *render_handles, float x_position, 
    float y_position, float radius_x, float radius_y, 
    float x_texture_coordinate, float y_texture_coordinate, float texture_width, 
    float texture_height, struct Texture texture) {
    
    /* Position is at center with radius being half the width and height.
    Texture coordinates are from the top left corner, texture width and height 
    specifying how far from the coordinates down and to the right of the texture
    you want to show, with 0 0 being top left of the texture, and 1, 1 meaning 
    bottom right. */

    /* Gets set to a uniform in the fragment shader, and then indexed by a 
    vertex attribute */
    render_handles->textures[render_handles->textures_current_position] 
        = texture.bindless_handle;

    render_add_vertex(render_handles, x_position - radius_x, y_position 
        - radius_y, 0.0f, x_texture_coordinate, y_texture_coordinate, 
        render_handles->textures_current_position);
    render_add_vertex(render_handles, x_position + radius_x, y_position 
        - radius_y, 0.0f, x_texture_coordinate + texture_width, 
        y_texture_coordinate, render_handles->textures_current_position);
    render_add_vertex(render_handles, x_position + radius_x, y_position 
        + radius_y, 0.0f, x_texture_coordinate + texture_width, 
        y_texture_coordinate + texture_height, 
        render_handles->textures_current_position);
    render_add_vertex(render_handles, x_position - radius_x, y_position 
        + radius_y, 0.0f, x_texture_coordinate, y_texture_coordinate 
        + texture_height, render_handles->textures_current_position);

    if (render_handles->vertices_current_position
        == render_handles->vertices_size) {
        
        render_flush(render_handles);
    }
    else {
        render_handles->textures_current_position += 1;
    }
}
void render_flush(struct RenderHandles *render_handles) {
    if (render_handles->vertices_current_position != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, render_handles->vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_handles->index_buffer);

        glUniformMatrix4fv(0, 1, GL_FALSE, render_handles->camera_matrix);
        glUniformHandleui64vARB(1, 100, 
            (const GLuint64 *)render_handles->textures);

        glBufferSubData(GL_ARRAY_BUFFER, 0, 
            render_handles->vertices_current_position, 
            render_handles->vertices);

        render_handles->vertices_current_position = 0;
        render_handles->textures_current_position = 0;

        glDrawElements(GL_TRIANGLES, render_handles->max_index_count, 
            GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
void render_free(struct RenderHandles *render_handles) {
    free(render_handles->vertices);
    render_handles->vertices = NULL;
    free(render_handles->textures);
    render_handles->textures = NULL;
}