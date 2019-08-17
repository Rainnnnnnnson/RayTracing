#include "Instance.h"

FlipNormal::FlipNormal(unique_ptr<Hitable> hitable) : hitable(std::move(hitable)) {}

bool FlipNormal::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	bool isHit = hitable->Hit(ray, tMin, tMax, record);
	record.normal = -record.normal;
	return isHit;
}

bool FlipNormal::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	return hitable->BoundingBox(time0, time1, box);
}

bool FlipNormal::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	return record.hitable->Calculate(record, scattered, emitted, attenuation);
}

Translate::Translate(unique_ptr<Hitable> hitable, Vector displacement) : hitable(std::move(hitable)), displacement(displacement) {}

bool Translate::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	Ray moveRay((ray.Origin() - displacement), ray.Direction(), ray.Time());
	if (!hitable->Hit(moveRay, tMin, tMax, record)) {
		return false;
	}
	record.ray = ray;
	record.hitPoint = record.hitPoint + displacement;
	return true;
}

bool Translate::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	if (!hitable->BoundingBox(time0, time1, box)) {
		return false;
	}
	box = AxisAlignmentBoundingBox(box.MinAxis() + displacement, box.MaxAxis() + displacement);
	return true;
}

bool Translate::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	Ray ray((record.ray.Origin() + displacement), record.ray.Direction(), record.ray.Time());
	Point hitPoint = record.hitPoint + displacement;

	HitRecord newRecord = record;
	newRecord.ray = ray;
	newRecord.hitPoint = hitPoint;
	return newRecord.hitable->Calculate(record, scattered, emitted, attenuation);
}

RotateYAxis::RotateYAxis(unique_ptr<Hitable> hitable, float angle) : hitable(std::move(hitable)){
	float radians = (PI / 180.0f) * angle;
	sinTheta = sin(radians);
	cosTheta = cos(radians);
}

bool RotateYAxis::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	Point origin = ray.Origin();
	Vector direction = ray.Direction();
	origin.X() = cosTheta * ray.Origin().X() - sinTheta * ray.Origin().Z();
	origin.Z() = sinTheta * ray.Origin().X() + cosTheta * ray.Origin().Z();
	direction.X() = cosTheta * ray.Direction().X() - sinTheta * ray.Direction().Z();
	direction.Z() = sinTheta * ray.Direction().X() + cosTheta * ray.Direction().Z();
	Ray rotatedRay = Ray(origin, direction, ray.Time());
	if (!hitable->Hit(rotatedRay, tMin, tMax, record)) {
		return false;
	}
	Point hitPoint = record.hitPoint;
	Vector normal = record.normal;
	hitPoint.X() = cosTheta * record.hitPoint.X() + sinTheta * record.hitPoint.Z();
	hitPoint.Z() = -sinTheta * record.hitPoint.X() + cosTheta * record.hitPoint.Z();
	normal.X() = cosTheta * record.normal.X() + sinTheta * record.normal.Z();
	normal.Z() = -sinTheta * record.normal.X() + cosTheta * record.normal.Z();
	record.ray = ray;
	record.hitPoint = hitPoint;
	record.normal = normal;
	return true;
}

bool RotateYAxis::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	Point min(FLT_MAX, FLT_MAX, FLT_MAX);
	Point max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	if (!hitable->BoundingBox(time0, time1, box)) {
		return false;
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				float x = static_cast<float>(i) * box.MaxAxis().X() + static_cast<float>(1 - i) * box.MinAxis().X();
				float y = static_cast<float>(j) * box.MaxAxis().Y() + static_cast<float>(1 - j) * box.MinAxis().Y();
				float z = static_cast<float>(k) * box.MaxAxis().Z() + static_cast<float>(1 - k) * box.MinAxis().Z();
				float newX = cosTheta * x + sinTheta * z;
				float newZ = -sinTheta * x + cosTheta * z;
				Vector tester(newX, y, newZ);
				for (int d = 0; d < 3; d++) {
					if (tester[d] > max[d]) {
						max[d] = tester[d];
					}
					if (tester[d] < min[d]) {
						min[d] = tester[d];
					}
				}
			}
		}
	}
	box = AxisAlignmentBoundingBox(min, max);
	return true;
}

bool RotateYAxis::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	return record.hitable->Calculate(record, scattered, emitted, attenuation);
}
