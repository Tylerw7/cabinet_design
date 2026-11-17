#include <iostream>
#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




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

        // Model
        glm::mat4 model = glm::mat4(1.0f);

        // model = glm::rotate(model, (float)glfwGetTime(),
        //                     glm::vec3(0.5f, 1.0f, 0.0f));

        // View - moves camera back
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // Projection 45 degree field of view
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)fbWidth / (float)fbHeight,
            0.1f, // near clip plane
            100.0f // far clip plane
        );

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        triangle.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}