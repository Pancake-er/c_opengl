#shared
#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : require

#vertex shader
layout(location = 0) in vec3 a_vertices;
layout(location = 1) in vec2 a_textureCoords;

layout(location = 0) uniform mat4 u_viewProjection;

layout(location = 0) out vec2 v_textureCoords;

void main() {
    v_textureCoords = a_textureCoords;
    gl_Position = u_viewProjection * vec4(a_vertices.x, a_vertices.y, a_vertices.z, 1);
}

#fragment shader
layout(location = 0) in vec2 v_textureCoords;

layout(location = 1) uniform sampler2D u_textureAtlas;

layout(location = 0) out vec4 o_color;

void main() {
    o_color = texture(u_textureAtlas, v_textureCoords);
}