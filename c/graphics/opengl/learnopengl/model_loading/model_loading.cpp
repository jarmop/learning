#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <cglm/cglm.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../lib/stb_image.h"
#include "../lib/get_shader_program.h"
// #include "learnopengl/model.hpp"
#include "model_loader.h"

// settings
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
    {-2.0, 1.5, 3.5}, // pos
    {0.0, 0.0, 0.0}, // front
    {0.0, 0.0, 0.0},  // right
    -60.0, // yaw
    -20.0, // pitch
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

void updateCamera() {
    camera.front[0] = cos(glm_rad(camera.yaw)) * cos(glm_rad(camera.pitch));
    camera.front[1] = sin(glm_rad(camera.pitch));
    camera.front[2] = sin(glm_rad(camera.yaw)) * cos(glm_rad(camera.pitch));
    glm_normalize(camera.front);
    glm_cross(camera.front, worldUp, camera.right);
    glm_normalize(camera.right);
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

    updateCamera();
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

// unsigned int VAO, VBO, EBO;

// void setupMesh() {
//     // create buffers/arrays
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);
//     // load data into vertex buffers
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     // A great thing about structs is that their memory layout is sequential for all its items.
//     // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
//     // again translates to 3/2 floats which translates to a byte array.
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

//     // set the vertex attribute pointers
//     // vertex Positions
//     glEnableVertexAttribArray(0);	
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//     // vertex normals
//     glEnableVertexAttribArray(1);	
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//     // vertex texture coords
//     glEnableVertexAttribArray(2);	
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//     // vertex tangent
//     glEnableVertexAttribArray(3);
//     glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//     // vertex bitangent
//     glEnableVertexAttribArray(4);
//     glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
//     // ids
//     glEnableVertexAttribArray(5);
//     glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

//     // weights
//     glEnableVertexAttribArray(6);
//     glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
//     glBindVertexArray(0);
// }

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

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");

        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    GLuint shaderProgram = get_shader_program("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
    // Model ourModel("../assets/backpack/backpack.obj");
    load_obj("../assets/backpack", "backpack.obj");

    
    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    updateCamera();

    // render loop
    // -----------
    // while (!glfwWindowShouldClose(window)) {
    while (false) {
        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        // ourShader.use();
        glUseProgram(shaderProgram);

        // view/projection transformations
        mat4 projection;
        glm_perspective(glm_rad(camera.fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1, 100.0, projection);


        mat4 view;
        vec3 cameraPosFront;
        glm_vec3_add(camera.pos, camera.front, cameraPosFront);
        glm_lookat(camera.pos, cameraPosFront, worldUp, view);

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat *)projection);

        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view);

        mat4 model;
        glm_mat4_identity(model);
        vec3 translation = { 0.0,  0.0,  0.0};
        glm_translate(model, translation);
        vec3 translation2 = { 1.0,  1.0,  1.0};
        glm_scale(model, translation2);
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
        // ourModel.Draw(shaderProgram);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        handle_camera_movement_keys(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
