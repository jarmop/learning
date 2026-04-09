#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "lib/get_shader_program.h"
#include "window.hpp"
#include "gui.hpp"

int main() {
    GLFWwindow *window = initWindow();

#ifndef IMGUI_DISABLED
    ImGuiIO& io = initImgui(window);
    // ImGuiIO& io = ImGui::GetIO();
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
    float vertexDataLen = 4;
    float vertexData[] = {
         0.5,  0.5, 0.0, // top right
         0.5, -0.5, 0.0, // bottom right
        -0.5,  0.5, 0.0, // top left
        -0.5, -0.5, 0.0, // bottom left
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Tell the vertex shader how to interpret the buffer data
    // glVertexAttribPointer( index, size (values per attribute), type, normalized, stride, pointer )
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader shaders[] = {
        {GL_VERTEX_SHADER, "shaders/shader.vs"},
        // {GL_FRAGMENT_SHADER, "shaders/shader.fs"},
    };
    GLuint shaderProgram = get_shader_program(shaders, 1);
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // int displayWidth, displayHeight;
        // glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        // glViewport(0, 0, displayWidth, displayHeight);
        // glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        // glDrawArrays(GL_POINTS, 0, vertexDataLen);
        // glDrawArrays(GL_LINE_STRIP, 0, vertexDataLen);
        // glDrawArrays(GL_LINE_LOOP, 0, vertexDataLen);
        // glDrawArrays(GL_LINES, 0, vertexDataLen);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexDataLen);
        // glDrawArrays(GL_TRIANGLE_FAN, 0, vertexDataLen);
        // glDrawArrays(GL_TRIANGLES, 0, vertexDataLen);

#ifndef IMGUI_DISABLED
        renderImgui(io, &showDemo);
#endif

        glfwSwapBuffers(window);
    }

    return 0;
}