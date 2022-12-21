#ifndef TEXTURE_INCLUDED
#define TEXTURE_INCLUDED

#include <glad/glad.h>
#include <stdbool.h>

struct Texture {
    int width;
    int height;
    GLuint id;
    GLuint64 bindless_handle;
};
struct Texture texture_create(const char *path, bool smooth);

#endif