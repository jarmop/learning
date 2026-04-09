#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "lib/get_shader_program.h"
#include <GLFW/glfw3.h>
#include "glad/glad.h"

// Update the viewport dimensions when the window is resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode; (void) mods;
    // fprintf(stderr, "%d - %d\n", key, action);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow *init_Window() {
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
        return NULL;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

int main() {
    GLFWwindow *window = init_Window();
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        return 1;
    }

    GLuint vertexArrays[1];
    glGenVertexArrays(1, vertexArrays);
    glBindVertexArray(vertexArrays[0]);

    GLuint vertexBuffers[1];
    glGenBuffers(1, vertexBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Pointer for the position attribute values
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
    // Pointer for the color attribute values
    // glVertexAttribPointer( index, size, type, normalized, stride, pointer )
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // GLuint shaderProgram = get_shader_program("shaders/shader.vs", "shaders/shader.fs");
    Shader shaders[] = {
        {GL_VERTEX_SHADER, "shaders/shader.vs"},
        // {GL_FRAGMENT_SHADER, "shaders/shader.fs"},
    };
    GLuint shaderProgram = get_shader_program(shaders, 1);
    glUseProgram(shaderProgram);

    // Render loop. Each iteration renders a new frame.
    while(!glfwWindowShouldClose(window)) {
        // glClearColor(0.0, 0.0, 0.0, 1.0);
        // glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        // Check input events and trigger the appropriate callback functions.
        // Needed, for example, for the window toolbar to work.
        glfwPollEvents();
    }

    return 0;
}