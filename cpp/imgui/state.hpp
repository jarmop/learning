#ifndef STATE_H
#define STATE_H

#include <glm/glm.hpp>

glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);

typedef struct {
    float yaw;
    float pitch;
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float speed;
} Camera;

Camera camera;

void initState() {
    camera.yaw = -62.0;
    camera.pitch = -25.0;
    camera.pos = glm::vec3(-3.0, 3.0, 6.0);
    camera.front = glm::vec3(0.0, 0.0, -1.0);
    camera.up = glm::vec3(0.0, 1.0, 0.0);
    camera.right = glm::vec3(1.0, 0.0, 0.0);
    camera.speed = 0.05;
}

#endif