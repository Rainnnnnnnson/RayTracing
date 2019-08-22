#include "Camera.h"

Camera::Camera(Point eye, Point lookAt, Vector up,
			   float vfov, float aspect, float aperture, float focalDistance,
			   float time0, float time1) : time0(time0), time1(time1) {
	lensRadius = aperture / 2.0f;
	float theta = vfov * PI / 180.0f;
	float halfHeight = tan(theta / 2.0f);
	float halfWidth = aspect * halfHeight;

	w = (lookAt - eye).Normalize();
	u = up.Cross(w).Normalize();
	v = w.Cross(u);

	origin = eye;
	lowerLeftCorner = origin + w * focalDistance + (-u * focalDistance * halfWidth) + (-v * focalDistance * halfHeight);

	horizontal = u * halfWidth * focalDistance * 2.0f;
	vertical = v * halfHeight * focalDistance * 2.0f;
}

Vector RamdomInUnitDisk() {
	Vector p;
	do {
		p = 2.0f * Point(Random(), Random(), 0.0f) - Point(1.0f, 1.0f, 0.0f);
	} while (p.Length() >= 1.0f);
	return p.Normalize();
}

Ray Camera::GetRay(float u, float v) const {
	Vector rand = RamdomInUnitDisk() * lensRadius;
	Vector lensOffest = this->u * rand.X() + this->v * rand.Y();
	float time = time0 + Random() * (time1 - time0);
	return Ray(origin + lensOffest, (lowerLeftCorner - origin + horizontal * u + vertical * v - lensOffest).Normalize(), time);
}
