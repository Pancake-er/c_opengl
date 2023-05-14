#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <render/render.h>
#include <render/texture_atlas.h>
#include <stdio.h>
#include <stdbool.h>
#include <render/matrix4f.h>

int main(void)
{
    GLFWwindow* window;
    
    // Initialize and setup GLFW and a window.
    if (!glfwInit()) {
        printf("Failed to initialize glfw");
        return 1;
    }
    window = glfwCreateWindow(480, 480, "OpenGL in C", NULL, NULL);
    if (!window) {
        printf("Failed to initialize window");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize glad");
        return 1;
    }
    // Tell OpenGL the window size.
    glViewport(0, 0, 480, 480);

    struct RenderHandles render_handles = render_init("./shaders/shader.glsl", 
        480, 480);
        
    struct TextureAtlas texture_atlas = texture_atlas_init("./res/image.png");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        matrix4f_translate(render_handles.camera_matrix, 0.001f, 0.0f, 0.0f);

        for (int i = 0; i < 101; i++) {
            render_add_quad(&render_handles, 100, 100, 50, 50, 0.0f, 0.0f, 1.0f,
                1.0f);
        }

        render_flush(&render_handles);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    render_free(&render_handles);
    glfwTerminate();
    return 0;
}