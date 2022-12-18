#shared
#version 410
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_bindless_texture : require  

#vertex shader
layout(location = 0) in vec3 a_vertices;
layout(location = 1) in vec2 a_textureCoords;
layout(location = 2) in int a_textureIndex;

layout(location = 0) uniform mat4 u_viewProjection;

layout(location = 0) out vec2 v_textureCoords;
layout(location = 1) out int v_textureIndex;

void main() {
    v_textureCoords = a_textureCoords;
    v_textureIndex = a_textureIndex;
    gl_Position = u_viewProjection * vec4(a_vertices.x, a_vertices.y, a_vertices.z, 1);
}

#fragment shader
layout(location = 0) in vec2 v_textureCoords;
layout(location = 1) flat in int v_textureIndex;

layout(bindless_sampler) uniform;
layout(bindless_image) uniform;
layout(location = 1) uniform sampler2D u_textures[100];

layout(location = 0) out vec4 o_color;

void main() {
    o_color = texture(u_textures[v_textureIndex], v_textureCoords);
}