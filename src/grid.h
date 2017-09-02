#pragma once

#include "3D.h"

constexpr int GRID_SIZE = 15;
constexpr double QUAD_SIZE = 1.0/(GRID_SIZE-1);

class Grid {
private:
	using function_t = double (*)(double, double);
	
	function_t func;
	double hmap[GRID_SIZE][GRID_SIZE];
	vec3 normals[GRID_SIZE-1][GRID_SIZE-1][2];
	
public:
	Grid(function_t func);
	
	void setWindow(double xMin, double xMax, double yMin, double yMax);
	
	void draw(struct SDL_Surface* screen, const Camera& cam) const;
};
