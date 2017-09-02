#include "grid.h"

#include <utility> // std::swap
#include <SDL/SDL_gfxPrimitives.h>
#include "3D.h"

Grid::Grid(function_t func):func(func) {
	setWindow(-3,3, -3,3);
}


void Grid::setWindow(double xMin, double xMax, double yMin, double yMax) {
	double xStep = (xMax-xMin)/(GRID_SIZE-1);
	double yStep = (yMax-yMin)/(GRID_SIZE-1);
	double wx = xMin;
	for (int x = 0; x < GRID_SIZE; x++,wx+=xStep) {
		double wy = yMin;
		for (int y = 0; y < GRID_SIZE; y++,wy+=yStep) {
			hmap[x][y] = func(wx, wy);
			if (x != 0 && y != 0) {
				vec3 p1{x*QUAD_SIZE, y*QUAD_SIZE, hmap[x][y]};
				vec3 p2{p1.x-QUAD_SIZE, p1.y, hmap[x-1][y]};
				vec3 p3{p1.x-QUAD_SIZE, p1.y-QUAD_SIZE, hmap[x-1][y-1]};
				vec3 p4{p1.x, p1.y-QUAD_SIZE, hmap[x][y-1]};
				normals[x-1][y-1][0] = (p2-p1).cross(p3-p1).normalize();
				normals[x-1][y-1][1] = (p3-p1).cross(p4-p1).normalize();
			}
		}
	}
}


constexpr vec3 lightDir = vec3{1,1,1}.normalize();

static void drawTriangle(SDL_Surface* screen,
                         point p1, point p2, point p3,
                         const vec3& normal) {
	// compute color from normal
	double br = lightDir.dot(normal)*0.5 + 0.5;
	
	// draw the triangle
	filledTrigonRGBA(screen, p1.x,p1.y, p2.x,p2.y, p3.x,p3.y, 0,0,255*br,255);
	//trigonRGBA      (screen, p1.x,p1.y, p2.x,p2.y, p3.x,p3.y, 0,0,0,255);
}

static void drawQuad(SDL_Surface* screen,
                     point p1, point p2, point p3, point p4,
                     const vec3* normals) {
	drawTriangle(screen, p1, p2, p3, normals[0]);
	drawTriangle(screen, p1, p3, p4, normals[1]);
	//polygonRGBA();
}

void Grid::draw(SDL_Surface* screen, const Camera& cam) const {
	// compute projected (on-screen) point locations
	point points[GRID_SIZE][GRID_SIZE];
	double wx = -0.5;
	for (int x = 0; x < GRID_SIZE; x++,wx+=QUAD_SIZE) {
		double wy = -0.5;
		for (int y = 0; y < GRID_SIZE; y++,wy+=QUAD_SIZE) {
			double wz = hmap[x][y];
			points[x][y] = cam.transform(wx, wy, wz);
		}
	}
	
	// draw the geometry
	// (all these shenanigans are to ensure back-to-front draw order)
	int x, y;
	int& a = cam.xInner? y : x;
	int& b = cam.xInner? x : y;
	
	int aBeg = 1, aEnd = GRID_SIZE-1, aStep = 1;
	if (cam.aRev) {
		std::swap(aBeg, aEnd);
		aStep = -aStep;
	}
	aEnd += aStep;
	int bBeg = 1, bEnd = GRID_SIZE-1, bStep = 1;
	if (cam.bRev) {
		std::swap(bBeg, bEnd);
		bStep = -bStep;
	}
	bEnd += bStep;
	
	for (a = aBeg; a != aEnd; a += aStep) {
		for (b = bBeg; b != bEnd; b += bStep) {
			drawQuad(screen,
			         points[x][y],points[x-1][y],
			         points[x-1][y-1],points[x][y-1],
			         normals[x-1][y-1]);
		}
	}
}
