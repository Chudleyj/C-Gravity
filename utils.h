#pragma once
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define PI 3.14159265359f
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define G 6.6743e-11 // m^3 kg^-1 s^-2
#define G_SCALED (G / (1e9 * 1e9 * 1e9))
#define METER_TO_OPENGL 1e-9f  // 1 OpenGL unit = 1 billion meters = 1 million km
#define AU_IN_METERS 1.496e11f // 1 AU in meters
#define NUM_SOLAR_OBJS 9 //TODO: Really shouldnt be a hard define, this should be variable...




double degrees_to_radians(double deg);

double safe_div(double a, double b);