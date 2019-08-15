#include "Instance.h"

FlipNormal::FlipNormal(unique_ptr<Hitable> hitable) : hitable(std::move(hitable)) {}

bool FlipNormal::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	return hitable->Hit(ray, tMin, tMax, record);
}

bool FlipNormal::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	return hitable->BoundingBox(time0, time1, box);
}

bool FlipNormal::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	throw std::exception("TODO");
}

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
	throw std::exception("TODO");
}

RotateYAxis::RotateYAxis(unique_ptr<Hitable> hitable, float angle) : hitable(std::move(hitable)), angle(angle) {}

bool RotateYAxis::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	float radians = (PI / 180.0f) * angle;
	float sinTheta = sin(radians);
	float cosTheta = cos(radians);

	Point origin = ray.Origin();
	Vector direction = ray.Direction();
	origin.X() = cosTheta * ray.Origin().X() - sinTheta * ray.Origin().Z();
	origin.Z() = sinTheta * ray.Origin().X() + cosTheta * ray.Origin().Z();
	direction.X() = cosTheta * ray.Direction().X() - sinTheta * ray.Direction().Z();
	direction.Z() = sinTheta * ray.Direction().X() + cosTheta * ray.Direction().Z();
	Ray RotatedR(origin, direction, ray.Time());

	if (!hitable->Hit(RotatedR, tMin, tMax, record)) {
		return false;
	}
	record.hitable = const_cast<RotateYAxis*>(this);
	return true;
}

bool RotateYAxis::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	if (!hitable->BoundingBox(0.0f, 1.0f, box)) {
		return false;
	};
	float radians = (PI / 180.0f) * angle;
	float sinTheta = sin(radians);
	float cosTheta = cos(radians);
	Point min(FLT_MAX, FLT_MAX, FLT_MAX);
	Point max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				float x = static_cast<float>(i) * box.MaxAxis().X() + static_cast<float>(1 - i) * box.MinAxis().X();
				float y = static_cast<float>(j) * box.MaxAxis().Y() + static_cast<float>(1 - j) * box.MinAxis().Y();
				float z = static_cast<float>(k) * box.MaxAxis().Z() + static_cast<float>(1 - k) * box.MinAxis().Z();
				float newX = cosTheta * x + sinTheta * z;
				float newZ = -sinTheta * x + cosTheta * z;
				Vector tester(newX, y, newZ);
				for (int i = 0; i < 3; i++) {
					if (tester[i] > max[i]) {
						max[i] = tester[i];
					}
					if (tester[i] < min[i]) {
						min[i] = tester[i];
					}
				}
			}
		}
	}
	box = AxisAlignmentBoundingBox(min, max);
	return true;
}

bool RotateYAxis::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	throw std::exception("TODO");
}
