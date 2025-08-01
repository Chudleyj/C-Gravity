#pragma once
#include "solarobj.h"
typedef struct {
    SolarObj objs[TOTAL_OBJECTS];
    int planet_count;
    int moon_count;
    int total_count;
}SolarSystem;


SolarSystem solar_obj_make_solar_system();
void solar_system_calc_gravity(SolarSystem* s);
void solor_system_physics_update(SolarSystem* s, const float dTime);