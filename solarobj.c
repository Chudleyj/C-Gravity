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

            // Triangle 1
            GLvec3_push_point(&verts, v1); GLvec3_push_point(&verts, n1); 
            GLvec3_push_point(&verts, v2); GLvec3_push_point(&verts, n2);  
            GLvec3_push_point(&verts, v3); GLvec3_push_point(&verts, n3);  

            // Triangle 2
            GLvec3_push_point(&verts, v2); GLvec3_push_point(&verts, n2);  
            GLvec3_push_point(&verts, v4); GLvec3_push_point(&verts, n4);  
            GLvec3_push_point(&verts, v3); GLvec3_push_point(&verts, n3);  
        }
    }
    return verts;
}

SolarSystem solar_obj_make_solar_system() {
   
    float AU_SCALE = 1.496e11f / 10.0f; // 1 AU = 10 OpenGL units
    
   vec3_t objPositions[] = {
        (vec3_t){0.0f, 0.0f, 0.0f},                                    // Sun at origin
        (vec3_t){5.791e10f * METER_TO_OPENGL, 0.0f, 0.0f},            // Mercury at 0.39 AU
        (vec3_t){1.082e11f * METER_TO_OPENGL, 0.0f, 0.0f},            // Venus at 0.72 AU
        (vec3_t){AU_IN_METERS * METER_TO_OPENGL, 0.0f, 0.0f},         // Earth at 1 AU
        (vec3_t){2.279e11f * METER_TO_OPENGL, 0.0f, 0.0f},            // Mars at 1.52 AU
        (vec3_t){7.785e11f * METER_TO_OPENGL, 0.0f, 0.0f},            // Jupiter at 5.2 AU
        (vec3_t){1.4335e12f * METER_TO_OPENGL, 0.0f, 0.0f},           // Saturn at 9.58 AU
        (vec3_t){2.87e12f * METER_TO_OPENGL, 0.0f, 0.0f},             // Uranus at 19.2 AU
        (vec3_t){4.495e12f * METER_TO_OPENGL, 0.0f, 0.0f}             // Neptune at 30.1 AU
    };

     vec3_t objStartVels[] = {
        (vec3_t){0.0f, 0.0f, 0.0f},                             // Sun (no velocity)     
        (vec3_t){0.0f, 47360.0f * METER_TO_OPENGL, 0.0f},      // Mercury
        (vec3_t){0.0f, 35020.0f * METER_TO_OPENGL, 0.0f},      // Venus
        (vec3_t){0.0f, 29783.6f * METER_TO_OPENGL, 0.0f},      // Earth
        (vec3_t){0.0f, 24077.0f * METER_TO_OPENGL, 0.0f},      // Mars  
        (vec3_t){0.0f, 13070.0f * METER_TO_OPENGL, 0.0f},      // Jupiter
        (vec3_t){0.0f, 9690.0f * METER_TO_OPENGL, 0.0f},       // Saturn 
        (vec3_t){0.0f, 6800.0f * METER_TO_OPENGL, 0.0f},       // Uranus
        (vec3_t){0.0f, 5430.0f * METER_TO_OPENGL, 0.0f}        // Neptune
    };
   // Densities in kg/m³ (order: Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune)
   float objDensities[] = {
       1410.0f, 5430.0f, 5240.0f, 5515.0f, 3930.0f, 1330.0f, 687.0f, 1270.0f, 1640.0f
   };


   float objMasses[] = { 1.989e30f, 5.972e24f, 6.39e23f, 4.867e24f, 3.285e23f, 1.898e27f, 5.683e26f, 8.681e25f, 1.024e26f };

   // Visual scaling factors (order: Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune)
   float scale[] = {
        20.0f,   // Sun (make it visible but not huge)
        750.0f, // Mercury (tiny, needs big scaling)
        1500.0f, // Venus
        1000.0f, // Earth  
        2500.0f, // Mars
        70.0f,   // Jupiter (big planet, less scaling needed)
        90.0f,   // Saturn
        200.0f,   // Uranus
        200.0f    // Neptune
   };

   float glColorsScale = 255.0f; //Hex rgb 0-255, gl 0-1, so scale
   colorVec_t colors[] = {
        (colorVec_t){255.0f / glColorsScale, 204.0f/glColorsScale, 51.0f/glColorsScale, 1.0f}, //Sun 
        (colorVec_t){177.0f/ glColorsScale, 177.0f/ glColorsScale, 173.0f / glColorsScale, 1.0f}, //Mercury 
        (colorVec_t){193.0f/ glColorsScale, 143.0f/ glColorsScale, 23.0f / glColorsScale,   1.0f}, //Venus
        (colorVec_t){107.0f/ glColorsScale, 147.0f/ glColorsScale, 214.0f / glColorsScale,  1.0f}, //Earth
        (colorVec_t){193.0f/ glColorsScale, 68.0f/ glColorsScale, 14.0f / glColorsScale,    1.0f}, //Mars
        (colorVec_t){216.0f/ glColorsScale, 202.0f/ glColorsScale, 157.0f / glColorsScale,  1.0f}, //Jupiter
        (colorVec_t){234.0f/ glColorsScale, 214.0f/ glColorsScale, 192.0f / glColorsScale,  1.0f}, //Saturn
        (colorVec_t){172.0f/ glColorsScale, 229.0f/ glColorsScale, 238.0f / glColorsScale,  1.0f}, //Uranus
        (colorVec_t){91.0f/ glColorsScale, 93.0f/ glColorsScale, 223.0f / glColorsScale,  1.0f} //Neptune
   };


    SolarSystem sol;
    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        solar_obj_init(&sol.objs[i], objPositions[i], objStartVels[i], colors[i], objMasses[i], objDensities[i], scale[i]);
    }

    return sol; 
}


