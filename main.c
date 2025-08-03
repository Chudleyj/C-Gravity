#include "shader.h"
#include "camera.h"
#include "solarsystem.h"

void processInput(GLFWwindow* window, Camera* cam, float dt);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void updateCamera(Shader shader);
void drawToScreen(SolarSystem *s, Shader shader);

Camera* global_camera = NULL;
bool firstMouse = true;

int main() {

    Camera camera = camera_init_vectors((vec3_t) { 0.0f, 50.0f, 500.0f },(vec3_t){ 0.0f, 1.0f, 1.0f }, YAW, PITCH);
    global_camera = &camera; 


    if (!glfwInit()) return -1;
    atexit(glfwTerminate);

    GLFWwindow* window = glfwCreateWindow(2560, 1440, "C-Gravity", glfwGetPrimaryMonitor(), NULL);
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    SolarSystem sol = solar_system_init();
  
    Shader shader = shader_create("vertex.glsl", "fragment.glsl");
    vec3_t lightPos = (vec3_t){ 1.2f, 1.0f, 2.0f };
    mat4_t projection;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    double simTime = 0.0; 
    double physStep = 86400.0 /3; //one day in sec = 86400

    while (!glfwWindowShouldClose(window)) {
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            deltaTime *= 50;
            processInput(window, global_camera, deltaTime);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            shader_use(shader);

            updateCamera(shader); 

            mat4_perspective(&projection, degrees_to_radians(global_camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            shader_setmat4(shader, "projection", &projection);
            shader_setVec3(shader, "viewPos", global_camera->position.x, global_camera->position.y, global_camera->position.z);
            shader_setVec3(shader, "lightPos", lightPos.x, lightPos.y, lightPos.z);
            //make the light move:
            lightPos.x = 50.0f * cos(currentFrame);
            lightPos.z = 50.0f * sin(currentFrame);
            lightPos.y = 20.0f;

          
            //Use RK45 Method to caluclate gravity over time
            sol = rk45(sol, simTime, simTime + physStep, 0.01, (vec3d_t) { 1e-3, 1e-3, 1e-3 }, (vec3d_t) { 1e-3, 1e-3, 1e-3 }, (vec3d_t) { 1e-9, 1e-9, 1e-9 }, (vec3d_t) { 1e-9, 1e-9, 1e-9 });
            
            drawToScreen(&sol, shader);
        
            glfwSwapBuffers(window);
            glfwPollEvents();
            simTime += physStep; 
    }
    free(sol.objs); 
    glfwDestroyWindow(window);
    return 0;
}

void drawToScreen(SolarSystem* s, Shader shader) {
    GLuint colorVecLocation = glGetUniformLocation(shader.ID, "objColor");
    GLuint emitLightLocation = glGetUniformLocation(shader.ID, "emitLight");
    mat4_t model;
    SolarObj* compareObj = &s->objs[0];
    SolarObj* currentObj = &s->objs[0];

    //Loop over the solar system, draw every object in the system at its updated positions
     for (int i = 0; i < s->total_count; i++) {
        currentObj = &s->objs[i];

        //Loop over the solar system again, compare each object against the current obj of the outer loop, check for collison 
        // (in our current sim, this really shouldnt come up, but there is a CHANCE with moons on some faster speeds I use during testing so we leave it for now)
        for (int j = 0; j < s->total_count; j++) {
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
        if (currentObj->is_moon) {
            // Special Moon positioning relative to its parent planet
            SolarObj* parent = &s->objs[currentObj->parent_id];
            
            vec3d_t moonRelToParent; 
            vec3d_subtract(currentObj->position, parent->position, &moonRelToParent);
            
            float moonVisScale = 40.0f;
            //Mar's moons are so tiny we gotta shoot them out even more visually. 
            if (currentObj->parent_id == MARS) {
                moonVisScale *= 20.0f;
            }
            moonRelToParent = (vec3d_t){ 
                moonRelToParent.x * moonVisScale,  
                moonRelToParent.y * moonVisScale,  
                moonRelToParent.z * moonVisScale 
            };
            
            vec3d_t parentScaledPos = solar_obj_log_scale_positions(parent->position);
            vec3d_t moonRenderPos;
            vec3d_add(parentScaledPos, moonRelToParent, &moonRenderPos); 
            
            model = mat4_dtranslate(model, moonRenderPos);
        }
        else {
            vec3d_t scaledPos = solar_obj_log_scale_positions(currentObj->position);
            model = mat4_translate(model, (vec3_t){ (float)scaledPos.x, (float)scaledPos.y, (float)scaledPos.z });
            
        }

        shader_setmat4(shader, "model", &model);
        glDrawArrays(GL_TRIANGLES, 0, currentObj->vertex_count / 3);

    }

}

void updateCamera(Shader shader) {
    mat4_t view = camera_get_view_matrix(*global_camera);
    shader_setmat4(shader, "view", &view);
}

void processInput(GLFWwindow* window, Camera* cam, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, FORWARD, dt);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, RIGHT, dt);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, UP, dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_ProcessKeyboard(cam, DOWN, dt);
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
