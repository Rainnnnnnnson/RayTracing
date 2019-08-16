#include "Material.h"

Lambertian::Lambertian(shared_ptr<Texture> albedo) : albedo(std::move(albedo)) {}

bool Lambertian::Calculate(const CalculateData& data, Ray& scattered, Color& emitted, Color& attenuation) const {
	Point target = data.hitPoint + data.normal + RamdomInUnitSphere();
	scattered = Ray(data.hitPoint, (target - data.hitPoint).Normalize(), data.ray.Time());
	emitted = Color(0.0f, 0.0f, 0.0f);
	attenuation = albedo->Value(data.hitPoint);
	return true;
}