void solar_obj_init(SolarObj* obj, vec3_t startPos, vec3_t startVel, colorVec_t color, float mass, float density, float scale) {
    obj->position = startPos;
    obj->velocity = startVel;
    obj->mass = mass;
    obj->density = density;
    obj->scale = scale;
    obj->color = color; 
    float volume = mass / density; // m^3
    float radius_meters = powf((3.0f * volume) / (4.0f * PI), 1.0f / 3.0f);

    obj->radius = radius_meters * METER_TO_OPENGL * obj->scale; // convert radius to OpenGL units directly

    GLvec3_t vertices = solar_obj_draw_sphere(obj->radius);
    obj->vertex_count = vertices.size;

    solar_obj_init_VAO_VBO(&(obj->VAO), &(obj->VBO), vertices.data, obj->vertex_count);
}

void solar_obj_update_position(SolarObj* obj, float dTime) {
    obj->position.x += obj->velocity.x * dTime; 
    obj->position.y += obj->velocity.y * dTime;
    obj->position.z += obj->velocity.z * dTime;

    float volume = obj->mass / obj->density; // m^3
    float radius_meters = powf((3.0f * volume) / (4.0f * PI), 1.0f / 3.0f);

    obj->radius = radius_meters * METER_TO_OPENGL * obj->scale; 
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

    obj->radius = radius_meters * METER_TO_OPENGL * obj->scale;
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


vec3_t solar_obj_log_scale_positions(const vec3_t realPosition) {
    float distance = sqrt(realPosition.x * realPosition.x + realPosition.y * realPosition.y + realPosition.z * realPosition.z);

    if (distance < 1e-6f) return realPosition; // Handle origin (Sun)

    // Logarithmic scaling: compress distances beyond X AU
    float scaleFactor;
    float AU = AU_IN_METERS * METER_TO_OPENGL; 


    float compressionStart = 3.0f * AU;     // Start compressing after X AU 
    float compressionFactor = 0.6f;         // Lower = more compression (0.5 = very compressed, 2.0 = less compressed)
    float maxDisplayDistance = 6.0f * AU;   // Maximum distance any planet will appear at

    if (distance <= compressionStart) {
        // Inner planets: no compression
        scaleFactor = 1.0f;
    }
    else {
        // Outer planets: logarithmic compression
        float beyondStart = distance / compressionStart;
        float compressedDistance = compressionStart + log10f(beyondStart) * compressionFactor * AU;

        // Cap the maximum distance
        if (compressedDistance > maxDisplayDistance) {
            compressedDistance = maxDisplayDistance;
        }

        scaleFactor = compressedDistance / distance;
    }

    return (vec3_t) {
        realPosition.x* scaleFactor,
            realPosition.y* scaleFactor,
            realPosition.z* scaleFactor
    };
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

