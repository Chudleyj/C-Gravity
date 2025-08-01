#include "solarsystem.h"

SolarSystem solar_obj_make_solar_system() {
    vector_result_t vecRes;

    float AU_SCALE = 1.496e11f / 10.0f; // 1 AU = 10 OpenGL units


    vec3d_t earthPos = (vec3d_t){ (double)AU_IN_METERS * (double)METER_TO_OPENGL, 0.0, 0.0 };

    double moonDistance = 3.84e8; // meters
    double moonDistOffest = 1.0; 
    vec3d_t moonOffset = { moonDistance * (double)METER_TO_OPENGL * moonDistOffest, 0.0, 0.0 }; // radial from Earth
    vec3d_t moonPos;
    vecRes = vec3d_add(earthPos, moonOffset, &moonPos); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

    vec3d_t earthVel = (vec3d_t){ 0.0, 29783.6 * METER_TO_OPENGL, 0.0 };

    double earthMass = 5.972e24;
    double gravConstant = 6.6743e-11; // m³/kg/s²


    // v = sqrt(GM/r) for circular orbit
    double moonOrbitalSpeed = sqrt((gravConstant * earthMass) / (moonDistance * moonDistOffest));

    vec3d_t moonVelOffset = { 0.0, moonOrbitalSpeed * (double)METER_TO_OPENGL, 0.0 }; // perpendicular to orbit
    vec3d_t moonVel;

    vecRes = vec3d_add(earthVel, moonVelOffset, &moonVel); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

   vec3d_t objPositions[] = {
        (vec3d_t){0.0, 0.0, 0.0},                                      // Sun at origin
        (vec3d_t){5.791e10 * METER_TO_OPENGL, 0.0, 0.0},              // Mercury at 0.39 AU
        (vec3d_t){1.082e11 * METER_TO_OPENGL, 0.0, 0.0},              // Venus at 0.72 AU
        (vec3d_t){AU_IN_METERS * METER_TO_OPENGL, 0.0, 0.0},          // Earth at 1 AU
        (vec3d_t){2.279e11 * METER_TO_OPENGL, 0.0, 0.0},              // Mars at 1.52 AU
        (vec3d_t){7.785e11 * METER_TO_OPENGL, 0.0, 0.0},              // Jupiter at 5.2 AU
        (vec3d_t){1.4335e12 * METER_TO_OPENGL, 0.0, 0.0},             // Saturn at 9.58 AU
        (vec3d_t){2.87e12 * METER_TO_OPENGL, 0.0, 0.0},               // Uranus at 19.2 AU
        (vec3d_t){4.495e12 * METER_TO_OPENGL, 0.0, 0.0},              // Neptune at 30.1 AU
         moonPos                                                       // Moon
    };

printf("%f, %f, %f\n %f %f %f\n", moonPos.x, moonPos.y, moonPos.z, earthPos.x, earthPos.y, earthPos.z);

   vec3d_t objStartVels[] = {
        (vec3d_t){0.0, 0.0, 0.0},                                    // Sun (no velocity)
        (vec3d_t){0.0, 47360.0 * METER_TO_OPENGL, 0.0},             // Mercury
        (vec3d_t){0.0, 35020.0 * METER_TO_OPENGL, 0.0},             // Venus
        (vec3d_t){0.0, 29783.6 * METER_TO_OPENGL, 0.0},             // Earth
        (vec3d_t){0.0, 24077.0 * METER_TO_OPENGL, 0.0},             // Mars
        (vec3d_t){0.0, 13070.0 * METER_TO_OPENGL, 0.0},             // Jupiter
        (vec3d_t){0.0, 9690.0 * METER_TO_OPENGL, 0.0},              // Saturn
        (vec3d_t){0.0, 6800.0 * METER_TO_OPENGL, 0.0},              // Uranus
        (vec3d_t){0.0, 5430.0 * METER_TO_OPENGL, 0.0},              // Neptune
        moonVel                                                     // Moon
   };

   // Densities in kg/m³ (order: Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune)
   float objDensities[] = {
       1410.0f, // Sun
       5430.0f, // Mercury  
       5240.0f, // Venus
       5510.0f, // Earth
       3930.0f, // Mars
       1330.0f, // Jupiter
       687.0f, // Saturn
       1270.0f, // Uranus
       1640.0f, // Neptune
       1000.0f // Moon
   };


   float objMasses[] = {
      1.989e30f,  // Sun
      3.285e23f,  // Mercury  
      4.867e24f,  // Venus
      5.972e24f,  // Earth
      6.39e23f,   // Mars
      1.898e27f,  // Jupiter
      5.683e26f,  // Saturn
      8.681e25f,  // Uranus
      1.024e26f,  // Neptune
      7.3e22f     // Moon
   };

   // Visual scaling factors (order: Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune)
   float scale[] = {
        20.0f,   // Sun (make it visible but not huge)
        3000.0f, // Mercury (tiny, needs big scaling)
        1500.0f, // Venus
        1000.0f, // Earth  
        2500.0f, // Mars
        70.0f,   // Jupiter (big planet, less scaling needed)
        90.0f,   // Saturn
        200.0f,   // Uranus
        200.0f,    // Neptune
        270.0f // Moon // Moon is ~0.27 Earth radii
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
        (colorVec_t){91.0f/ glColorsScale, 93.0f/ glColorsScale, 223.0f / glColorsScale,  1.0f}, //Neptune
        (colorVec_t){1.0f, 1.0f, 1.0f,  1.0f} //Moon
   };


    SolarSystem sol;
    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        solar_obj_init(&sol.objs[i], objPositions[i], objStartVels[i], colors[i], objMasses[i], objDensities[i], scale[i]);
    }

    return sol; 
}


