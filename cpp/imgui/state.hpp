#ifndef STATE_H
#define STATE_H

typedef struct {
    float yaw;
    float pitch;
} Camera;

Camera camera;

void initState() {
    camera.yaw = 0.0;
    camera.pitch = 0.0;
}

#endif