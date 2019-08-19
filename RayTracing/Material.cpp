#include "Material.h"
#include "Assertion.h"
Lambertian::Lambertian(shared_ptr<Texture> albedo) : albedo(std::move(albedo)) {}

bool Lambertian::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	Point target = record.hitPoint + record.normal + RamdomInUnitSphere();
	scattered = Ray(record.hitPoint, (target - record.hitPoint).Normalize(), record.ray.Time());
	emitted = Color(0.0f, 0.0f, 0.0f);
	attenuation = albedo->Value(record.hitPoint);
	return true;
}

Metal::Metal(Color albedo, float fuzzier) : albedo(albedo), fuzzier(fuzzier) {
	assertion(fuzzier >= 0.0f && fuzzier <= 1.0f);
}

Vector Reflect(Vector v, Vector normal) {
	return v - (normal * 2.0f * v.Dot(normal));
}

bool Metal::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	Vector reflected = Reflect(record.ray.Direction(), record.normal);
	scattered = Ray{record.hitPoint, reflected + RamdomInUnitSphere() * fuzzier, record.ray.Time()};
	emitted = Color(0.0f, 0.0f, 0.0f);
	attenuation = albedo;
	return scattered.Direction().Dot(record.normal) > 0;
}

Dielectric::Dielectric(float refractive) : refractive(refractive) {}

bool Refract(Vector v, Vector normal, float NiOverNt, Vector& refracted) {
	v = v.Normalize();
	float dt = v.Dot(normal);
	float discriminant = 1.0f - NiOverNt * NiOverNt * (1.0f - dt * dt);
	if (discriminant > 0.0f) {
		float cosRefact = sqrt(discriminant);
		refracted = (v + -(normal * dt)) * NiOverNt + -(normal * cosRefact);
		return true;
	} else {
		return false;
	}
}

float Schlick(float cosine, float refracted) {
	float r0 = (refracted - 1.0f) / (refracted + 1.0f);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1 - cosine), 5.0f);
}

bool Dielectric::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	Vector outwardNormal;
	Vector reflected = Reflect(record.ray.Direction(), record.normal);
	float NiOverNt;
	attenuation = Color{1.0f, 1.0f, 1.0f};
	Vector refracted;
	float reflectPercent;
	float cosine;

	if (record.ray.Direction().Dot(record.normal) > 0) {
		outwardNormal = -record.normal;
		NiOverNt = refractive;
		cosine = refractive * record.ray.Direction().Dot(record.normal) / record.ray.Direction().Length();
	} else {
		outwardNormal = record.normal;
		NiOverNt = 1.0f / refractive;
		cosine = -record.ray.Direction().Dot(record.normal) / record.ray.Direction().Length();
	}

	if (Refract(record.ray.Direction(), outwardNormal, NiOverNt, refracted)) {
		reflectPercent = Schlick(cosine, refractive);
	} else {
		reflectPercent = 1.0f;
	}

	if (Random() < reflectPercent) {
		scattered = Ray(record.hitPoint, reflected, record.ray.Time());
	} else {
		scattered = Ray(record.hitPoint, refracted, record.ray.Time());
	}
	emitted = Color(0.0f, 0.0f, 0.0f);
	return true;
}

DiffuseLight::DiffuseLight(shared_ptr<Texture> texture) : emit(std::move(texture)) {}

bool DiffuseLight::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	emitted = emit->Value(record.hitPoint);
	return false;
}