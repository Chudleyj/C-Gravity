#pragma once
#include "glad/include/glad/glad.h"
#include "vectors.h"
#include "utils.h"

#define YAW -90.0f
#define PITCH -30.0f
#define SPEED 1.5f
#define SENSITIVITY 0.1f
#define ZOOM 100.0f

typedef enum{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT, 
    UP,
    DOWN
}Camera_Movement;

typedef struct {
    vec3_t position;
    vec3_t front; 
    vec3_t up; 
    vec3_t right;
    vec3_t worldUp; 

    float yaw; 
    float pitch; 

    float movementSpeed; 
    float mouseSensitivity; 
    float zoom; 

}Camera;

Camera camera_init_vectors(vec3_t position, vec3_t up, float yaw, float pitch);
Camera camera_init_scalar(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
mat4_t camera_get_view_matrix(Camera cam);
void camera_ProcessKeyboard(Camera *cam, Camera_Movement direction, const float deltaTime);
void camera_ProcessMouseMovement(Camera* cam, float xoffset, float yoffset, GLboolean constrainPitch);
void camera_ProcessMouseScroll(Camera *cam, float yoffset);