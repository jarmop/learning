#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lib/get_shader_program.h"
#include "window.hpp"
#include "gui.hpp"
#include "state.hpp"

int SCR_WIDTH = 1900; int SCR_HEIGHT = 1200;

int main() {
    GLFWwindow *window = initWindow();
    initState();

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
    float vertexData[] = {
        // back
        -0.5, -0.5, -0.5, // bottom left
        -0.5,  0.5, -0.5, // top left
         0.5,  0.5, -0.5, // top right
         0.5, -0.5, -0.5, // bottom right
         // front
        -0.5, -0.5,  0.5, // bottom left
        -0.5,  0.5,  0.5, // top left
         0.5,  0.5,  0.5, // top right
         0.5, -0.5,  0.5, // bottom right
    };
    GLuint BACK_BOTTOM_LEFT = 0;
    GLuint BACK_TOP_LEFT = 1;
    GLuint BACK_TOP_RIGHT = 2;
    GLuint BACK_BOTTOM_RIGHT = 3;
    GLuint FRONT_BOTTOM_LEFT = 4;
    GLuint FRONT_TOP_LEFT = 5;
    GLuint FRONT_TOP_RIGHT = 6;
    GLuint FRONT_BOTTOM_RIGHT = 7;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Tell the vertex shader how to interpret the buffer data
    // glVertexAttribPointer( index, size (values per attribute), type, normalized, stride, pointer )
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int indicesLen = 6 * 6;
    GLuint indices[] = {
        // Sides, starting from back, going clockwise
        BACK_BOTTOM_LEFT,   BACK_TOP_LEFT,      BACK_TOP_RIGHT,
        BACK_BOTTOM_LEFT,   BACK_BOTTOM_RIGHT,  BACK_TOP_RIGHT,
        BACK_BOTTOM_RIGHT,  BACK_TOP_RIGHT,     FRONT_TOP_RIGHT,
        BACK_BOTTOM_RIGHT,  FRONT_BOTTOM_RIGHT, FRONT_TOP_RIGHT,
        FRONT_BOTTOM_RIGHT, FRONT_TOP_RIGHT,    FRONT_TOP_LEFT,
        FRONT_BOTTOM_RIGHT, FRONT_BOTTOM_LEFT,  FRONT_TOP_LEFT,
        FRONT_BOTTOM_LEFT,  FRONT_TOP_LEFT,     BACK_TOP_LEFT,
        FRONT_BOTTOM_LEFT,  BACK_BOTTOM_LEFT,   BACK_TOP_LEFT,
        // Bottom
        FRONT_BOTTOM_LEFT,  BACK_BOTTOM_LEFT,   BACK_BOTTOM_RIGHT,
        FRONT_BOTTOM_LEFT,  FRONT_BOTTOM_RIGHT, BACK_BOTTOM_RIGHT,
        // Top
        FRONT_TOP_RIGHT,    FRONT_TOP_LEFT,     BACK_TOP_LEFT,
        FRONT_TOP_RIGHT,    BACK_TOP_RIGHT,     BACK_TOP_LEFT,
    };
    GLuint elementBuffers[1];
    glGenBuffers(1, elementBuffers);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffers[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader shaders[] = {
        {GL_VERTEX_SHADER, "shaders/shader.vs"},
        // {GL_FRAGMENT_SHADER, "shaders/shader.fs"},
    };
    GLuint shaderProgram = get_shader_program(shaders, 1);
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model,  glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    // model = glm::rotate(model,  glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // model = glm::rotate(model,  glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int sceneLoc = glGetUniformLocation(shaderProgram, "view");
    glm::mat4 scene = glm::mat4(1.0f);
    // z is negative, because we are moving the scene away from us
    scene = glm::translate(scene, glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(sceneLoc, 1, GL_FALSE, glm::value_ptr(scene));

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        unsigned int projectionLoc  = glGetUniformLocation(shaderProgram, "projection");
        glm::mat4 projection;
        // Recalculate the aspect ratio on every frame to handle window resize
        int displayWidth, displayHeight;
        glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        projection = glm::perspective(glm::radians(45.0f), (float)displayWidth / (float)displayHeight, 0.1f, 100.0f);
        projection = glm::rotate(projection,  glm::radians(camera.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::rotate(projection,  glm::radians(camera.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // int displayWidth, displayHeight;
        // glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        // glViewport(0, 0, displayWidth, displayHeight);
        // glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, indicesLen, GL_UNSIGNED_INT, 0);

#ifndef IMGUI_DISABLED
        renderImgui(io, &showDemo);
#endif

        glfwSwapBuffers(window);
    }

    return 0;
}