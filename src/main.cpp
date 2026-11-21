#include <iostream>
#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

// ****************************************************
// Hook ups for mouse
// ****************************************************
struct InputState
{
    Camera* camera = nullptr;

    bool   orbiting = false;   // left button held
    bool   panning  = false;   // right button held
    double lastX = 0.0;
    double lastY = 0.0;
};

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
    auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
    if (!input) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
        input->orbiting = (action == GLFW_PRESS);
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        input->panning = (action == GLFW_PRESS);

    // Record where the drag started to avoid a jump on first move.
    glfwGetCursorPos(window, &input->lastX, &input->lastY);
}

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
    if (!input || !input->camera) return;

    float dx = static_cast<float>(xpos - input->lastX);
    float dy = static_cast<float>(ypos - input->lastY);
    input->lastX = xpos;
    input->lastY = ypos;

    if (input->orbiting)
    {
        // Drag right → orbit right; drag up → pitch up.
        input->camera->orbit(dx * 0.3f, -dy * 0.3f);
    }
    else if (input->panning)
    {
        input->camera->pan(-dx * 0.01f, dy * 0.01f);
    }
}

static void scrollCallback(GLFWwindow* window, double, double yoffset)
{
    auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
    if (!input || !input->camera) return;

    // Scroll up → zoom in (decrease distance).
    input->camera->zoom(static_cast<float>(-yoffset) * 0.5f);
}




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



    // Camera
    Camera camera;




    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.15f, 0.18f, 0.22f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        // Model
        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view = camera.getViewMatrix();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)w / (float)h,
            0.1f,
            100.0f
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