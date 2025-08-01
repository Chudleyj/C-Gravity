#pragma once
#include "vectors.h"
#include "utils.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SIZE_RATIO 1000000000.0f    
#define METER_TO_OPENGL 1e-9f  // 1 OpenGL unit = 1 billion meters = 1 million km
#define AU_IN_METERS 1.496e11f // 1 AU in meters
#define NUM_SOLAR_OBJS 10 //TODO: Really shouldnt be a hard define, this should be variable...


typedef enum {
    SUN = 0, 
    MERCURY = 1, 
    VENUS = 2, 
    EARTH = 3, 
    MARS = 4, 
    JUPITER = 5, 
    SATURN = 6, 
    URANUS = 7, 
    NEPTUNE = 8,
    MOON = 9
}SolarObjNames;


typedef struct {
    GLuint VAO, VBO;
    float mass;
    float density;
    float radius;
    float scale;
    vec3d_t accleration; 
    vec3d_t priorAccleration;
    vec3d_t position;
    vec3d_t velocity;
    colorVec_t color; 
    size_t vertex_count;
}SolarObj;

GLvec3_t solar_obj_draw_sphere(float radius);
void solar_obj_init(SolarObj* obj, vec3d_t startPos, vec3d_t startVel, colorVec_t color, float mass, float density, float scale);
float solar_obj_check_collision(const SolarObj *obj1, const SolarObj *obj2);
vec3d_t solar_obj_log_scale_positions(const vec3d_t realPosition);
static void solar_obj_init_VAO_VBO(GLuint* VAO, GLuint* VBO, const float* vertices, size_t vertexCount);
