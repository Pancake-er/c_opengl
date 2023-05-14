#include <render/texture_atlas.h>
#include <glad/glad.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

struct TextureAtlas texture_atlas_init(const char *path) {
    struct TextureAtlas texture;

    int channels;
    GLvoid *data = stbi_load(path, &texture.width, &texture.height, 
        &channels, 4);
        
    if (data == NULL) {
        printf("Error loading image");
        exit(EXIT_FAILURE);
    }
    
    glGenTextures(1, &texture.id);

    glActiveTexture(GL_TEXTURE0);
    // The location must be the same as u_textureAtlas' in the shader.
    glUniform1i(1, 0);

    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    return texture;
}