#include "solarsystem.h"

SolarSystem solar_system_init() {
     vec3d_t objPositions[] = {
        (vec3d_t){0.0, 0.0, 0.0},                                      // Sun at origin
        (vec3d_t){5.791e10 * METER_TO_OPENGL, 0.0, 0.0},              // Mercury at 0.39 AU
        (vec3d_t){1.082e11 * METER_TO_OPENGL, 0.0, 0.0},              // Venus at 0.72 AU
        (vec3d_t){(double)AU_IN_METERS * METER_TO_OPENGL, 0.0, 0.0},          // Earth at 1 AU
        (vec3d_t){2.279e11 * METER_TO_OPENGL, 0.0, 0.0},              // Mars at 1.52 AU
        (vec3d_t){7.785e11 * METER_TO_OPENGL, 0.0, 0.0},              // Jupiter at 5.2 AU
        (vec3d_t){1.4335e12 * METER_TO_OPENGL, 0.0, 0.0},             // Saturn at 9.58 AU
        (vec3d_t){2.87e12 * METER_TO_OPENGL, 0.0, 0.0},               // Uranus at 19.2 AU
        (vec3d_t){4.495e12 * METER_TO_OPENGL, 0.0, 0.0},              // Neptune at 30.1 AU
    };

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
   };

   // Densities in kg/m^3
   float objDensities[] = {
       1410.0f, // Sun
       5430.0f, // Mercury  
       5240.0f, // Venus
       5510.0f, // Earth
       3930.0f, // Mars
       1330.0f, // Jupiter
       687.0f, // Saturn
       1270.0f, // Uranus
       1640.0f // Neptune
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
      1.024e26f // Neptune
    
   };

   float scale[] = {
        20.0f,   // Sun
        3000.0f, // Mercury 
        1500.0f, // Venus
        1000.0f, // Earth  
        1500.0f, // Mars
        70.0f,   // Jupiter 
        90.0f,   // Saturn
        200.0f,   // Uranus
        200.0f  // Neptune
 
   };

