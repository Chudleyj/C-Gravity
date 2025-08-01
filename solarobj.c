#include "solarobj.h"

//theta goes from 0 to 180 deg (longitude of sphere)
//phi goes from 0 to 360 (Latitude) 
//iterating over long and lat of a circle, we get 4 values: (theta+1, phi), (theta+1, phi+1), (theta, phi), (theta, phi + 1)
//these 4 point coordinates = a rect on the face of a sphere (aka 2 triangles) 
//do this for every point, and boom we got a sphere
GLvec3_t solar_obj_draw_sphere(float radius) {
    vector_result_t vecRes;

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
            vec3_t v1 = vec3_sphericalToCartesian(radius, theta1, phi1);
            vec3_t v2 = vec3_sphericalToCartesian(radius, theta1, phi2);
            vec3_t v3 = vec3_sphericalToCartesian(radius, theta2, phi1);
            vec3_t v4 = vec3_sphericalToCartesian(radius, theta2, phi2);

            vec3_t n1 = v1; 
      
            vecRes = vec3_normalize(&n1); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
      

            vec3_t n2 = v2; 
            vecRes = vec3_normalize(&n2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
           

            vec3_t n3 = v3; 
            vecRes = vec3_normalize(&n3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            

            vec3_t n4 = v4; 
            vecRes = vec3_normalize(&n4); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
    

            // Triangle 1
            vecRes = GLvec3_push_point(&verts, v1); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n1); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

            vecRes = GLvec3_push_point(&verts, v2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

            vecRes = GLvec3_push_point(&verts, v3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

            // Triangle 2
            vecRes = GLvec3_push_point(&verts, v2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

            vecRes = GLvec3_push_point(&verts, v4); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n4); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

            vecRes = GLvec3_push_point(&verts, v3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
        }
    }
    return verts;
}

void solar_obj_init(SolarObj* obj, vec3d_t startPos, vec3d_t startVel, colorVec_t color, float mass, float density, float scale) {
    obj->position = startPos;
    obj->velocity = startVel;
    obj->mass = mass;
    obj->density = density;
    obj->scale = scale;
    obj->color = color;
    obj->accleration = (vec3d_t){ 0.0f };
    obj->priorAccleration = (vec3d_t){ 0.0f };
    float volume = mass / density; // m^3
    float radius_meters = powf((3.0f * volume) / (4.0f * PI), 1.0f / 3.0f);

    obj->radius = radius_meters * METER_TO_OPENGL * obj->scale; // convert radius to OpenGL units directly

    GLvec3_t verticies = solar_obj_draw_sphere(obj->radius);
    obj->vertex_count = verticies.size;

    solar_obj_init_VAO_VBO(&(obj->VAO), &(obj->VBO), verticies.data, obj->vertex_count);


    vector_result_t vecRes = GLvec3_free(&verticies); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

}


float solar_obj_check_collision(const SolarObj *obj1, const SolarObj *obj2) {
    float dx = obj2->position.x - obj1->position.x;
    float dy = obj2->position.y - obj1->position.y;
    float dz = obj2->position.z - obj1->position.z;

    float distance = sqrt(dx * dx + dy * dy + dz * dz);
    if ((obj2->radius/obj2->scale) + (obj1->radius/obj1->scale) > distance) {
        return -0.2f;
    }
    return 1.0f;
}

vec3d_t solar_obj_log_scale_positions(const vec3d_t realPosition) {
    double distance = sqrt(realPosition.x * realPosition.x + realPosition.y * realPosition.y + realPosition.z * realPosition.z);

    if (distance < 1e-6 || distance < 0.01) return realPosition; // Handle origin (Sun)

    // Logarithmic scaling: compress distances beyond X AU
    double scaleFactor;
    double AU = (double)AU_IN_METERS * (double)METER_TO_OPENGL;


    double compressionStart = 3.0 * AU;     // Start compressing after X AU 
    double compressionFactor = 0.6;         // Lower = more compression 
    double maxDisplayDistance = 5.0 * AU;

    if (distance <= compressionStart) {
        // Inner planets: no compression
        scaleFactor = 1.0;
    }
    else {
        // Outer planets: logarithmic compression
        double beyondStart = distance / compressionStart;
        double compressedDistance = compressionStart + log10(beyondStart) * compressionFactor * AU;

        // Cap the maximum distance
        if (compressedDistance > maxDisplayDistance) {
            compressedDistance = maxDisplayDistance;
        }

        scaleFactor = compressedDistance / distance;
    }

    return (vec3d_t) {
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

