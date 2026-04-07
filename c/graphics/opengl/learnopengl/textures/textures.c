#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include "../lib/get_shader_program.h"
#include "../lib/stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

void handle_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer object
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer args --> index, size, type, normalized, stride, pointer
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint textures[2];
    // Fill the textures array with two texture names, and mark those names as
    // being used. Not strictly necessary.
    glGenTextures(2, textures);

    setTexture(textures[0], GL_TEXTURE0, "assets/container.jpg", GL_RGB);
    setTexture(textures[1], GL_TEXTURE1, "assets/awesomeface.png", GL_RGBA);

    GLuint shaderProgram = get_shader_program("textures/shader.vs", "textures/shader.fs");
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texSampler1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texSampler2"), 1);

    // Render loop. Each iteration renders a new frame.
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.53f, 0.18f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        // Check input events and trigger the appropriate callback functions
        glfwPollEvents();

        handle_input(window);
    }

    return 0;
}