float rotationSpeeds[] = {
    0.1f * PHYSICS_TIMESTEP / SEC_IN_DAY,      // Sun
    1.0f * PHYSICS_TIMESTEP / SEC_IN_DAY,      // Mercury (1 rotation per Mercury day) 
    -0.004f * PHYSICS_TIMESTEP / SEC_IN_DAY,   // Venus (243 Earth days per rotation)
    1.0f * PHYSICS_TIMESTEP / SEC_IN_DAY,      // Earth (1 rotation per day)
    1.03f * PHYSICS_TIMESTEP / SEC_IN_DAY,     // Mars (24.6 hour day)
    2.4f * PHYSICS_TIMESTEP / SEC_IN_DAY,      // Jupiter (10 hour day)
    2.2f * PHYSICS_TIMESTEP / SEC_IN_DAY,      // Saturn (10.7 hour day)
    -1.4f * PHYSICS_TIMESTEP / SEC_IN_DAY,     // Uranus (17.2 hour day, retrograde)
    1.5f * PHYSICS_TIMESTEP / SEC_IN_DAY       // Neptune (16.1 hour day)
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


   SolarSystem sol = { 0 };
   sol.objs = malloc(sizeof(SolarObj) * TOTAL_OBJECTS);
   if (sol.objs == NULL) {
       printf("SOL MALLOC FAIL\n");
       exit(EXIT_FAILURE); 
   }

    for (int i = 0; i < NUM_SOLAR_OBJS; i++) {
        solar_obj_init(&sol.objs[i], objPositions[i], objStartVels[i], colors[i], objMasses[i], objDensities[i], scale[i], i, rotationSpeeds[i]);
        sol.objs[i].parent_id = -1; 
        sol.objs[i].is_moon = false; 
    }
    sol.planet_count = NUM_SOLAR_OBJS;

    for (int i = 0; i < NUM_MOONS; i++) {
        const MoonData* moon_data = &moon_configs[i]; 
        SolarObj* parent = &sol.objs[moon_data->parent_planet_id];
        SolarObj* moon = &sol.objs[sol.planet_count + i]; 
        solar_obj_init_moon(moon, parent, moon_data); 
    }
    sol.moon_count = NUM_MOONS;
    sol.total_count = sol.planet_count + sol.moon_count; 

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

    for (int i = 0; i < s->total_count; i++) {
        s->objs[i].priorAccleration = s->objs[i].accleration;
        s->objs[i].accleration = (vec3d_t){ 0.0, 0.0, 0.0 };
    }


    for (int i = 0; i < s->total_count; i++) {
        currentObj = &s->objs[i];
        for (int j = 0; j < s->total_count; j++) {
            compareObj = &s->objs[j];
            if (j != i) {
                vec3d_t direction;
                vec3d_subtract(compareObj->position, currentObj->position, &direction);

                double distance = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
                double minDist = (double)((currentObj->radius / currentObj->scale) + (double)(compareObj->radius / compareObj->scale)) * 1.1;
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

SolarSystem solar_system_copy(SolarSystem src) {
    SolarSystem dest = { 0 };
    dest.total_count = src.total_count;
    dest.planet_count = src.planet_count;
    dest.moon_count = src.moon_count;

    dest.objs = malloc(sizeof(SolarObj) * src.total_count);
    if (dest.objs == NULL) {
        printf("SOL COPY MALLOC FAIL\n"); 
        exit(EXIT_FAILURE); 
    }

    for (int i = 0; i < src.total_count; i++) {
        dest.objs[i] = src.objs[i];
    }
    return dest;
}

/*
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

initialize t = t0
initialize y = current state (positions + velocities of all bodies)
h = initial time step

while t < tf:
    if t + h > tf:
        h = tf - t  // prevent overshooting

    // Step 1: Evaluate derivative at current state
    k1 = h * f(t, y)

    // Step 2: Use k1 to build a trial state, evaluate derivative
    k2 = h * f(t + a2*h, y + b21*k1)

    // Step 3: Repeat with updated trial states
    find B values at https://ntrs.nasa.gov/api/citations/19690021375/downloads/19690021375.pdf
    k3 = h * f(t + a3*h, y + b31*k1 + b32*k2)
    k4 = h * f(t + a4*h, y + b41*k1 + b42*k2 + b43*k3)
    k5 = h * f(t + a5*h, y + b51*k1 + b52*k2 + b53*k3 + b54*k4)
    k6 = h * f(t + a6*h, y + b61*k1 + ... + b65*k5)

    // Step 4: Estimate two different solutions
    https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta%E2%80%93Fehlberg_method
    Butcher tableau for Fehlberg's 4(5) method
    The first row of coefficients at the bottom of the table gives the fifth-order accurate method,
    and the second row gives the fourth-order accurate method:
        0
        1/4	1/4
        3/8	3/32	9/32
        12/13	1932/2197	−7200/2197	7296/2197
        1	439/216	−8	3680/513	−845/4104
        1/2	−8/27	2	−3544/2565	1859/4104	−11/40
        16/135	0	6656/12825	28561/56430	−9/50	2/55 <--- 5th Order Solution (note we skip k2)
        25/216	0	1408/2565	2197/4104	−1/5	0 <---- 4th Order Solution (note we skip k2 and k6)


        y_4th = y + (25/216) * k1
                + (1408/2565) * k3
                + (2197/4104) * k4
                - (1/5) * k5

        y_5th = y + (16/135) * k1
                    + (6656/12825) * k3
                    + (28561/56430) * k4
                    - (9/50) * k5
                    + (2/55) * k6

    y_4th = y + weighted_sum(k1, k3, k4, k5)    // 4th-order solution
    y_5th = y + weighted_sum(k1, k3, k4, k5, k6) // 5th-order solution

    // Step 5: Compute error estimate
    err = abs(y_5th - y_4th)
    tolerance = tol_abs + tol_rel * max(abs(y), abs(y_4th))

    if err <= tolerance:
        // Accept step
        y = y_5th
        t = t + h

    // Step 6: Update step size for next iteration
    h = h * (0.84 * (tolerance / err)^(1/4))


//k1:
calcGravity(&y) //new accl
k1 pos = h * y.newPos
k1 vel = h* y.Accl


//k2
temp.pos  = y.pos + (k1.pos * b21)
temp.vel = y.vel + (k1.vel * b21)


calcGravity(&temp)
k2.pos = h * temp.vel
k2.vel = h*temp.accl


//k3
temp.pos = y.pos + (k2.pos * b32) + (k1.pos *b31)
temp.vel = y.vel + (k2.vel * b32) + (k1.vel * b31)


calcGravity(&temp);
k3.pos = h * temp.vel
k3.vel = h*temp.accl



//k4
temp.pos = y.pos +  (k3.pos * b43) +  (k2.pos * b42) + (k1.pos *b41)
temp.vel = y.vel + (k3.vel * b43) + (k2.vel * b42) + (k1.vel * b41)


calcGravity(&temp);
k4.pos = h * temp.vel
k4.vel = h * temp.accl


etc..




k2.vel = h*temp.accl

*/


SolarSystem rk45(SolarSystem s, double initTime, double finalTime, double h_initial, vec3d_t tolAbsPos, vec3d_t tolAbsVel, vec3d_t tolRelPos, vec3d_t tolRelVel) {
    SolarSystem y, k1, k2, k3, k4, k5, k6, temp, y4th_orderSolution, y5th_orderSolution;

    y = solar_system_copy(s);
    k1 = solar_system_copy(s);
    k2 = solar_system_copy(s);
    k3 = solar_system_copy(s);
    k4 = solar_system_copy(s);
    k5 = solar_system_copy(s);
    k6 = solar_system_copy(s);
    temp = solar_system_copy(s);
    y4th_orderSolution = solar_system_copy(s);
    y5th_orderSolution = solar_system_copy(s);

    double h = h_initial; 
    double t = initTime; 
   
    vec3d_t errPos[TOTAL_OBJECTS] = { 0 };
    vec3d_t errVel[TOTAL_OBJECTS] = { 0 };
    vec3d_t tolerancePos[TOTAL_OBJECTS] = { 0 };
    vec3d_t toleranceVel[TOTAL_OBJECTS] = { 0 };
    while (t < finalTime) {
        if (t + h > finalTime) {
            h = finalTime - t;
        }
        bool step_accepted = false;

        while (!step_accepted){
            rk45_k1(&k1, &y, h); 
            
            rk45_k2(&k2, &temp, k1, y, h);

            rk45_k3(&k3, &temp, k2, k1, y, h); 

            rk45_k4(&k4, &temp, k3, k2, k1, y, h); 

            rk45_k5(&k5, &temp, k4, k3, k2, k1, y, h);
            
            rk45_k6(&k6, &temp, k5, k4, k3, k2, k1, y, h);

            //Error correct
            rk45_err_correct_4th_order(&y4th_orderSolution, y, k1,  k3,  k4, k5);
            rk45_err_correct_5th_order(&y5th_orderSolution, y, k1,  k3, k4, k5, k6);
    
            rk45_err_vec(errPos, errVel, y5th_orderSolution, y4th_orderSolution);
            
            rk45_tolerance_vec(tolerancePos, toleranceVel, tolAbsPos, tolRelPos, tolAbsVel, tolRelVel, y, y4th_orderSolution);
          
            step_accepted = rk45_check_step_acceptance(errPos, errVel, tolerancePos, toleranceVel, y.total_count); 
        }


        for (int i = 0; i < y.total_count; i++) {
            y.objs[i] = y5th_orderSolution.objs[i];
        }
        t += h;

        double scale_factor = 1.0;
        // Find the most restrictive scaling 
        for (int i = 0; i < y.total_count; i++) {
            double pos_scale = min(safe_div(tolerancePos[i].x, errPos[i].x),
                min(safe_div(tolerancePos[i].y, errPos[i].y),
                    safe_div(tolerancePos[i].z, errPos[i].z)));
            double vel_scale = min(safe_div(toleranceVel[i].x, errVel[i].x),
                min(safe_div(toleranceVel[i].y, errVel[i].y),
                    safe_div(toleranceVel[i].z, errVel[i].z)));
            scale_factor = min(scale_factor, min(pos_scale, vel_scale));
        }
        if (scale_factor < 3.0) scale_factor = 3.0; 
        h = h * 0.84 * pow(scale_factor, 0.25);

     }

     free(k1.objs);
     free(k2.objs);
     free(k3.objs);
     free(k4.objs);
     free(k5.objs);
     free(k6.objs);
     free(temp.objs);
     free(y4th_orderSolution.objs);
     free(y5th_orderSolution.objs);

     return y; 
}

void rk45_updateSystem(SolarSystem* s, const SolarSystem temp, double h) {
    for (int i = 0; i < temp.total_count; i++) {
        s->objs[i].position.x = temp.objs[i].velocity.x * h;
        s->objs[i].velocity.x = temp.objs[i].accleration.x * h;

        s->objs[i].position.y = temp.objs[i].velocity.y * h;
        s->objs[i].velocity.y = temp.objs[i].accleration.y * h;

        s->objs[i].position.z = temp.objs[i].velocity.z * h;
        s->objs[i].velocity.z = temp.objs[i].accleration.z * h;
    }
}


//k1 = h * f(x + A(1) * h,y)
void rk45_k1(SolarSystem* k1, SolarSystem* y, double h) {
    solar_system_calc_gravity(y);
    rk45_updateSystem(k1, *y, h);
}

//NOTE: FOR COEFFICENTS LIKE B(2,1) , THIS REFERS TO A BUTCHER TABLE. WHILE FHELBERG DEFINES HIS BUTCHER TABLES FROM INIT K = 0, JOHN BUTCHER'S SYNTAX USES INIT K=1 INDEXING. AS SUCH, K1 FOR US IS ROW 0 IN THE BUTCHER TABLE SEE: https://www.math.auckland.ac.nz/~butcher/ODE-book-2008/Tutorials/RK-methods.pdf
//IN PRACTICE, THIS MEANS THAT B(2,1) BELOW ACTUALLY REFERS TO B(1,1) IN FHELBERG'S BUTCHER TABLES
//k2 = h * f(x+ A(2) * h,y + B(2,1) * k1
void rk45_k2(SolarSystem* k2, SolarSystem* temp, const SolarSystem k1, const SolarSystem y, double h) {
    for (int i = 0; i < y.total_count; i++) {
        temp->objs[i].position.x = y.objs[i].position.x + (k1.objs[i].position.x * (1.0 / 4.0));
        temp->objs[i].position.y = y.objs[i].position.y + (k1.objs[i].position.y * (1.0 / 4.0));
        temp->objs[i].position.z = y.objs[i].position.z + (k1.objs[i].position.z * (1.0 / 4.0));

        temp->objs[i].velocity.x = y.objs[i].velocity.x + (k1.objs[i].velocity.x * (1.0 / 4.0));
        temp->objs[i].velocity.y = y.objs[i].velocity.y + (k1.objs[i].velocity.y * (1.0 / 4.0));
        temp->objs[i].velocity.z = y.objs[i].velocity.z + (k1.objs[i].velocity.z * (1.0 / 4.0));

    }
    solar_system_calc_gravity(temp);
    rk45_updateSystem(k2, *temp, h);
}

//k3 = h * f(x + A(3) * h,y + B(3,1) + k1 + B(3,2) * k2 
void rk45_k3(SolarSystem* k3,  SolarSystem* temp, const SolarSystem k2,  const SolarSystem k1, const SolarSystem y, double h) {
    for (int i = 0; i < y.total_count; i++) {
        temp->objs[i].position.x = y.objs[i].position.x + (k1.objs[i].position.x * (3.0 / 32.0)) + (k2.objs[i].position.x * (9.0 / 32.0));
        temp->objs[i].position.y = y.objs[i].position.y + (k1.objs[i].position.y * (3.0 / 32.0)) + (k2.objs[i].position.y * (9.0 / 32.0));
        temp->objs[i].position.z = y.objs[i].position.z + (k1.objs[i].position.z * (3.0 / 32.0)) + (k2.objs[i].position.z * (9.0 / 32.0));

        temp->objs[i].velocity.x = y.objs[i].velocity.x + (k1.objs[i].velocity.x * (3.0 / 32.0)) + (k2.objs[i].velocity.x * (9.0 / 32.0));
        temp->objs[i].velocity.y = y.objs[i].velocity.y + (k1.objs[i].velocity.y * (3.0 / 32.0)) + (k2.objs[i].velocity.y * (9.0 / 32.0));
        temp->objs[i].velocity.z = y.objs[i].velocity.z + (k1.objs[i].velocity.z * (3.0 / 32.0)) + (k2.objs[i].velocity.z * (9.0 / 32.0));
    }
    solar_system_calc_gravity(temp);
    rk45_updateSystem(k3, *temp, h);

}

//k4 = h * f(x + A(4) * h,y + B(4,1)*k1 + B(4,2)*k2 + B(4,3)*k3
void rk45_k4(SolarSystem* k4, SolarSystem* temp, const SolarSystem k3, const SolarSystem k2, const SolarSystem k1, const SolarSystem y, double h) {

    for (int i = 0; i < y.total_count; i++) {
        temp->objs[i].position.x = y.objs[i].position.x + (k1.objs[i].position.x * (1932.0 / 2197.0)) + (k2.objs[i].position.x * (-7200.0 / 2197.0)) + (k3.objs[i].position.x * (7296.0 / 2197.0));
        temp->objs[i].position.y = y.objs[i].position.y + (k1.objs[i].position.y * (1932.0 / 2197.0)) + (k2.objs[i].position.y * (-7200.0 / 2197.0)) + (k3.objs[i].position.y * (7296.0 / 2197.0));
        temp->objs[i].position.z = y.objs[i].position.z + (k1.objs[i].position.z * (1932.0 / 2197.0)) + (k2.objs[i].position.z * (-7200.0 / 2197.0)) + (k3.objs[i].position.z * (7296.0 / 2197.0));

        temp->objs[i].velocity.x = y.objs[i].velocity.x + (k1.objs[i].velocity.x * (1932.0 / 2197.0)) + (k2.objs[i].velocity.x * (-7200.0 / 2197.0)) + (k3.objs[i].velocity.x * (7296.0 / 2197.0));
        temp->objs[i].velocity.y = y.objs[i].velocity.y + (k1.objs[i].velocity.y * (1932.0 / 2197.0)) + (k2.objs[i].velocity.y * (-7200.0 / 2197.0)) + (k3.objs[i].velocity.y * (7296.0 / 2197.0));
        temp->objs[i].velocity.z = y.objs[i].velocity.z + (k1.objs[i].velocity.z * (1932.0 / 2197.0)) + (k2.objs[i].velocity.z * (-7200.0 / 2197.0)) + (k3.objs[i].velocity.z * (7296.0 / 2197.0));
    }
    solar_system_calc_gravity(temp);
    rk45_updateSystem(k4, *temp, h);
}

//k5 = h * f(x + A(5) * h,y + B(5,1)*k1 + B(5,2)*k2 + B(5,3)*k3 + B(5,4)*k4
void rk45_k5(SolarSystem* k5, SolarSystem* temp, const SolarSystem k4, const SolarSystem k3, const SolarSystem k2, const SolarSystem k1, const SolarSystem y, double h) {
    for (int i = 0; i < y.total_count; i++) {
        temp->objs[i].position.x = y.objs[i].position.x + (k1.objs[i].position.x * (439.0 / 216.0)) + (k2.objs[i].position.x * -8.0) + (k3.objs[i].position.x * (3680.0 / 513.0)) + (k4.objs[i].position.x * (-845.0 / 4104.0));
        temp->objs[i].position.y = y.objs[i].position.y + (k1.objs[i].position.y * (439.0 / 216.0)) + (k2.objs[i].position.y * -8.0) + (k3.objs[i].position.y * (3680.0 / 513.0)) + (k4.objs[i].position.y * (-845.0 / 4104.0));
        temp->objs[i].position.z = y.objs[i].position.z + (k1.objs[i].position.z * (439.0 / 216.0)) + (k2.objs[i].position.z * -8.0) + (k3.objs[i].position.z * (3680.0 / 513.0)) + (k4.objs[i].position.z * (-845.0 / 4104.0));

        temp->objs[i].velocity.x = y.objs[i].velocity.x + (k1.objs[i].velocity.x * (439.0 / 216.0)) + (k2.objs[i].velocity.x * -8.0) + (k3.objs[i].velocity.x * (3680 / 513.0)) + (k4.objs[i].velocity.x * (-845.0 / 4104.0));
        temp->objs[i].velocity.y = y.objs[i].velocity.y + (k1.objs[i].velocity.y * (439.0 / 216.0)) + (k2.objs[i].velocity.y * -8.0) + (k3.objs[i].velocity.y * (3680 / 513.0)) + (k4.objs[i].velocity.y * (-845.0 / 4104.0));
        temp->objs[i].velocity.z = y.objs[i].velocity.z + (k1.objs[i].velocity.z * (439.0 / 216.0)) + (k2.objs[i].velocity.z * -8.0) + (k3.objs[i].velocity.z * (3680 / 513.0)) + (k4.objs[i].velocity.z * (-845.0 / 4104.0));
    }
    solar_system_calc_gravity(temp);
    rk45_updateSystem(k5, *temp, h);

}

//k6 = h * f(x + A(6) * h,y + B(6,1)*k1 + B(6,2)*k2 + B(6,3)*k3 + B(6,4)*k4 + B(6,5)*k5
void rk45_k6(SolarSystem* k6, SolarSystem* temp, const SolarSystem k5, const SolarSystem k4, const SolarSystem k3, const SolarSystem k2, const SolarSystem k1, const SolarSystem y, double h) {
    for (int i = 0; i < y.total_count; i++) {
        temp->objs[i].position.x = y.objs[i].position.x + (k1.objs[i].position.x * (-8.0 / 27.0)) + (k2.objs[i].position.x * 2.0) + (k3.objs[i].position.x * (-3544.0 / 2565.0)) + (k4.objs[i].position.x * (1859.0 / 4104.0)) + (k5.objs[i].position.x * (-11.0 / 40.0));
        temp->objs[i].position.y = y.objs[i].position.y + (k1.objs[i].position.y * (-8.0 / 27.0)) + (k2.objs[i].position.y * 2.0) + (k3.objs[i].position.y * (-3544.0 / 2565.0)) + (k4.objs[i].position.y * (1859.0 / 4104.0)) + (k5.objs[i].position.y * (-11.0 / 40.0));
        temp->objs[i].position.z = y.objs[i].position.z + (k1.objs[i].position.z * (-8.0 / 27.0)) + (k2.objs[i].position.z * 2.0) + (k3.objs[i].position.z * (-3544.0 / 2565.0)) + (k4.objs[i].position.z * (1859.0 / 4104.0)) + (k5.objs[i].position.z * (-11.0 / 40.0));

        temp->objs[i].velocity.x = y.objs[i].velocity.x + (k1.objs[i].velocity.x * (-8.0 / 27.0)) + (k2.objs[i].velocity.x * 2.0) + (k3.objs[i].velocity.x * (-3544.0 / 2565.0)) + (k4.objs[i].velocity.x * (1859.0 / 4104.0)) + (k5.objs[i].velocity.x * (-11.0 / 40.0));
        temp->objs[i].velocity.y = y.objs[i].velocity.y + (k1.objs[i].velocity.y * (-8.0 / 27.0)) + (k2.objs[i].velocity.y * 2.0) + (k3.objs[i].velocity.y * (-3544.0 / 2565.0)) + (k4.objs[i].velocity.y * (1859.0 / 4104.0)) + (k5.objs[i].velocity.y * (-11.0 / 40.0));
        temp->objs[i].velocity.z = y.objs[i].velocity.z + (k1.objs[i].velocity.z * (-8.0 / 27.0)) + (k2.objs[i].velocity.z * 2.0) + (k3.objs[i].velocity.z * (-3544.0 / 2565.0)) + (k4.objs[i].velocity.z * (1859.0 / 4104.0)) + (k5.objs[i].velocity.z * (-11.0 / 40.0));
    }
    solar_system_calc_gravity(temp);
    rk45_updateSystem(k6, *temp, h);
}


//WEIGHTED AVERAGE = y(x+h) = y(x) + CH(1)*k1 + CH(2)*k2 + CH(3)*k3 + CH(4)*k4 + CH(5)*k5 + CH(6)*k6
//CH(k) = BUTCHER TABLE VALUES

void rk45_err_correct_4th_order(SolarSystem *y4th_orderSolution, const SolarSystem y, const SolarSystem k1, const SolarSystem k3, const SolarSystem k4, const SolarSystem k5) {
    for (int i = 0; i < y.total_count; i++) {
        y4th_orderSolution->objs[i].position.x = y.objs[i].position.x + ((25.0 / 216.0) * k1.objs[i].position.x) + ((1408.0 / 2565.0) * k3.objs[i].position.x) + ((2197.0 / 4104.0) * k4.objs[i].position.x) - ((1.0 / 5.0) * k5.objs[i].position.x);
        y4th_orderSolution->objs[i].position.y = y.objs[i].position.y + ((25.0 / 216.0) * k1.objs[i].position.y) + ((1408.0 / 2565.0) * k3.objs[i].position.y) + ((2197.0 / 4104.0) * k4.objs[i].position.y) - ((1.0 / 5.0) * k5.objs[i].position.y);
        y4th_orderSolution->objs[i].position.z = y.objs[i].position.z + ((25.0 / 216.0) * k1.objs[i].position.z) + ((1408.0 / 2565.0) * k3.objs[i].position.z) + ((2197.0 / 4104.0) * k4.objs[i].position.z) - ((1.0 / 5.0) * k5.objs[i].position.z);

        y4th_orderSolution->objs[i].velocity.x = y.objs[i].velocity.x + ((25.0 / 216.0) * k1.objs[i].velocity.x) + ((1408.0 / 2565.0) * k3.objs[i].velocity.x) + ((2197.0 / 4104.0) * k4.objs[i].velocity.x) - ((1.0 / 5.0) * k5.objs[i].velocity.x);
        y4th_orderSolution->objs[i].velocity.y = y.objs[i].velocity.y + ((25.0 / 216.0) * k1.objs[i].velocity.y) + ((1408.0 / 2565.0) * k3.objs[i].velocity.y) + ((2197.0 / 4104.0) * k4.objs[i].velocity.y) - ((1.0 / 5.0) * k5.objs[i].velocity.y);
        y4th_orderSolution->objs[i].velocity.z = y.objs[i].velocity.z + ((25.0 / 216.0) * k1.objs[i].velocity.z) + ((1408.0 / 2565.0) * k3.objs[i].velocity.z) + ((2197.0 / 4104.0) * k4.objs[i].velocity.z) - ((1.0 / 5.0) * k5.objs[i].velocity.z);
    }



}

void rk45_err_correct_5th_order(SolarSystem *y5th_orderSolution, const SolarSystem y, const SolarSystem k1, const SolarSystem k3, const SolarSystem k4, const SolarSystem k5, const SolarSystem k6){
    for (int i = 0; i < y.total_count; i++) {
        y5th_orderSolution->objs[i].position.x = y.objs[i].position.x + ((16.0 / 135.0) * k1.objs[i].position.x) + ((6656.0 / 12825.0) * k3.objs[i].position.x) + ((28561.0 / 56430.0) * k4.objs[i].position.x) - ((9.0 / 50.0) * k5.objs[i].position.x) + ((2.0 / 55.0) * k6.objs[i].position.x);
        y5th_orderSolution->objs[i].position.y = y.objs[i].position.y + ((16.0 / 135.0) * k1.objs[i].position.y) + ((6656.0 / 12825.0) * k3.objs[i].position.y) + ((28561.0 / 56430.0) * k4.objs[i].position.y) - ((9.0 / 50.0) * k5.objs[i].position.y) + ((2.0 / 55.0) * k6.objs[i].position.y);
        y5th_orderSolution->objs[i].position.z = y.objs[i].position.z + ((16.0 / 135.0) * k1.objs[i].position.z) + ((6656.0 / 12825.0) * k3.objs[i].position.z) + ((28561.0 / 56430.0) * k4.objs[i].position.z) - ((9.0 / 50.0) * k5.objs[i].position.z) + ((2.0 / 55.0) * k6.objs[i].position.z);

        y5th_orderSolution->objs[i].velocity.x = y.objs[i].velocity.x + ((16.0 / 135.0) * k1.objs[i].velocity.x) + ((6656.0 / 12825.0) * k3.objs[i].velocity.x) + ((28561.0 / 56430.0) * k4.objs[i].velocity.x) - ((9.0 / 50.0) * k5.objs[i].velocity.x) + ((2.0 / 55.0) * k6.objs[i].velocity.x);
        y5th_orderSolution->objs[i].velocity.y = y.objs[i].velocity.y + ((16.0 / 135.0) * k1.objs[i].velocity.y) + ((6656.0 / 12825.0) * k3.objs[i].velocity.y) + ((28561.0 / 56430.0) * k4.objs[i].velocity.y) - ((9.0 / 50.0) * k5.objs[i].velocity.y) + ((2.0 / 55.0) * k6.objs[i].velocity.y);
        y5th_orderSolution->objs[i].velocity.z = y.objs[i].velocity.z + ((16.0 / 135.0) * k1.objs[i].velocity.z) + ((6656.0 / 12825.0) * k3.objs[i].velocity.z) + ((28561.0 / 56430.0) * k4.objs[i].velocity.z) - ((9.0 / 50.0) * k5.objs[i].velocity.z) + ((2.0 / 55.0) * k6.objs[i].velocity.z);
    }
}

void rk45_err_vec(vec3d_t *errPos, vec3d_t *errVel, SolarSystem y5th_orderSolution, SolarSystem y4th_orderSolution) {
    for (int i = 0; i < y5th_orderSolution.total_count; i++) {
        errPos[i] = (vec3d_t){ fabs(y5th_orderSolution.objs[i].position.x - y4th_orderSolution.objs[i].position.x), fabs(y5th_orderSolution.objs[i].position.y - y4th_orderSolution.objs[i].position.y), fabs(y5th_orderSolution.objs[i].position.z - y4th_orderSolution.objs[i].position.z) };
        errVel[i] = (vec3d_t){ fabs(y5th_orderSolution.objs[i].velocity.x - y4th_orderSolution.objs[i].velocity.x), fabs(y5th_orderSolution.objs[i].velocity.y - y4th_orderSolution.objs[i].velocity.y), fabs(y5th_orderSolution.objs[i].velocity.z - y4th_orderSolution.objs[i].velocity.z) };
    }

}
void rk45_tolerance_vec(vec3d_t* tolerancePos, vec3d_t* toleranceVel, vec3d_t tolAbsPos, vec3d_t tolRelPos, vec3d_t tolAbsVel, vec3d_t tolRelVel, SolarSystem y, SolarSystem y4th_orderSolution) {
    for (int i = 0; i < y.total_count; i++) {
        tolerancePos[i] = (vec3d_t){
            (tolAbsPos.x + tolRelPos.x * max(fabs(y.objs[i].position.x), fabs(y4th_orderSolution.objs[i].position.x))),
            (tolAbsPos.y + tolRelPos.y * max(fabs(y.objs[i].position.y), fabs(y4th_orderSolution.objs[i].position.y))),
            (tolAbsPos.z + tolRelPos.z * max(fabs(y.objs[i].position.z), fabs(y4th_orderSolution.objs[i].position.z)))
        };

        toleranceVel[i] = (vec3d_t){
            (tolAbsVel.x + tolRelVel.x * max(fabs(y.objs[i].velocity.x), fabs(y4th_orderSolution.objs[i].velocity.x))),
            (tolAbsVel.y + tolRelVel.y * max(fabs(y.objs[i].velocity.y), fabs(y4th_orderSolution.objs[i].velocity.y))),
            (tolAbsVel.z + tolRelVel.z * max(fabs(y.objs[i].velocity.z), fabs(y4th_orderSolution.objs[i].velocity.z)))
        };

    }
}

bool rk45_check_step_acceptance(vec3d_t *errPos, vec3d_t *errVel, vec3d_t *tolerancePos, vec3d_t *toleranceVel, int total_count) {
    bool step_accepted = true;
    for (int i = 0; i < total_count; i++) {
        if (errPos[i].x > tolerancePos[i].x ||
            errPos[i].y > tolerancePos[i].y ||
            errPos[i].z > tolerancePos[i].z) {
            step_accepted = false;
            break;
        }

        if (errVel[i].x > toleranceVel[i].x ||
            errVel[i].y > toleranceVel[i].y ||
            errVel[i].z > toleranceVel[i].z) {
            step_accepted = false;
            break;
        }
    }
    return step_accepted;
}