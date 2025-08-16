#pragma once
#include "solarobj.h"
typedef struct {
    SolarObj *objs;
    int planet_count;
    int moon_count;
    int total_count;
}SolarSystem;

SolarSystem solar_system_init();
void solar_system_log_tail_points(SolarSystem *s);
void solar_system_calc_gravity(SolarSystem* s);
SolarSystem rk45(SolarSystem s, double initTime, double finalTime, double h_initial, vec3d_t tol_abs_pos, vec3d_t tol_abs_vel, vec3d_t tol_rel_pos, vec3d_t tol_rel_vel);
void rk45_updateSystem(SolarSystem* s, const SolarSystem temp, double h); 
SolarSystem solar_system_copy(SolarSystem src);
void rk45_k1(SolarSystem* k1, SolarSystem* y, double h);
void rk45_k2(SolarSystem* k2, SolarSystem* temp, const SolarSystem k1, const SolarSystem y, double h);
void rk45_k3(SolarSystem* k3, SolarSystem* temp, const SolarSystem k2, const SolarSystem k1, const SolarSystem y, double h);
void rk45_k4(SolarSystem* k4, SolarSystem* temp, const SolarSystem k3, const SolarSystem k2, const SolarSystem k1, const SolarSystem y, double h);
void rk45_k5(SolarSystem* k5, SolarSystem* temp, const SolarSystem k4, const SolarSystem k3, const SolarSystem k2, const SolarSystem k1, const SolarSystem y, double h);
void rk45_k6(SolarSystem* k6, SolarSystem* temp, const SolarSystem k5, const SolarSystem k4, const SolarSystem k3, const SolarSystem k2, const SolarSystem k1, const SolarSystem y, double h);
void rk45_err_correct_4th_order(SolarSystem* y4th_orderSolution, const SolarSystem y, const SolarSystem k1, const SolarSystem k3, const SolarSystem k4, const SolarSystem k5);
void rk45_err_correct_5th_order(SolarSystem* y5th_orderSolution, const SolarSystem y, const SolarSystem k1, const SolarSystem k3, const SolarSystem k4, const SolarSystem k5, const SolarSystem k6);
void rk45_err_vec(vec3d_t* errPos, vec3d_t* errVel, SolarSystem y5th_orderSolution, SolarSystem y4th_orderSolution);
void rk45_tolerance_vec(vec3d_t* tolerancePos, vec3d_t* toleranceVel, vec3d_t tolAbsPos, vec3d_t tolRelPos, vec3d_t tolAbsVel, vec3d_t tolRelVel, SolarSystem y, SolarSystem y4th_orderSolution);
bool rk45_check_step_acceptance(vec3d_t* errPos, vec3d_t* errVel, vec3d_t* tolerancePos, vec3d_t* toleranceVel, int total_count);
