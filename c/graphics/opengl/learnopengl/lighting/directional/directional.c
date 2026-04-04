#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <cglm/cglm.h>

#include "../../glad/glad.h"
#include <GLFW/glfw3.h>

#include "../../lib/get_shader_program.h"
#include "../../lib/stb_image.h"
#include "../../lib/util.h"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const float MOUSE_SENSITIVITY = 0.1;
const float MAX_FOV = 45.0;

struct camera {
    vec3 pos;
    vec3 front;
    vec3 right;
    float yaw;
    float pitch;
    float speed;
    float fov;
} camera = {
    {0.0, 0.0, 3.0}, // pos
    {0.0, 0.0, -1.0}, // front
    {1.0, 0.0, 0.0},  // right
    -90.0, // yaw
    0.0, // pitch
    2.5, // speed
    MAX_FOV // fov
};

vec3 worldUp = {0.0, 1.0, 0.0};

float prevMouseX, prevMouseY;
bool mouseRightPressed = false;
bool firstMouse = true;
float previousFrameTime = 0.0; // Time of the previous frame

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void)window; (void)mods;
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            mouseRightPressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            mouseRightPressed = false;
            firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    } 
}

void mouse_callback(GLFWwindow* window, double mouseX, double mouseY) {
    (void) window;
    if (!mouseRightPressed) {
        return;
    }
    // fprintf(stderr, "x: %d, y: %d\n", (int)xpos, (int)ypos);

    if (firstMouse) {
        prevMouseX = mouseX;
        prevMouseY = mouseY;
        firstMouse = false;
    }

    camera.yaw += (mouseX - prevMouseX) * MOUSE_SENSITIVITY;
    camera.pitch -= (mouseY - prevMouseY) * MOUSE_SENSITIVITY;
    prevMouseX = mouseX;
    prevMouseY = mouseY;

    if (camera.pitch > 89.0) {
        camera.pitch = 89.0;
    } else if (camera.pitch < -89.0) {
        camera.pitch = -89.0;
    }

    camera.front[0] = cos(glm_rad(camera.yaw)) * cos(glm_rad(camera.pitch));
    camera.front[1] = sin(glm_rad(camera.pitch));
    camera.front[2] = sin(glm_rad(camera.yaw)) * cos(glm_rad(camera.pitch));
    glm_normalize(camera.front);
    glm_cross(camera.front, worldUp, camera.right);
    glm_normalize(camera.right);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // fprintf(stderr, "%d\n", (int)fov);
    (void)window; (void)xoffset;
    camera.fov -= (float)yoffset;
    if (camera.fov < 1.0)
        camera.fov = 1.0;
    if (camera.fov > MAX_FOV)
        camera.fov = MAX_FOV; 
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode; (void) mods;
    // fprintf(stderr, "%d - %d\n", key, action);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

/**
 * GLFW input getters like glfwGetKey return the last state for the specified 
 * key, ignoring GLFW_REPEAT as it's the same as GLFW_PRESS in this context, so 
 * they only return GLFW_PRESS or GLFW_KEY.
 */
void handle_camera_movement_keys(GLFWwindow *window) {
    float currentTime = glfwGetTime();
    const float cameraMovement = camera.speed * (currentTime - previousFrameTime);
    previousFrameTime = currentTime;
    
    // WSAD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_muladds(camera.front, cameraMovement, camera.pos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_mulsubs(camera.front, cameraMovement, camera.pos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_vec3_mulsubs(camera.right, cameraMovement, camera.pos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_vec3_muladds(camera.right, cameraMovement, camera.pos);
    }

    // Elevation
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glm_vec3_muladds(worldUp, cameraMovement, camera.pos);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        glm_vec3_mulsubs(worldUp, cameraMovement, camera.pos);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // Render elements at the front over the ones in the back
    glEnable(GL_DEPTH_TEST);

    // Vertex buffer object
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    float vertices[] = {
        // vertices           // surface normal vectors
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex array object
    GLuint targetCubeVAO;
    glGenVertexArrays(1, &targetCubeVAO);
    glBindVertexArray(targetCubeVAO);
    // glVertexAttribPointer args --> index, size, type, normalized, stride, pointer
    // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // aNormal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vec3  lightCubePosition = {1.2, 1.0, 2.0};

    GLuint targetCubeShader = get_shader_program("lighting/directional/target_cube.vs", "lighting/directional/target_cube.fs");
    GLuint lightCubeShader = get_shader_program("lighting/directional/light_cube.vs", "lighting/directional/light_cube.fs");

    // Render loop. Each iteration renders a new frame.
    while(!glfwWindowShouldClose(window)) {
        // glClearColor(0.53f, 0.18f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the target cube

        glUseProgram(targetCubeShader);
        glUniform3f(glGetUniformLocation(targetCubeShader, "objectColor"), 1.0, 0.5, 0.31);
        glUniform3f(glGetUniformLocation(targetCubeShader, "lightColor"), 1.0, 1.0, 1.0);
        glUniform3fv(glGetUniformLocation(targetCubeShader, "lightPos"), 1, &lightCubePosition[0]);
        glUniform3fv(glGetUniformLocation(targetCubeShader, "viewPos"), 1, &camera.pos[0]);

        mat4 view;
        vec3 cameraPosFront;
        glm_vec3_add(camera.pos, camera.front, cameraPosFront);
        glm_lookat(camera.pos, cameraPosFront, worldUp, view);
        int viewLoc = glGetUniformLocation(targetCubeShader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view);

        mat4 projection;
        glm_perspective(glm_rad(camera.fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1, 100.0, projection);
        int projectionLoc = glGetUniformLocation(targetCubeShader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat *)projection);

        mat4 model;
        glm_mat4_identity(model);
        glm_translate(model, (vec3){ 0.0,  0.0,  0.0});
        int modelLoc = glGetUniformLocation(targetCubeShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);

        glBindVertexArray(targetCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the light cube
        glUseProgram(lightCubeShader);
        viewLoc = glGetUniformLocation(lightCubeShader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view);        
        projectionLoc = glGetUniformLocation(lightCubeShader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat *)projection);
        glm_mat4_identity(model);
        glm_translate(model, lightCubePosition);
        glm_scale(model, (vec3){0.2, 0.2, 0.2});
        modelLoc = glGetUniformLocation(lightCubeShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);

        // Check input events and trigger the appropriate callback functions
        glfwPollEvents();

        handle_camera_movement_keys(window);
    }

    return 0;
}