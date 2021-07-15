#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#include <glad/glad.h>

#include <cglm/cglm.h>

// Camera defaults
#define CAM_DEFAULT_YAW     -90.0f
#define CAM_DEFAULT_PITCH   0.0f

const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_ZOOM = 45.0f;

typedef struct Camera {
    // camera attributes
    float *position;
    float *front;
    float *up;
    float *right;
    float *world_up;

    // Euler angles (no roll)
    float yaw;
    float pitch;

    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;   
} Camera;

typedef enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} CameraMovement;

void update_camera_vectors(Camera *camera);
void get_view_matrix(Camera *camera, mat4 matrix);
void process_keyboard(Camera *camera, CameraMovement direction, float delta);

Camera new_camera(vec3 position, vec3 world_up, float yaw, float pitch)
{
    Camera camera = {
        position,
        GLM_VEC3_ZERO,
        GLM_VEC3_ZERO,
        GLM_VEC3_ZERO,
        world_up,

        yaw,
        pitch,

        DEFAULT_SPEED,
        DEFAULT_SENSITIVITY,
        DEFAULT_ZOOM
    };

    update_camera_vectors(&camera);

    // printf("FWD: ");
    // for(int i = 0; i < 3; i++)
    //     printf("%f, ", camera.front[2]);
    // printf("\n");

    // printf("RGHT: ");
    // for(int i = 0; i < 3; i++)
    //     printf("%f, ", camera.right[i]);
    // printf("\n");

    // printf("UP: ");
    // for(int i = 0; i < 3; i++)
    //     printf("%f, ", camera.up[i]);
    // printf("\n");

    return camera;
}

// finds new front, right and up vectors
void update_camera_vectors(Camera *camera)
{
    // calculate new front vector
    float f_x = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    float f_y = sin(glm_rad(camera->pitch));
    float f_z = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));

    vec3 front = { f_x, f_y, f_z };
    glm_normalize(front);
    camera->front = front;

    vec3 right;
    glm_cross(front, camera->world_up, right);
    glm_normalize(right);
    camera->right = right;

    vec3 up;
    glm_cross(right, front, up);
    glm_normalize(up);
    camera->up = up;
}

// copy lookat matrix into matrix variable
void get_view_matrix(Camera *camera, mat4 matrix)
{
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);

    glm_lookat(camera->position, center, camera->up, matrix);
}

void process_keyboard(Camera *camera, CameraMovement direction, float delta)
{
    float velocity = camera->movement_speed * delta;

    if (direction == FORWARD)
    {
        float *vec;
        vec = GLM_VEC3_ONE;
        glm_vec3_scale(camera->front, velocity, vec);
        glm_vec3_add(camera->position, vec, camera->position);

        printf("FRONT: ");
        for(int i = 0; i < 3; i++)
            printf("%f, ", camera->front[i]);
        printf("\n");

        printf("VEL: ");
        printf("%f, ", velocity);
        printf("\n");

        printf("VEC: ");
        for(int i = 0; i < 3; i++)
            printf("%f, ", vec[i]);
        printf("\n");

        printf("POS: ");
        for(int i = 0; i < 3; i++)
            printf("%f, ", camera->position[i]);
        printf("\n");
        printf("\n");

        update_camera_vectors(camera);
    }

    if (direction == BACKWARD)
    {
        vec3 vec;
        glm_vec3_scale(camera->front, velocity, vec);
        glm_vec3_sub(camera->position, vec, camera->position);
    }

    if (direction == LEFT)
    {
        vec3 vec;
        glm_vec3_scale(camera->right, velocity, vec);
        glm_vec3_sub(camera->position, vec, camera->position);
    }


    if (direction == RIGHT)
    {
        vec3 vec;
        glm_vec3_scale(camera->right, velocity, vec);
        glm_vec3_add(camera->position, vec, camera->position);
    }
}

void process_mouse_movement(Camera *camera, float x_offset, float y_offset)
// void process_mouse_movement(Camera *camera, float xoffset, float yoffset, GLboolean constrainPitch = true)
{
    x_offset *= camera->mouse_sensitivity;
    y_offset *= camera->mouse_sensitivity;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    // constrain pitch
    // if (constrainPitch)
    // {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    // }

    update_camera_vectors(camera);
}

void process_mouse_scroll(Camera *camera, float y_offset)
{
    camera->zoom -= (float) y_offset;
    if (camera->zoom < 1.0f)
        camera->zoom = 1.0f;
    if (camera->zoom > 45.0f)
        camera->zoom = 45.0f; 
}

#endif