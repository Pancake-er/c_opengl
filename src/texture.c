#include <render/texture.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stdbool.h>

struct Texture texture_create(const char *path, bool smooth) {
    struct Texture texture;

    int channels;
    void *data = stbi_load(path, &texture.width, &texture.height, 
        &channels, 4);
        
    if (data == NULL) {
        printf("Error loading image. Are you sure the path given to " 
            "\"texture_create_texture\" is correct?\n");
        exit(EXIT_FAILURE);
    }
    
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    if (smooth) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    texture.bindless_handle = glGetTextureHandleARB(texture.id); 
    glMakeTextureHandleResidentARB(texture.bindless_handle);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture;
}