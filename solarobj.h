#pragma once
#include "stb_image.h"
#include "vectors.h"
#include "utils.h"
#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

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
    int parent_planet_id;
    double distance_from_parent;  // in meters
    double orbital_speed_factor;  // multiplier for calculated orbital speed
    float mass;
    float density;
    float scale;
    colorVec_t color;
    const char* name;
} MoonData;


static const MoonData moon_configs[] = {
 
    {EARTH, 3.84e8, 1.0, 7.3e22f, 3340.0f, 470.0f,
     {1.0f, 1.0f, 1.0f, 1.0f}, "Luna"},

     {MARS, 9.376e6, 1.1, 1.0659e16f, 1876.0f, 200000.0f,
      {0.4f, 0.4f, 0.4f, 1.0f}, "Phobos"},
     {MARS, 2.3463e7, 1.0, 1.4762e15f, 1471.0f, 200000.0f,
      {0.3f, 0.3f, 0.3f, 1.0f}, "Deimos"},

      // Jupiter's major moons only atm
      {JUPITER, 4.217e8, 1.0, 8.9319e22f, 3528.0f, 500.0f,
       {1.0f, 1.0f, 0.8f, 1.0f}, "Io"},
      {JUPITER, 6.709e8, 1.0, 4.7998e22f, 3013.0f, 550.0f,
       {0.9f, 0.9f, 0.7f, 1.0f}, "Europa"},
};
#define NUM_MOONS (sizeof(moon_configs) / sizeof(MoonData)) //TODO: Gross placement & I don't think this should be a hard define...
#define TOTAL_OBJECTS (NUM_SOLAR_OBJS + NUM_MOONS) //TODO:  Really shouldnt be a hard define, this should be variable...

typedef struct {
    GLuint VAO, VBO;
    float mass;
    float density;
    float radius;
    float scale;
    float rotationSpeed; 
    vec3d_t accleration; 
    vec3d_t priorAccleration;
    vec3d_t position;
    vec3d_t velocity;
    colorVec_t color; 
    size_t vertex_count;
    size_t moon_count; 
    int id; 
    int parent_id; //-1 if not a moon, else a SolarObjNames enum value 
    bool is_moon;
    unsigned int texture; 
}SolarObj;

GLvec3_t solar_obj_draw_sphere(float radius);
unsigned int solar_obj_get_texture(int id);
void solar_obj_init_moon(SolarObj* moon, const SolarObj* parent, const MoonData* moon_data);
void solar_obj_init(SolarObj* obj, vec3d_t startPos, vec3d_t startVel, colorVec_t color, float mass, float density, float scale, int id, float rotSpeed);
double solar_obj_check_collision(const SolarObj *obj1, const SolarObj *obj2);
vec3d_t solar_obj_log_scale_positions(const vec3d_t realPosition);
static void solar_obj_init_VAO_VBO(GLuint* VAO, GLuint* VBO, const float* vertices, size_t vertexCount);
















































































