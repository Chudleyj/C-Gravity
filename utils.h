#pragma once

#define PI 3.14159265359f
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define G 6.6743e-11 // m^3 kg^-1 s^-2
#define G_SCALED (G / (1e9 * 1e9 * 1e9))
#define PHYSICS_TIMESTEP 1000.0f
#define SIZE_RATIO 1000000000.0f    
#define METER_TO_OPENGL 1e-9f  // 1 OpenGL unit = 1 billion meters = 1 million km
#define AU_IN_METERS 1.496e11f // 1 AU in meters
#define NUM_SOLAR_OBJS 9 //TODO: Really shouldnt be a hard define, this should be variable...
//1500.0f

#define MAX_ACCL 10000000000000.0f //1e-3f 

double degrees_to_radians(double deg);