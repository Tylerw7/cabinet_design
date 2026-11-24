#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Grid.h"

#include <iostream>
#include <vector>
#include <cstdint>

// ---------------------------------------------------------------------------
// Input handling
// ---------------------------------------------------------------------------

struct InputState
{
    Camera* camera = nullptr;

    bool   orbiting = false;   // left button held
    bool   panning  = false;   // right button held
    double lastX = 0.0;
    double lastY = 0.0;
};

static void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

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

    if (ImGui::GetIO().WantCaptureMouse)
        return;

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
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
    if (!input || !input->camera) return;

    // Scroll up → zoom in (decrease distance).
    input->camera->zoom(static_cast<float>(-yoffset) * 0.5f);
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main()
{
    // --- GLFW init ---
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        1280, 800, "cabinetDesign", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // --- GLAD init (after context current, before any gl* call) ---
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    // --- Viewport + resize handling ---
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    // Depth testing so near geometry correctly covers far geometry.
    glEnable(GL_DEPTH_TEST);

    // --- Camera + input (register callbacks BEFORE ImGui init) ---
    Camera camera;

    InputState input;
    input.camera = &camera;
    glfwSetWindowUserPointer(window, &input);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // --- ImGui setup (chains onto the callbacks registered above) ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // --- Scene resources ---
    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    std::vector<float> vertices = {
        //   position            color
         0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // top    - red
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // left   - green
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // right  - blue
    };
    std::vector<std::uint32_t> indices = { 0, 1, 2 };
    Mesh triangle(vertices, indices);

    Shader gridShader("assets/shaders/grid.vert", "assets/shaders/grid.frag");
    Grid grid(20, 0.5f);   // 20 lines each way, 0.5 units apart

    // --- Render loop ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Define UI
        ImGui::Begin("Cabinet Designer");
        ImGui::Text("Phase 1 foundation online.");
        ImGui::Text("Camera and viewport active.");
        ImGui::End();

        // Clear
        glClearColor(0.15f, 0.18f, 0.22f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shared camera matrices for this frame
        glm::mat4 view = camera.getViewMatrix();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);

        // --- Grid (drawn first, under everything) ---
        gridShader.use();
        gridShader.setMat4("view", view);
        gridShader.setMat4("projection", projection);
        gridShader.setVec3("lineColor", glm::vec3(0.4f, 0.4f, 0.4f));
        grid.draw();

        // --- Triangle ---
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        triangle.draw();

        // --- ImGui on top ---
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // --- Cleanup ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}