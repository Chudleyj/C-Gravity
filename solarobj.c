#include "solarobj.h"

//theta goes from 0 to 180 deg (longitude of sphere)
//phi goes from 0 to 360 (Latitude) 
//iterating over long and lat of a circle, we get 4 values: (theta+1, phi), (theta+1, phi+1), (theta, phi), (theta, phi + 1)
//these 4 point coordinates = a rect on the face of a sphere (aka 2 triangles) 
//do this for every point, and boom we got a sphere
// Special thank you to kavan010 https://github.com/kavan010/gravity_sim/tree/main for teaching me how the heck to draw a sphere in OpenGL (https://www.youtube.com/watch?v=_YbGWoUaZg0), this project was very stuck until I found that. 
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

            //Texture coords
            float U1 = phi1/(2*PI); 
            float U2 = phi2/(2*PI); 
            float V1 = theta1/PI; 
            float V2 = theta2/PI;

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
            vecRes = GLvec3_push_point(&verts, n1); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL); //Normalized pos
            vecRes = GLvec3_push(&verts, U1, V1); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);//Textures

            vecRes = GLvec3_push_point(&verts, v2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push(&verts, U2, V1); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL); 

            vecRes = GLvec3_push_point(&verts, v3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push(&verts, U1, V2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

            // Triangle 2
            vecRes = GLvec3_push_point(&verts, v2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push(&verts, U2, V1); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);



            vecRes = GLvec3_push_point(&verts, v4); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n4); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push(&verts, U2, V2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);


            vecRes = GLvec3_push_point(&verts, v3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push_point(&verts, n3); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
            vecRes = GLvec3_push(&verts, U1, V2); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
        }
    }
    return verts;
}

void solar_obj_init_moon(SolarObj* moon, const SolarObj* parent, const MoonData* moon_data) {
    // Calculate moon's initial position (offset from parent)
    vec3d_t moon_offset = { moon_data->distance_from_parent * METER_TO_OPENGL, 0.0, 0.0 };
    vec3d_t moon_pos;
    vec3d_add(parent->position, moon_offset, &moon_pos);

    // Calculate orbital velocity for circular orbit
    double orbital_speed = sqrt((G * (double)parent->mass) / moon_data->distance_from_parent);
    orbital_speed *= moon_data->orbital_speed_factor; 

    vec3d_t moon_vel_offset = { 0.0, orbital_speed * METER_TO_OPENGL, 0.0 };
    vec3d_t moon_vel;
    vec3d_add(parent->velocity, moon_vel_offset, &moon_vel);

   
    solar_obj_init(moon, moon_pos, moon_vel, moon_data->color,
        moon_data->mass, moon_data->density, moon_data->scale, -1 , 0.1f);

    moon->parent_id = moon_data->parent_planet_id;
    moon->is_moon = true;
}

 unsigned int solar_obj_get_texture(int id){
    unsigned int texture; 
    glGenTextures(1, &(texture));  
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    char* path; 
    switch(id){
        case SUN: path = "images/8k_sun.jpg"; break;
        case MERCURY: path = "images/8k_mercury.jpg"; break;
        case VENUS: path = "images/8k_venus_surface.jpg"; break;
        case EARTH: path = "images/8k_earth_daymap.jpg"; break;
        case MARS: path = "images/8k_mars.jpg"; break;
        case JUPITER: path = "images/8k_jupiter.jpg"; break;
        case SATURN: path = "images/8k_saturn.jpg"; break;
        case URANUS: path = "images/2k_uranus.jpg"; break;
        case NEPTUNE: path = "images/2k_neptune.jpg"; break;
        default: path = "images/8k_moon.jpg"; break;
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0); 
    if(data == NULL){
        printf("image load fail\n");
        exit(EXIT_FAILURE); 
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
 printf("Generated texture ID %u for object %d\n", texture, id);
    return texture; 
}

void solar_obj_init(SolarObj* obj, vec3d_t startPos, vec3d_t startVel, colorVec_t color, float mass, float density, float scale, int id, float rotSpeed) {
    obj->position = startPos;
    obj->velocity = startVel;
    obj->mass = mass;
    obj->density = density;
    obj->scale = scale;
    obj->color = color;
    obj->accleration = (vec3d_t){ 0.0f };
    obj->priorAccleration = (vec3d_t){ 0.0f };
    obj-> id = id; 
    obj->rotationSpeed = rotSpeed; 
    float volume = mass / density; 
    float radius_meters = powf((3.0f * volume) / (4.0f * PI), 1.0f / 3.0f);

    obj->radius = radius_meters * METER_TO_OPENGL * obj->scale; // convert radius to OpenGL units directly

    GLvec3_t verticies = solar_obj_draw_sphere(obj->radius);
    obj->vertex_count = verticies.size;

    obj->texture = solar_obj_get_texture(id); 
    solar_obj_init_VAO_VBO(&(obj->VAO), &(obj->VBO), verticies.data, obj->vertex_count);


    vector_result_t vecRes = GLvec3_free(&verticies); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

}


double solar_obj_check_collision(const SolarObj *obj1, const SolarObj *obj2) {
    double dx = obj2->position.x - obj1->position.x;
    double dy = obj2->position.y - obj1->position.y;
    double dz = obj2->position.z - obj1->position.z;

    double distance = sqrt(dx * dx + dy * dy + dz * dz);
    if ((double)(obj2->radius/obj2->scale) + (double)(obj1->radius/obj1->scale) > distance) {
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
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

