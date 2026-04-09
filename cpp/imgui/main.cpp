#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#ifndef IMGUI_DISABLED
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#include "lib/get_shader_program.h"

static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    (void) scancode; (void) mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWmonitor *getLaptopMonitor() {
    int monitorsLen;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorsLen);
    int monitorIndex = -1;
    std::string monitorName = "eDP-1";
    for (int i = 0; i < monitorsLen; i++) {
        std::string currentMonitorName = glfwGetMonitorName(monitors[i]);
        if (currentMonitorName == monitorName) {
            monitorIndex = i;
        }
    }    

    return monitors[monitorIndex];
}

GLFWwindow *initWindow() {
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return NULL;
    }

    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = getLaptopMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "ImGui + GLFW + GLAD", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return NULL;
    }

    // make the context of the window the main context on the current thread
    glfwMakeContextCurrent(window);

    // Enable vsync
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    // Initialize the OpenGL function pointers for GLAD using the cross-platform
    // getter function provided by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return NULL;
    }


    return window;
}

#ifndef IMGUI_DISABLED
void initImgui(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
#endif

#ifndef IMGUI_DISABLED
void renderImgui(ImGuiIO& io, bool *showDemo) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (*showDemo) {
        ImGui::ShowDemoWindow(showDemo);
    }

    ImGui::Begin("Demo");
    ImGui::Text("Hello from ImGui!");
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Checkbox("Demo Window", showDemo);      // Edit bools storing our window open/close state
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#endif

int main()
{
    GLFWwindow *window = initWindow();

#ifndef IMGUI_DISABLED
    initImgui(window);
    ImGuiIO& io = ImGui::GetIO();
    bool showDemo = false;
#endif

    GLuint vertexArrays[1];
    glGenVertexArrays(1, vertexArrays);
    glBindVertexArray(vertexArrays[0]);

    // Create buffer
    GLuint buffers[1];
    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

    // Store vertex positions into the buffer
    float vertexPositions[] = {
         0.0,  0.5, 0.0, // top
         0.5, -0.5, 0.0, // bottom right
        -0.5, -0.5, 0.0, // bottom left
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

    // Tell the vertex shader how to interpret the buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader shaders[] = {
        {GL_VERTEX_SHADER, "shaders/shader.vs"},
        // {GL_FRAGMENT_SHADER, "shaders/shader.fs"},
    };
    GLuint shaderProgram = get_shader_program(shaders, 1);
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // int displayWidth, displayHeight;
        // glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        // glViewport(0, 0, displayWidth, displayHeight);
        // glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

#ifndef IMGUI_DISABLED
    renderImgui(io, &showDemo);
#endif

        glfwSwapBuffers(window);
    }

    return 0;
}