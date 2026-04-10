#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

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
    GLFWwindow *window = glfwCreateWindow(mode->width / 2, mode->height, "ImGui + GLFW + GLAD", nullptr, nullptr);
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