//Force = mass * accl
//Force due to gravity = F(g)
//F(g) = G * ( (m1 * m2)/ distance^2)

//Assume earth = mE
// r = dist of mE and m (pythagoream theorm) 
//F(g) = G((mE * m) / r^2)
// 
//since F = ma = F(g):
// ma = G((mE * m)/r^2)
// 
//derivative - rate of change of some quantity expressed as a func
// velocity: 1st deriv of pos:
// ie, V = dr/dt: V = (r2 - r1) / (t2 - t1) where r2 is current pos, r1 prior pos, t2 current time, t1 prior time
// Simplyfing further, t2-t1 is delta time: V = (r2 - r1) / delta time
// 
// 
// 
//Acceleration: first deriv of velocity , 2nd deriv of pos
// a = (d^2r)/(dt^2)
//a = dV/dt = d^2r/dt^2
// therefore,  a = (v2 - v1) / delta time
// 
// alternatively, going straight from position: 
// v2 = (r2 - r1) / delta time
// v1 = (r1 - r0) / delta time
// 
//  a = ((r2 - 2*r1 + r0) / delta time^2)
// 
//  a = (r2 - 2r1 + r0) / (dt * dt)
// 
// diff eq of grav: d^2r/dt^2 = G(M/r^2)
// 
// recalling that:  ma = G((mE * m)/r^2), divide out m and then:
// a = G * mE / r^2
// thus, d^2r/dt^2 = G * M/r^2 where M = mE
//
//Problem: this is a scal equation, it tells us magnitude of gravity but NOT direction)
//Solution: vector equation time
// let r = positions (x,y,z), R = magnitude (distance) sqrt(x^2 + y^2 + z^2)
// ^r = r/R
// i.e., vecDir = vecPos/vecDist
// note: vecPos is the DIRECTION vector technically, thus its vecPos2 - vecPos1
// direction on an object is towards the sun so we inverse the vecDir:
// -^r
// 
// d^2r/dt^2 = ((G*M)/r^2)*(-vecDir) = -(G*M/r^2)*vecDir = -G*M * (vecPos / vecDist^3)
// 
// 
// a = -G*M * (vecPos / vecDist^3)
// acclVec = -G*M * (vecPos / vecDist^3)
// we can use a scale func and reword this to accl = scale(vecPos, -G * M / (vecDist * vecDist * vecDist)
void solar_system_calc_gravity(SolarSystem* s) {
    SolarObj* compareObj = &s->objs[0];
    SolarObj* currentObj = &s->objs[0];

    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        s->objs[i].priorAccleration = s->objs[i].accleration;
        s->objs[i].accleration = (vec3d_t){ 0.0, 0.0, 0.0 };
    }


    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        currentObj = &s->objs[i];
        for (int j = 0; j < NUM_SOLAR_OBJS; j++) {
            compareObj = &s->objs[j];
            if (j != i) {
                vec3d_t direction;
                vec3d_subtract(compareObj->position, currentObj->position, &direction);

                double distance = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
                double minDist = (double)((currentObj->radius / currentObj->scale) + (compareObj->radius / compareObj->scale)) * 1.1;
                if (distance < minDist) distance = minDist;

                double distanceCubed = distance * distance * distance;
                double scalar = G_SCALED * (double)compareObj->mass / distanceCubed;
     
                currentObj->accleration.x += direction.x * scalar;
                currentObj->accleration.y += direction.y * scalar;
                currentObj->accleration.z += direction.z * scalar;

            }
        }
 

    }

}

void solor_system_physics_update(SolarSystem* s, const float dTime) {
    double dt = (double)dTime;
    static int count = 0;
    vector_result_t vecRes;
    double step = 0.5; 
    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        SolarObj* obj = &s->objs[i];

        vecRes = vec3d_scale(&obj->accleration, step * dt); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);
        vecRes = vec3d_add(obj->velocity, obj->accleration, &obj->velocity); if (vecRes) vector_generic_error_handle(vecRes, __func__, CRITICAL);

    }

    //manually do the "scale" and add from here on out bc scale will change velocity/accl permanently and we dont want that right here
    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        SolarObj* obj = &s->objs[i];

        obj->position.x += obj->velocity.x * dt;
        obj->position.y += obj->velocity.y * dt;
        obj->position.z += obj->velocity.z * dt;
    }

    solar_system_calc_gravity(s);

    //  Second half of velocity Verlet (v += 0.5 * a_new * dt)
    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        SolarObj* obj = &s->objs[i];

        obj->velocity.x += 0.5 * obj->accleration.x * dt;
        obj->velocity.y += 0.5 * obj->accleration.y * dt;
        obj->velocity.z += 0.5 * obj->accleration.z * dt;
    }


}