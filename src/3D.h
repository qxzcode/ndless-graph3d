#pragma once

#include <cmath>

struct point {
	int x, y;
};

struct vec3 {
	double x, y, z;
	
	constexpr vec3 operator+(const vec3& rhs) const {
		return {x+rhs.x, y+rhs.y, z+rhs.z};
	}
	
	constexpr vec3 operator-(const vec3& rhs) const {
		return {x-rhs.x, y-rhs.y, z-rhs.z};
	}
	
	constexpr vec3 operator*(double m) const {
		return {x*m, y*m, z*m};
	}
	
	constexpr vec3 operator/(double m) const {
		return {x/m, y/m, z/m};
	}
	
	constexpr double dot(const vec3& rhs) const {
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}
	
	constexpr vec3 cross(const vec3& rhs) const {
		return {y*rhs.z-rhs.y*z, rhs.x*z-x*rhs.z, x*rhs.y-rhs.x*y};
	}
	
	constexpr double norm() const {
		return sqrt(x*x + y*y + z*z);
	}
	
	constexpr vec3 normalize() const {
		return *this / norm();
	}
};

class Camera {
private:
	double m_xx, m_xy,
	       m_yx, m_yy, m_yz,
	       m_zx, m_zy, m_zz;
	
public:
	double x = 0, y = 0, z = 2.5;
	double pitch = M_PI/4, yaw = -M_PI/4;
	
	bool xInner, aRev, bRev;
	
	void calcMatrix();
	point transform(double x, double y, double z) const;
	
};
