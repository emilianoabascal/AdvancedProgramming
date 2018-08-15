#include <stdio.h>
#include <math.h>
#include "exersizeFuncs.h"


void calculateParabole(double u, double a, double t){
	double x = u*t*(cos(a));
	double y = u*t*(sin(a));
	printf("X Value: %f, Y Value %f\n", x, y);
	
	double Vi = u*cos(a);
	double Vj = u*sin(a);
	
	double V = sqrt(Vi + Vj);
	
	//printf("Velocity %f", V);
	
}