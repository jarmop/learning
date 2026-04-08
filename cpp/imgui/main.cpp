#include "glad/glad.h"
#include <GLFW/glfw3.h>

#ifndef IMGUI_DISABLED
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode; (void) mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* init_window() {
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return NULL;
    }

    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui + GLFW + GLAD", nullptr, nullptr);
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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

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
void init_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
#endif

#ifndef IMGUI_DISABLED
void render_imgui(ImGuiIO& io) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Demo");
    ImGui::Text("Hello from ImGui!");
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#endif

int main()
{
    GLFWwindow* window = init_window();

#ifndef IMGUI_DISABLED
    init_imgui(window);
    ImGuiIO& io = ImGui::GetIO();
#endif

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

#ifndef IMGUI_DISABLED
        render_imgui(io);
#endif

        glfwSwapBuffers(window);
    }

    // -------------------------
    // Cleanup
    // -------------------------
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();

    // glfwDestroyWindow(window);
    // glfwTerminate();

    return 0;
}