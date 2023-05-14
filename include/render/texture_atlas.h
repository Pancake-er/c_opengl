#ifndef TEXTURE_ATLAS_INCLUDED
#define TEXTURE_ATLAS_INCLUDED

#include <glad/glad.h>
#include <stdbool.h>

struct TextureAtlas {
    int width;
    int height;
    GLuint id;
};
struct TextureAtlas texture_atlas_init(const char *path);

#endif