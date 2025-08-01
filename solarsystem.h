#pragma once
#include "solarobj.h"
typedef struct {
    SolarObj objs[NUM_SOLAR_OBJS];
}SolarSystem;

SolarSystem solar_obj_make_solar_system();
void solar_system_calc_gravity(SolarSystem* s);
void solor_system_physics_update(SolarSystem* s, const float dTime);