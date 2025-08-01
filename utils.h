#pragma once

#define PI 3.14159265359f
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define G 6.6743e-11 // m^3 kg^-1 s^-2
#define G_SCALED (G / (1e9 * 1e9 * 1e9))
#define PHYSICS_TIMESTEP 1000.0f
//1500.0f

#define MAX_ACCL 10000000000000.0f //1e-3f 

double degrees_to_radians(double deg);