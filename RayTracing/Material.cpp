#include "Material.h"

Vector RamdomInUnitSphere() {
	Vector p;
	do {
		p = 2.0f * Point(Random(), Random(), Random()) - Point(1.0f, 1.0f, 1.0f);
	} while (p.Length() >= 1.0f);
	return p;
}

Lambertian::Lambertian(unique_ptr<Texture> albedo) : albedo(std::move(albedo)) {}

bool Lambertian::Calculate(const CalculateData& data, Ray& scattered, Color& emitted, Color& attenuation) const {
	Point target = data.hitPoint + data.normal + RamdomInUnitSphere();
	scattered = Ray(data.hitPoint, target - data.hitPoint, data.ray.Time());
	attenuation = albedo->Value(data.hitPoint);
	return true;
}

