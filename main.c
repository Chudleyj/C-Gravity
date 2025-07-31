#include "shader.h"
#include "camera.h"
#include "solarobj.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define G 6.6743e-11 // m^3 kg^-1 s^-2
#define G_SCALED (G * METER_TO_OPENGL * METER_TO_OPENGL * METER_TO_OPENGL)
#define PHYSICS_TIMESTEP 1500.0f


#define MAX_ACCL 1e-3f //1e10f?

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;


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
    
    SolarSystem sol = solar_obj_make_solar_system(); 

    Shader shader = shader_create("vertex.glsl", "fragment.glsl");
    vec3_t lightPos = (vec3_t){ 1.2f, 1.0f, 2.0f };
    mat4_t projection;
    mat4_t model;

    SolarObj *compareObj = &sol.objs[0];
    SolarObj *currentObj = &sol.objs[0];
    vec3_t accelaerations[NUM_SOLAR_OBJS];
    vec3_t oldAccelerations[NUM_SOLAR_OBJS];
    GLuint colorVecLocation = glGetUniformLocation(shader.ID, "objColor");
    GLuint emitLightLocation = glGetUniformLocation(shader.ID, "emitLight");
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

   
            shader_setVec3(shader, "viewPos", global_camera->position.x, global_camera->position.y, global_camera->position.z);

            // Set light and view positions for lighting
            shader_setVec3(shader, "lightPos", lightPos.x, lightPos.y, lightPos.z);
            //make the light move:
            lightPos.x = 50.0f * cos(currentFrame);
            lightPos.z = 50.0f * sin(currentFrame);
            lightPos.y = 20.0f;
            float physicsTimeStep = PHYSICS_TIMESTEP; 
           // physicsTimeStep = deltaTime * 86400.0f;
            for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
                currentObj = &sol.objs[i];
                accelaerations[i] = (vec3_t){ 0.0f, 0.0f, 0.0f };

                for (int j = 0; j < NUM_SOLAR_OBJS; j++) {
                    compareObj = &sol.objs[j];
                    if (currentObj != compareObj) {
                        vec3_t delta = {
                            compareObj->position.x - currentObj->position.x,
                            compareObj->position.y - currentObj->position.y,
                            compareObj->position.z - currentObj->position.z
                        };

                        double distance = sqrt((double)(delta.x * delta.x) + (double)(delta.y * delta.y) + (double)(delta.z * delta.z));

                        // Use minimum distance based on object radii to prevent singularity
                        double minDist = (double)(currentObj->radius + compareObj->radius) * 2.0;
                        if (distance < minDist) distance = minDist;

                        vec3_t direction = {
                            (float)(delta.x / distance),
                            (float)(delta.y / distance),
                            (float)(delta.z / distance)
                        };

                        // Use double precision for force calculation
                        double forceMagnitude = (G_SCALED * (double)compareObj->mass * (double)currentObj->mass) / (distance * distance);
                        double acclMagnitude = forceMagnitude / (double)currentObj->mass;

                        // Clamp acceleration to prevent numerical instability
                        if (acclMagnitude > MAX_ACCL) acclMagnitude = MAX_ACCL;

                        accelaerations[i].x += (float)(direction.x * acclMagnitude);
                        accelaerations[i].y += (float)(direction.y * acclMagnitude);
                        accelaerations[i].z += (float)(direction.z * acclMagnitude);
                    }
                }
                oldAccelerations[i] = accelaerations[i];
            }

            for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
                currentObj = &sol.objs[i];
    
               solor_obj_physics_update(currentObj, oldAccelerations[i], accelaerations[i], physicsTimeStep);
     
                for (int j = 0; j < NUM_SOLAR_OBJS; j++) {
                    compareObj = &sol.objs[j];
                    if (compareObj != currentObj) {
                        float bounce = solar_obj_check_collision(currentObj, compareObj);
                        currentObj->velocity.x *= bounce;
                        currentObj->velocity.y *= bounce;
                        currentObj->velocity.z *= bounce;
                    }
                }
                //its the sun, emit light instead of reflect it
                if (i == 0) {
                    glUniform1i(emitLightLocation, 1);
                }
                else{
                    glUniform1i(emitLightLocation, 0);
                }
                
                glUniform4f(colorVecLocation, currentObj->color.r, currentObj->color.g, currentObj->color.b, currentObj->color.a);
                glBindVertexArray(currentObj->VAO);
                mat4_identity(&model);
                vec3_t scaledPos = solar_obj_log_scale_positions(currentObj->position);
                model = mat4_translate(model, scaledPos);
                shader_setmat4(shader, "model", &model);

                glDrawArrays(GL_TRIANGLES, 0, currentObj->vertex_count / 3);
     
            }
        
/*
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
   */     
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
