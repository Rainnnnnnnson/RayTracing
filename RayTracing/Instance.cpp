#include "Instance.h"

Translate::Translate(unique_ptr<Hitable> hitable, Vector displacement) : hitable(std::move(hitable)), displacement(displacement) {}

bool Translate::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	Ray moveRay((ray.Origin() - displacement), ray.Direction(), ray.Time());
	if (!hitable->Hit(moveRay, tMin, tMax, record)) {
		return false;
	}
	record.hitable = const_cast<Translate*>(this);
	return true;
}

bool Translate::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	if (!hitable->BoundingBox(time0, time1, box)) {
		return false;
	}
	box = AxisAlignmentBoundingBox(box.MinAxis() + displacement, box.MaxAxis() + displacement);
	return true;
}

bool Translate::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	Ray moveRay((ray.Origin() + displacement), ray.Direction(), ray.Time());
	return hitable->Calculate(moveRay, t, scattered, emited, attenuation);
}