#include "utils.h"

double degrees_to_radians(double deg) {
	double result = (PI / 180.0) * deg;
	return result;
}

double safe_div(double a, double b){
	return (fabs(b) > 1e-15) ? (a / b) : 1e15; // big scale means it's fine
}
