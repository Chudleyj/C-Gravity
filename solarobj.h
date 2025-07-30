#pragma once
#include "vectors.h"
#include "utils.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SIZE_RATIO 1000000000.0f    
#define METER_TO_OPENGL 1e-9f  // 1 OpenGL unit = 1 billion meters = 1 million km
#define AU_IN_METERS 1.496e11f // 1 AU in meters

typedef struct {
    GLuint VAO, VBO;
    float mass;
    float density;
    float radius;
    vec3_t position;
    vec3_t velocity;
    size_t vertex_count;
}SolarObj;

GLvec3_t solar_obj_draw_sphere(float radius);
void solar_obj_init(SolarObj* obj, vec3_t startPos, vec3_t startVel, float mass, float density);
void solar_obj_update_position(SolarObj* obj, float dTime);
void solar_obj_accelerate(SolarObj* obj, const float x, const float y, const float z, const float dTime);
void solar_obj_accelerate_vec(SolarObj* obj, const vec3_t v, const float dTime);
void solar_obj_update_verts(SolarObj* obj);
float solar_obj_check_collision(const SolarObj *obj1, const SolarObj *obj2);
void solor_obj_physics_update(SolarObj* obj, const vec3_t oldAccl, const vec3_t newAccl, const float dTime);
static void solar_obj_init_VAO_VBO(GLuint* VAO, GLuint* VBO, const float* vertices, size_t vertexCount);
