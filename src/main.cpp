#include <iostream>
#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

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

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}