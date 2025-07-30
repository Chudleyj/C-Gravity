#include "solarobj.h"

//theta goes from 0 to 180 deg (longitude of sphere)
//phi goes from 0 to 360 (Latitude) 
//iterating over long and lat of a circle, we get 4 values: (theta+1, phi), (theta+1, phi+1), (theta, phi), (theta, phi + 1)
//these 4 point coordinates = a rect on the face of a sphere (aka 2 triangles) 
//do this for every point, and boom we got a sphere
GLvec3_t solar_obj_draw_sphere(float radius) {
    GLvec3_t verts;
    GLvec3_init(&verts);

    int stacks = 10;
    int sectors = 50;

 




    for (float i = 0.0f; i <= stacks; i++) {
        float theta1 = (i / stacks) * PI;
        float theta2 = ((i + 1) / stacks) * PI;
        for (float j = 0.0f; j < sectors; j++) {
            float phi1 = j / sectors * 2 * PI;
            float phi2 = (j + 1) / sectors * 2 * PI;
            vec3_t v1 = sphericalToCartesian(radius, theta1, phi1);
            vec3_t v2 = sphericalToCartesian(radius, theta1, phi2);
            vec3_t v3 = sphericalToCartesian(radius, theta2, phi1);
            vec3_t v4 = sphericalToCartesian(radius, theta2, phi2);

            vec3_t n1 = v1; vec3_normalize(&n1);
            vec3_t n2 = v2; vec3_normalize(&n2);
            vec3_t n3 = v3; vec3_normalize(&n3);
            vec3_t n4 = v4; vec3_normalize(&n4);

            // Triangle 1: v1, v2, v3
            GLvec3_push_point(&verts, v1); GLvec3_push_point(&verts, n1); 
            GLvec3_push_point(&verts, v2); GLvec3_push_point(&verts, n2);  
            GLvec3_push_point(&verts, v3); GLvec3_push_point(&verts, n3);  


            // Triangle 2: v2, v4, v3
            GLvec3_push_point(&verts, v2); GLvec3_push_point(&verts, n2);  
            GLvec3_push_point(&verts, v4); GLvec3_push_point(&verts, n4);  
            GLvec3_push_point(&verts, v3); GLvec3_push_point(&verts, n3);  
        }
    }
    return verts;
}

void solar_obj_init(SolarObj* obj, vec3_t startPos, vec3_t startVel, float mass, float density) {
    obj->position = startPos;
    obj->velocity = startVel;
    obj->mass = mass;
    obj->density = density;

    float volume = mass / density; // m^3
    float radius_meters = powf((3.0f * volume) / (4.0f * PI), 1.0f / 3.0f);

    obj->radius = radius_meters * METER_TO_OPENGL; // convert radius to OpenGL units directly
    // Scale planets for visibility (keep sun normal size)
    if (obj->mass < 1e29f) { // If not the sun
        obj->radius *= 750.0f; // Make planets 1000x larger for visibility
    }
    else {
        obj->radius *= 20.0f;
    }
    // No minimum radius needed if camera & scale are set well

    GLvec3_t vertices = solar_obj_draw_sphere(obj->radius);
    obj->vertex_count = vertices.size;

    solar_obj_init_VAO_VBO(&(obj->VAO), &(obj->VBO), vertices.data, obj->vertex_count);
}


/*
void solar_obj_init(SolarObj* obj, vec3_t startPos, vec3_t startVel, float mass, float density) {
    obj->position = startPos;
    obj->velocity = startVel;
    obj->mass = mass;
    obj->density = density;
    float volume = mass / density;
    obj->radius = pow(3 * volume / (4 * PI), 1.0f / 3.0f) / SIZE_RATIO;
    // TEMPORARY: Set minimum radius so all objects are visible
   // obj->radius = fmaxf(tradius, 0.2f); // Minimum 0.01 units radius
   
    GLvec3_t verticies = solar_obj_draw_sphere(obj->radius);
    obj->vertex_count = verticies.size;

    solar_obj_init_VAO_VBO(&(obj->VAO), &(obj->VBO), verticies.data, obj->vertex_count);
}
*/
void solar_obj_update_position(SolarObj* obj, float dTime) {
    obj->position.x += obj->velocity.x * dTime; 
    obj->position.y += obj->velocity.y * dTime;
    obj->position.z += obj->velocity.z * dTime;

    float volume = obj->mass / obj->density; // m^3
    float radius_meters = powf((3.0f * volume) / (4.0f * PI), 1.0f / 3.0f);

    obj->radius = radius_meters * METER_TO_OPENGL; 
    // Scale planets for visibility (keep sun normal size)
    if (obj->mass < 1e29f) { // If not the sun
        obj->radius *= 1000.0f; // Make planets 1000x larger for visibility
    }


}

void solar_obj_update_verts(SolarObj* obj) {
    GLvec3_t verticies = solar_obj_draw_sphere(obj->radius); 
    obj->vertex_count = verticies.size;
 
    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->vertex_count * sizeof(float), verticies.data, GL_STATIC_DRAW);
}

void solar_obj_accelerate(SolarObj* obj, const float x, const float y, const float z, const float dTime) {
    obj->velocity.x += x * dTime; 
    obj->velocity.y += y * dTime;
    obj->velocity.z += z * dTime;
}


void solar_obj_accelerate_vec(SolarObj* obj, const vec3_t v,  const float dTime) {
    obj->velocity.x += v.x * dTime;
    obj->velocity.y += v.y * dTime;
    obj->velocity.z += v.z * dTime;
}


void solor_obj_physics_update(SolarObj* obj, const vec3_t oldAccl, const vec3_t newAccl, const float dTime) {
    obj->position.x += obj->velocity.x * dTime + 0.5f * oldAccl.x * dTime * dTime;
    obj->position.y += obj->velocity.y * dTime + 0.5f * oldAccl.y * dTime * dTime;
    obj->position.z += obj->velocity.z * dTime + 0.5f * oldAccl.z * dTime * dTime;

    obj->velocity.x += 0.5f * (oldAccl.x + newAccl.x) * dTime;
    obj->velocity.y += 0.5f * (oldAccl.y + newAccl.y) * dTime;
    obj->velocity.z += 0.5f * (oldAccl.z + newAccl.z) * dTime;

    float volume = obj->mass / obj->density; // m^3
    float radius_meters = powf((3.0f * volume) / (4.0f * PI), 1.0f / 3.0f);

    obj->radius = radius_meters * METER_TO_OPENGL;
    // Scale planets for visibility (keep sun normal size)
    if (obj->mass < 1e29f) { // If not the sun
        obj->radius *= 1000.0f; // Make planets 1000x larger for visibility
    }
}

float solar_obj_check_collision(const SolarObj *obj1, const SolarObj *obj2) {
    float dx = obj2->position.x - obj1->position.x;
    float dy = obj2->position.y - obj1->position.y;
    float dz = obj2->position.z - obj1->position.z;

    float distance = sqrt(dx * dx + dy * dy + dz * dz);
    if (obj2->radius + obj1->radius > distance) {
        return -0.2f;
    }
    return 1.0f;
}

static void solar_obj_init_VAO_VBO(GLuint* VAO, GLuint* VBO, const float* vertices, size_t vertexCount) {
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);
}

