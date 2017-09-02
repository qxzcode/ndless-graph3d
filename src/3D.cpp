#include "3D.h"

constexpr double P_SCALE = 430.0;

constexpr int SCREEN_W = 320;
constexpr int SCREEN_H = 240;

void Camera::calcMatrix() {
	double sy = sin(yaw), cy = cos(yaw);
	double sp = sin(pitch), cp = cos(pitch);
	bool xRev = sy>0, yRev = cy>0;
	xInner = std::abs(sy) < std::abs(cy);
	aRev = xInner? yRev : xRev;
	bRev = xInner? xRev : yRev;
	m_xx = -cy;    m_xy =  sy;
	m_yx =  sy*sp; m_yy =  cy*sp; m_yz = cp;
	m_zx = -sy*cp; m_zy = -cy*cp; m_zz = sp;
}

point Camera::transform(double x, double y, double z) const {
	// view transform...
	double xt = m_xx*x + m_xy*y;
	double yt = m_yx*x + m_yy*y + m_yz*z;
	double zt = m_zx*x + m_zy*y + m_zz*z;
	xt -= this->x;
	yt -= this->y;
	zt -= this->z;
	
	return point{
		static_cast<int>(P_SCALE*xt/zt) + SCREEN_W/2,
		static_cast<int>(P_SCALE*yt/zt) + SCREEN_H/2
	};
}
