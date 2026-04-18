#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>

#include "lib/get_shader_program.h"
#include "lib/obj_loader.hpp"
#include "lib/stb_image.h"
#include "window.hpp"
#include "gui.hpp"
#include "state.hpp"

void onFrame(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.pos += camera.speed * camera.front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.pos -= camera.speed * camera.front;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.pos += camera.speed * camera.right;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.pos -= camera.speed * camera.right;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.pos += camera.speed * worldUp;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.pos -= camera.speed * worldUp;
    }
}

void setTexture(GLuint texture, GLenum textureUnit, const char *imagePath, GLint format) {
    glActiveTexture(textureUnit); // select texture unit 0 as the unit to bind to
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data) {
        /**
         * Set the main texture image
         */
        glTexImage2D(
            GL_TEXTURE_2D,      // target
            0,                  // level of detail (0 = the main image)
            format,             // internal format
            width,              // width of the image
            height,             // height of the image
            0,                  // Border width. Affects the dimensions of the texture (whatever that means)
            format,             // format of the data
            GL_UNSIGNED_BYTE,   // type of the data
            data                // the pixel data
        );
        // Generate the various lower resolution versions of the main image
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fprintf(stderr, "Failed to load %s\n", imagePath);
    }
    stbi_image_free(data);
}

int main() {
    GLFWwindow *window = initWindow();
    initState();

#ifndef IMGUI_DISABLED
    ImGuiIO& io = initImgui(window);
    // ImGuiIO& io = ImGui::GetIO();
    bool showDemo = false;
#endif

    // Render elements at the front over the ones in the back
    glEnable(GL_DEPTH_TEST);

    GLuint vertexArrays[1];
    glGenVertexArrays(1, vertexArrays);
    glBindVertexArray(vertexArrays[0]);

    // Create buffer
    GLuint buffers[1];
    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

    int vpStride = 3;
    int vnStride = 3;
    int vtStride = 2;
    int vStride = vpStride + vnStride + vtStride;
    int numOfVertices;
    float *vertexData = loadObj(&numOfVertices);

    // for (int i = 0; i < numOfVertices * vStride; i++) {
    //     fprintf(stderr, "%f, ", vertexData[i]);
    //     if ((i + 1) % vStride == 0) {
    //         fprintf(stderr, "\n");
    //     }
    // }

    glBufferData(GL_ARRAY_BUFFER, numOfVertices * vStride * sizeof(float *), vertexData, GL_STATIC_DRAW);
    // Tell the vertex shader how to interpret the buffer data
    // glVertexAttribPointer( index, size (values per attribute), type, normalized, stride, pointer )
    glVertexAttribPointer(0, vpStride, GL_FLOAT, GL_FALSE, vStride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, vnStride, GL_FLOAT, GL_FALSE, vStride * sizeof(float), (void*)(vpStride * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, vtStride, GL_FLOAT, GL_FALSE, vStride * sizeof(float), (void*)((vpStride + vnStride) * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint textures[1];
    glGenTextures(1, textures);
    // setTexture(textures[0], GL_TEXTURE0, "assets/crate.jpg", GL_RGB);
    setTexture(textures[0], GL_TEXTURE0, "assets/moon_diffuse.png", GL_RGBA);
    // setTexture(textures[0], GL_TEXTURE0, "assets/moon_bump.png", GL_RGBA);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader shaders[] = {
        {GL_VERTEX_SHADER, "shaders/shader.vs"},
        {GL_FRAGMENT_SHADER, "shaders/shader.fs"},
    };
    GLuint shaderProgram = get_shader_program(shaders, 2);
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texSampler"), 0);

    glm::mat4 model = glm::mat4(1.0f);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    updateCamera();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        onFrame(window);

        unsigned int sceneLoc = glGetUniformLocation(shaderProgram, "view");
        glm::mat4 scene = glm::mat4(1.0f);
        // z is negative, because we are moving the scene away from us
        scene = glm::translate(scene, -camera.pos);
        scene = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
        glUniformMatrix4fv(sceneLoc, 1, GL_FALSE, glm::value_ptr(scene));

        unsigned int projectionLoc  = glGetUniformLocation(shaderProgram, "projection");
        glm::mat4 projection;
        // Recalculate the aspect ratio on every frame to handle window resize
        int displayWidth, displayHeight;
        glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        projection = glm::perspective(glm::radians(45.0f), (float)displayWidth / (float)displayHeight, 0.1f, 100.0f);
        // projection = glm::rotate(projection,  glm::radians(camera.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        // projection = glm::rotate(projection,  glm::radians(camera.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Lights
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0, 0.5, 0.31);
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &camera.pos[0]);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.color"), 1.0, 1.0, 1.0);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.direction"), -0.2, -1.0, -0.3);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.ambient"), 0.2, 0.2, 0.2);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.diffuse"), 0.5, 0.5, 0.5);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.specular"), 1.0, 1.0, 1.0);

        // int displayWidth, displayHeight;
        // glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        // glViewport(0, 0, displayWidth, displayHeight);
        // glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, numOfVertices);

#ifndef IMGUI_DISABLED
        renderImgui(io, &showDemo);
#endif

        glfwSwapBuffers(window);
    }

    return 0;
}