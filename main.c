#include "shader.h"
#include "camera.h"
#include "solarsystem.h"


float deltaTime = 0.0f;


Camera *global_camera = NULL; 

bool firstMouse = true;

void processInput(GLFWwindow* window, Camera* cam);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void updateCamera(Shader shader);
void drawToScreen(SolarSystem *s, vec3_t* accl, vec3_t* oldAccl, Shader shader, float ts);

int main() {
    Camera camera = camera_init_vectors((vec3_t) { 0.0f, 50.0f, 500.0f },(vec3_t){ 0.0f, 1.0f, 1.0f }, YAW, PITCH);
    global_camera = &camera; 

    float lastFrame = 0.0f;

    if (!glfwInit()) return -1;
    atexit(glfwTerminate);

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
    solor_system_physics_update(&sol, PHYSICS_TIMESTEP);

    Shader shader = shader_create("vertex.glsl", "fragment.glsl");
    vec3_t lightPos = (vec3_t){ 1.2f, 1.0f, 2.0f };
    mat4_t projection;
    vec3_t accelaerations[NUM_SOLAR_OBJS];
    vec3_t oldAccelerations[NUM_SOLAR_OBJS];
  
    while (!glfwWindowShouldClose(window)) {
            processInput(window, global_camera);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            shader_use(shader);

            updateCamera(shader); 

            mat4_perspective(&projection, degrees_to_radians(global_camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            shader_setmat4(shader, "projection", &projection);
            shader_setVec3(shader, "viewPos", global_camera->position.x, global_camera->position.y, global_camera->position.z);

            // Set light and view positions for lighting
            shader_setVec3(shader, "lightPos", lightPos.x, lightPos.y, lightPos.z);
            //make the light move:
            lightPos.x = 50.0f * cos(currentFrame);
            lightPos.z = 50.0f * sin(currentFrame);
            lightPos.y = 20.0f;

            float physicsTimeStep = PHYSICS_TIMESTEP; 
          //physicsTimeStep = deltaTime * 86400.0f
  
            drawToScreen(&sol, accelaerations, oldAccelerations, shader, physicsTimeStep);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    return 0;
}

void drawToScreen(SolarSystem *s, vec3_t *accl, vec3_t *oldAccl, Shader shader, float ts) {
    GLuint colorVecLocation = glGetUniformLocation(shader.ID, "objColor");
    GLuint emitLightLocation = glGetUniformLocation(shader.ID, "emitLight");
    mat4_t model;
    SolarObj* compareObj = &s->objs[0];
    SolarObj* currentObj = &s->objs[0];
    solor_system_physics_update(s, ts);
    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        currentObj = &s->objs[i];

        

        for (int j = 0; j < NUM_SOLAR_OBJS; j++) {
            compareObj = &s->objs[j];
            if (currentObj != compareObj) {
                if (compareObj != currentObj) {
                   float bounce = solar_obj_check_collision(currentObj, compareObj);
                    currentObj->velocity.x *= bounce;
                    currentObj->velocity.y *= bounce;
                    currentObj->velocity.z *= bounce;
                }
            }
        }
        //its the sun, emit light instead of reflect it
        if (i == 0) {
            glUniform1i(emitLightLocation, 1);
        }
        else {
            glUniform1i(emitLightLocation, 0);
        }

        glUniform4f(colorVecLocation, currentObj->color.r, currentObj->color.g, currentObj->color.b, currentObj->color.a);
        glBindVertexArray(currentObj->VAO);
        mat4_identity(&model);
        vec3d_t scaledPos = solar_obj_log_scale_positions(currentObj->position);
        if (i == MOON) {
           
            vec3d_t moonRelToEarth; 
            vec3d_subtract(currentObj->position, s->objs[EARTH].position, &moonRelToEarth);
            float moonVisScale = 40.0f;
            moonRelToEarth = (vec3d_t){ moonRelToEarth.x * moonVisScale,  moonRelToEarth.y * moonVisScale,  moonRelToEarth.z * moonVisScale };
            vec3d_t moonRenderPos;
            vec3d_add(s->objs[EARTH].position, moonRelToEarth, &moonRenderPos); 
            model = mat4_dtranslate(model, moonRenderPos);
            shader_setmat4(shader, "model", &model);
        }
        else {
            model = mat4_translate(model, (vec3_t){ (float)scaledPos.x, (float)scaledPos.y, (float)scaledPos.z });
            shader_setmat4(shader, "model", &model);
        }


        glDrawArrays(GL_TRIANGLES, 0, currentObj->vertex_count / 3);

    }

}

void updateCamera(Shader shader) {
    mat4_t view = camera_get_view_matrix(*global_camera);
    shader_setmat4(shader, "view", &view);
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
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;

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
