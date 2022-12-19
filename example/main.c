#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <render/render.h>
#include <render/texture.h>
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    GLFWwindow* window;

    // Initialize and setup GLFW and a window.
    if (!glfwInit()) {
        return -1;
    }
    window = glfwCreateWindow(480, 480, "OpenGL in C", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    // Tell OpenGL the window size.
    glViewport(0, 0, 480, 480);

    // Needed for textures to work.
    glEnable(GL_TEXTURE_2D);
	// Needed for textures to have transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    struct RenderHandles render_handles = render_init("./shaders/shader.glsl", 
        480, 480);

    struct Texture texture = texture_create("./res/dog.jpg");

    // Main loop.
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < 101; i++) {
            render_add_quad(&render_handles, 100, 100, 50, 50, 0.0f, 0.0f, 1.0f,
                1.0f, texture);
        }

        render_flush(&render_handles);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    render_free(&render_handles);
    glfwTerminate();
    return 0;
}