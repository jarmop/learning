#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "state.hpp"

bool mouseRightPressed = false;
bool firstCursorPos = true;
double prevXpos = 0.0;
double prevYpos = 0.0;

static void onFramebufferSize(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    (void) scancode; (void) mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void onMouseButton(GLFWwindow *window, int button, int action, int mods) {
    (void) mods;
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            mouseRightPressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            mouseRightPressed = false;
            firstCursorPos = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    } 
}

void updateCamera() {
    camera.front[0] = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front[1] = sin(glm::radians(camera.pitch));
    camera.front[2] = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    glm::normalize(camera.front);
    camera.right = glm::cross(camera.front, worldUp);
    glm::normalize(camera.right);
}

void onCursorPos(GLFWwindow* window, double xpos, double ypos) {
    (void) window;
    if (!mouseRightPressed) {
        return;
    }

    if (firstCursorPos) {
        firstCursorPos = false;
        prevXpos = xpos;
        prevYpos = ypos;
    }

    float mouseSensitivity = 0.1;
    camera.yaw += mouseSensitivity * (xpos - prevXpos);
    camera.pitch -= mouseSensitivity * (ypos - prevYpos);

    prevXpos = xpos;
    prevYpos = ypos;

    updateCamera();
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

    glfwSetFramebufferSizeCallback(window, onFramebufferSize);
    glfwSetKeyCallback(window, onKey);
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onCursorPos);

    // Initialize the OpenGL function pointers for GLAD using the cross-platform
    // getter function provided by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return NULL;
    }

    return window;
}