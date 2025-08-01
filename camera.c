#include "camera.h"
#include <stdio.h>
static void update_camera_vectors(Camera* cam) {

    float yaw_rad = degrees_to_radians(cam->yaw);
    float pitch_rad = degrees_to_radians(cam->pitch);

    vec3_t front;

    front.x = cosf(yaw_rad) * cosf(pitch_rad);
    front.y = sinf(pitch_rad);
    front.z = sinf(yaw_rad) * cosf(pitch_rad);

    vec3_normalize(&front);
    cam->front = front;


    vec3_t right;
    vec3_cross_product(cam->front, cam->worldUp, &right);
    vec3_normalize(&right);
    cam->right = right;

    vec3_t up;
    vec3_cross_product(cam->right, cam->front, &up);
    vec3_normalize(&up);
    cam->up = up;
}

Camera camera_init_vectors(vec3_t position, vec3_t up, float yaw, float pitch){
    Camera cam; 

    cam.front = (vec3_t){ 0.0f, 0.0f, -1.0f };
    cam.movementSpeed = SPEED; 
    cam.mouseSensitivity = SENSITIVITY; 
    cam.zoom = ZOOM; 

    cam.position = position; 
    cam.worldUp = up; 
    cam.yaw = yaw; 
    cam.pitch = pitch; 
    update_camera_vectors(&cam); 

    return cam; 
}

Camera camera_init_scalar(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    Camera cam; 

    cam.front = (vec3_t){ 0.0f, 0.0f, -1.0f };
    cam.movementSpeed = SPEED;
    cam.mouseSensitivity = SENSITIVITY;
    cam.zoom = ZOOM;

    cam.position = (vec3_t){ posX, posY, posZ };
    cam.worldUp = (vec3_t){ upX, upY, upZ };
    cam.yaw = yaw; 
    cam.pitch = pitch; 
    update_camera_vectors(&cam);

    return cam;
}

mat4_t camera_get_view_matrix(Camera cam) {
    vec3_t target; 
    vec3_add(cam.position, cam.front, &target);
    mat4_t mat = mat4_lookAt(cam.position, target, cam.up);
    return mat; 
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void camera_ProcessKeyboard(Camera *cam, Camera_Movement direction, const float deltaTime){

    float velocity = cam->movementSpeed * deltaTime;


    if (direction == FORWARD) {
        printf("%f", deltaTime);
        cam->position.x += cam->front.x * velocity;  
        cam->position.y += cam->front.y * velocity;
        cam->position.z += cam->front.z * velocity;
    }
    if (direction == BACKWARD) {
        cam->position.x -= cam->front.x * velocity;
        cam->position.y -= cam->front.y * velocity;
        cam->position.z -= cam->front.z * velocity;
    }
    if (direction == LEFT) {
        cam->position.x -= cam->right.x * velocity;
        cam->position.y -= cam->right.y * velocity;
        cam->position.z -= cam->right.z * velocity;
    }
    if (direction == RIGHT) {
        cam->position.x += cam->right.x * velocity;
        cam->position.y += cam->right.y * velocity;
        cam->position.z += cam->right.z * velocity;
    }
    if (direction == UP) {
        cam->position.x += cam->up.x * velocity;
        cam->position.y += cam->up.y * velocity;
        cam->position.z += cam->up.z * velocity;
    }
    if (direction == DOWN) {
        cam->position.x -= cam->up.x * velocity;
        cam->position.y -= cam->up.y * velocity;
        cam->position.z -= cam->up.z * velocity;
    }
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void camera_ProcessMouseMovement(Camera *cam, float xoffset, float yoffset, GLboolean constrainPitch){

    xoffset *= cam->mouseSensitivity;
    yoffset *= cam->mouseSensitivity;
   
    cam->yaw += xoffset;
    cam->pitch += yoffset;
    
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (cam->pitch > 89.0f)
            cam->pitch = 89.0f;
        if (cam->pitch < -89.0f)
            cam->pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors(cam);

}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void camera_ProcessMouseScroll(Camera *cam, float yoffset){
    cam->zoom -= (float)yoffset;
    if (cam->zoom < 1.0f)
        cam->zoom = 1.0f;
    if (cam->zoom > ZOOM)
        cam->zoom = ZOOM;
}
