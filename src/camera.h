#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include <cglm/cglm.h>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Camera defaults
const float YAW = -90.0f;
const float PITCH = 0.0f;

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

typedef struct Camera {
    // camera Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    // Euler angles (no roll)
    float yaw;
    float pitch;

    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;   
} Camera;

Camera new_camera();

#endif