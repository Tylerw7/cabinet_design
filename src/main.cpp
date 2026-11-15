#include <iostream>
#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Mesh.h"
#include "Shader.h"




static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);


};




int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #endif  

    GLFWwindow* window = glfwCreateWindow(
        1280,
        800,
        "fastCabinet",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    std::vector<float> vertices = 
    {
        //   position            color
         0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // top    - red
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // left   - green
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // right  - blue
    };

    std::vector<std::uint32_t> indices =
    {
        0, 1, 2
    };

    Mesh triangle(vertices, indices);

    // Confirm what we actually got from the driver.
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    // Keep the viewport in sync with the window size.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Set the initial viewport to the current framebuffer size.
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.15f, 0.18f, 0.22f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        triangle.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}