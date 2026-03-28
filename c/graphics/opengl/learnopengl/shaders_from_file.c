#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "get_shader_program.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

void handle_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main() {
    glfwInit();
    // Use OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use the core profile of OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    // make the context of the window the main context on the current thread.
    glfwMakeContextCurrent(window);

    // Initialize the OpenGL function pointers for GLAD using the cross-platform
    // getter function provided by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }

    // Update the viewport dimensions when the window is resized. All callback 
    // functions should be registered after the window is created and before
    // starting the render loop.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile the vertex shader
    GLuint shaderProgram = get_shader_program();

    // Check shader program linking errors
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader program linking failed\n%s\n", infoLog);
        return 1;
    }

    // Vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Specify the array to store the vertex attribute arrays in 
    // (in other words, how OpenGL should interpret the vertex data)
    glVertexAttribPointer(
        // index – identifies the generic vertex attribute array being described.
        // We specified "location = 0" in the vertex shader.
        0,
        3, // the number of values per attribute as well as their component ordering
        GL_FLOAT, // the data type of the values stored in the array
        // Specifies whether the data type should be normalized when converting 
        // to float. Our data is already in floats so this is irrelevant. 
        GL_FALSE,
        // stride - the distance in bytes from the start of one attribute to 
        // the start of the next
        6 * sizeof(float),
        // pointer - specifies the offset within a buffer of the first value 
        // of the first element of the array being specified.
        (void*)0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop. Each iteration renders a new frame.
    while(!glfwWindowShouldClose(window)) {
        // glClearColor(1.0f, 0.33f, 0.1f, 1.0f);
        glClearColor(0.53f, 0.18f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram); // this needs to run before setting the uniform value

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        // Check input events and trigger the appropriate callback functions
        glfwPollEvents();

        handle_input(window);
    }

    return 0;
}