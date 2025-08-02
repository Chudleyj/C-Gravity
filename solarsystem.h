#pragma once
#include "solarobj.h"
typedef struct {
    SolarObj *objs;
    int planet_count;
    int moon_count;
    int total_count;
}SolarSystem;

SolarSystem solar_obj_make_solar_system();
void solar_system_calc_gravity(SolarSystem* s);
void solor_system_physics_update(SolarSystem* s, const float dTime);
SolarSystem rk45(SolarSystem s, double initTime, double finalTime, double h_initial, vec3d_t tol_abs_pos, vec3d_t tol_abs_vel, vec3d_t tol_rel_pos, vec3d_t tol_rel_vel);
void rk45_updateSystem(SolarSystem* s, const SolarSystem temp, double h); \
SolarSystem copy_solar_system(SolarSystem src);