#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vectors.h"
#include "shader.h"
#include "utils.h"
#include "camera.h"
#include "solarobj.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define G 6.6743e-11 // m^3 kg^-1 s^-2
#define G_SCALED (G * METER_TO_OPENGL * METER_TO_OPENGL * METER_TO_OPENGL)
#define PHYSICS_TIMESTEP 1000.0f
//#define PHYSICS_TIMESTEP 3600.0f  // 1 hour per frame (much more reasonable)

//#define PHYSICS_TIMESTEP 86400.0f // 1 day per frame

#define MAX_ACCL 1e10f

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;// time between current frame and last frame

Camera *global_camera = NULL; 

bool firstMouse = true;

void processInput(GLFWwindow* window, Camera* cam);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

int main() {
    Camera camera = camera_init_vectors((vec3_t) { 0.0f, 50.0f, 500.0f },(vec3_t){ 0.0f, 1.0f, 1.0f }, YAW, PITCH);
    global_camera = &camera; 

    float lastFrame = 0.0f;

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Sphere", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
  

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to init GLAD\n");
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
   // Use real astronomical masses but convert to appropriate distances
    float sunMass = 1.989e30f; // Actual sun mass in kg
    float earthMass = 5.972e24f; // Actual earth mass in kg
    //float density = 5515.0f; // Keep real density
    
    // Use astronomical distances (converted to simulation units)
    float AU_SCALE = 1.496e11f / 10.0f; // 1 AU = 10 OpenGL units
    
    /*
    vec3_t startVels[5] = {
        (vec3_t){ 0.0f, 0.0f, 0.0f},           // Sun (stationary)
        (vec3_t){ 0.0f, 29783.6f / AU_SCALE, 0.0f},       // Earth orbital velocity (m/s)
        (vec3_t){ 0.0f, 24130.0f / AU_SCALE, 0.0f},       // Mars orbital velocity  
        (vec3_t){ 0.0f, 35020.0f / AU_SCALE, 0.0f},       // Venus orbital velocity
        (vec3_t){ 0.0f, 47870.0f / AU_SCALE, 0.0f}        // Mercury orbital velocity
    };
    
    vec3_t cubePositions[] = {
        (vec3_t){0.0f, 0.0f, 0.0f},                    // Sun at center
        (vec3_t){1.496e11f / AU_SCALE, 0.0f, 0.0f},    // Earth at 1 AU
        (vec3_t){2.279e11f / AU_SCALE, 0.0f, 0.0f},    // Mars at 1.52 AU
        (vec3_t){1.082e11f / AU_SCALE, 0.0f, 0.0f},    // Venus at 0.72 AU
        (vec3_t){5.791e10f / AU_SCALE, 0.0f, 0.0f}     // Mercury at 0.39 AU
    };
    */

    vec3_t cubePositions[] = {
    (vec3_t){0.0f, 0.0f, 0.0f},                                       // Sun at origin
    (vec3_t){AU_IN_METERS * METER_TO_OPENGL, 0.0f, 0.0f},              // Earth at 1 AU in OpenGL units
    (vec3_t){2.279e11f * METER_TO_OPENGL, 0.0f, 0.0f},                 // Mars at 1.52 AU
    (vec3_t){1.082e11f * METER_TO_OPENGL, 0.0f, 0.0f},                 // Venus at 0.72 AU
    (vec3_t){5.791e10f * METER_TO_OPENGL, 0.0f, 0.0f}                  // Mercury at 0.39 AU
    };

    for (int i = 0; i < 5; i++) {
        printf("Planet %d position: %.2f, %.2f, %.2f\n",
            i, cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
    }

    vec3_t startVels[5] = {
    (vec3_t){ 0.0f, 0.0f, 0.0f},           
    (vec3_t){ 0.0f, 29783.6f * METER_TO_OPENGL, 0.0f},       // Earth orbital velocity
    (vec3_t){ 0.0f, 24130.0f * METER_TO_OPENGL, 0.0f},       // Mars
    (vec3_t){ 0.0f, 35020.0f * METER_TO_OPENGL, 0.0f},       // Venus
    (vec3_t){ 0.0f, 47870.0f * METER_TO_OPENGL, 0.0f}        // Mercury
};

    float density[] = { 1410.0, 5515.0, 3930.0, 5240.0, 5430.0 };

    // Debug the actual positions stored in the array
    printf("\nActual cubePositions array:\n");
    for (int i = 0; i < 5; i++) {
        printf("Position %d: (%.6f, %.6f, %.6f)\n", i,
            cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
    }

    float masses[5] = {sunMass, earthMass, 6.39e23f, 4.867e24f, 3.285e23f}; // Sun, Earth, Mars, Venus, Mercury
    
    SolarObj SolarObjs[5];
    for (int i = 0; i < 5; i++) {
        solar_obj_init(&SolarObjs[i], cubePositions[i], startVels[i], masses[i], density[i]);
    }


    Shader shader = shader_create("vertex.glsl", "fragment.glsl");
    vec3_t lightPos = (vec3_t){ 1.2f, 1.0f, 2.0f };
    mat4_t projection;
    mat4_t model;

    SolarObj *currentObj = &SolarObjs[0]; 
    SolarObj *priorObj = &SolarObjs[0];
    vec3_t accelaerations[5];
    vec3_t oldAccelerations[5];

    while (!glfwWindowShouldClose(window)) {
 
   
            processInput(window, global_camera);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;


            shader_use(shader);

            mat4_t view = camera_get_view_matrix(*global_camera);
            shader_setmat4(shader, "view", &view);


            mat4_perspective(&projection, degrees_to_radians(global_camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            shader_setmat4(shader, "projection", &projection);

            mat4_t mtest;
            mat4_identity(&mtest);
            float testscale = 1.0f;
            mat4_t scale = mat4_scale(mtest, (vec3_t) { testscale, testscale, testscale });
            shader_setmat4(shader, "scale", &scale);

            // mat4_t rotateTest = mat4_rotate(mtest, (float)glfwGetTime(), (vec3_t) { 0.5f, 1.0f, 0.0f });
            // shader_setmat4(shader, "rotate", &rotateTest);

             //make the light move:
            lightPos.x = 5.0f * cos(currentFrame);
            lightPos.z = 5.0f * sin(currentFrame);
            lightPos.y = 2.0f;

            // Set light and view positions for lighting
            shader_setVec3(shader, "lightPos", lightPos.x, lightPos.y, lightPos.z);
            shader_setVec3(shader, "viewPos", global_camera->position.x, global_camera->position.y, global_camera->position.z);

            float physicsTimeStep = PHYSICS_TIMESTEP; // Simulate 1000 seconds per frame
            for (int i = 0; i < 5; i++) {
                priorObj = &SolarObjs[i];
                accelaerations[i] = (vec3_t){ 0.0f, 0.0f, 0.0f };

                for (int j = 0; j < 5; j++) {
                    currentObj = &SolarObjs[j];
                    if (priorObj != currentObj) {
                        vec3_t delta = {
                            currentObj->position.x - priorObj->position.x,
                            currentObj->position.y - priorObj->position.y,
                            currentObj->position.z - priorObj->position.z
                        };

                        double distance = sqrt((double)(delta.x * delta.x) + (double)(delta.y * delta.y) + (double)(delta.z * delta.z));

                        // Use minimum distance based on object radii to prevent singularity
                        double minDist = (double)(priorObj->radius + currentObj->radius) * 2.0;
                        if (distance < minDist) distance = minDist;

                        vec3_t direction = {
                            (float)(delta.x / distance),
                            (float)(delta.y / distance),
                            (float)(delta.z / distance)
                        };

                        // Use double precision for force calculation
                        double forceMagnitude = (G_SCALED * (double)currentObj->mass * (double)priorObj->mass) / (distance * distance);
                        double acclMagnitude = forceMagnitude / (double)priorObj->mass;

                        // Clamp acceleration to prevent numerical instability
                        if (acclMagnitude > MAX_ACCL) acclMagnitude = MAX_ACCL;

                        accelaerations[i].x += (float)(direction.x * acclMagnitude);
                        accelaerations[i].y += (float)(direction.y * acclMagnitude);
                        accelaerations[i].z += (float)(direction.z * acclMagnitude);
                      //  printf("Accel[%d]: (%.3e, %.3e, %.3e)\n", i, accelaerations[i].x, accelaerations[i].y, accelaerations[i].z);
                       //printf("\nACCL %f %f %f\n", accelaerations[i].x, accelaerations[i].y, accelaerations[i].z);
                    }
                }
                oldAccelerations[i] = accelaerations[i];
            }

            //printf("\nACCL2 %f %f %f\n", accelaerations[0].x, accelaerations[0].y, accelaerations[0].z);

            for (int i = 0; i < 5; i++) {
               priorObj = &SolarObjs[i];
    

               solor_obj_physics_update(priorObj, oldAccelerations[i], accelaerations[i], physicsTimeStep);
               //solar_obj_accelerate_vec(priorObj, accelaerations[i], physicsTimeStep);
               //solar_obj_update_position(priorObj, physicsTimeStep);
               //solar_obj_update_verts(priorObj);
               
              // solar_obj_accelerate_vec(priorObj, accelaerations[i], physicsTimeStep);
              // solar_obj_update_position(priorObj, physicsTimeStep);
               //solar_obj_update_verts(priorObj);
                for (int j = 0; j < 5; j++) {
                    currentObj = &SolarObjs[j];
                    if (currentObj != priorObj) {
                        float bounce = solar_obj_check_collision(priorObj, currentObj);
                        priorObj->velocity.x *= bounce;
                        priorObj->velocity.y *= bounce;
                        priorObj->velocity.z *= bounce;
                    }
                }
                glBindVertexArray(priorObj->VAO);
                mat4_identity(&model);
                model = mat4_translate(model, priorObj->position);
                shader_setmat4(shader, "model", &model);

                glDrawArrays(GL_TRIANGLES, 0, priorObj->vertex_count / 3);
     
            }
        

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                // increase mass by 1% per second
                priorObj->mass *= 1.0 + 1.0 * deltaTime;

                // update radius based on new mass
                priorObj->radius = pow(
                    (3 * priorObj->mass / priorObj->density) /
                    (4 * 3.14159265359f),
                    1.0f / 3.0f
                ) / SIZE_RATIO;

                // update vertex data
                solar_obj_update_verts(priorObj);
            }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}






void processInput(GLFWwindow* window, Camera* cam)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = (float)xposIn;
    float ypos = (float)yposIn;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera_ProcessMouseMovement(global_camera, xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera_ProcessMouseScroll(global_camera, (float)yoffset);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
