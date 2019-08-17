#include "Special.h"

CircleTextureSphere::CircleTextureSphere(Point center, float radius, RGBImage texture)
	: center(center), radius(radius), texture(std::move(texture)) {}

bool CircleTextureSphere::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	Vector co = ray.Origin() - center;
	float a = ray.Direction().Dot(ray.Direction());
	float b = 2.0f * co.Dot(ray.Direction());
	float c = co.Dot(co) - radius * radius;
	float delta = b * b - 4 * a * c;
	if (delta > 0) {
		float t;
		float t1 = (-b - sqrt(delta)) / (2.0f * a);
		float t2 = (-b + sqrt(delta)) / (2.0f * a);
		if (t1 > tMin && t1 < tMax) {
			t = t1;
		} else if (t2 > tMin && t2 < tMax) {
			t = t2;
		} else {
			return false;
		}
		Point hitPoint = ray.PointAtParamter(t);
		Vector normal = (hitPoint - center).Normalize();
		record = {const_cast<CircleTextureSphere*>(this), t, ray, hitPoint, normal};
		return true;
	}
	return false;
}

bool CircleTextureSphere::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	box = AxisAlignmentBoundingBox(center - Vector(radius, radius, radius), center + Vector(radius, radius, radius));
	return true;
}

void GetSphereUV(Vector vec, float& u, float& v) {
	float phi = atan2(vec.Z(), vec.X());
	float theta = asin(vec.Y());
	u = 1.0f - (phi + PI) / (2.0f * PI);
	v = (theta + PI / 2.0f) / PI;
}

bool CircleTextureSphere::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {

	Point target = record.hitPoint + record.normal + RamdomInUnitSphere();
	scattered = Ray(record.hitPoint, (target - record.hitPoint).Normalize(), record.ray.Time());
	emitted = Color(0.0f, 0.0f, 0.0f);

	float u;
	float v;
	GetSphereUV(record.normal, u, v);
	int w = static_cast<int>(u * static_cast<float>(texture.GetWidth()));
	int h = static_cast<int>(v * static_cast<float>(texture.GetHeight()));
	w = std::clamp(w, 0, texture.GetWidth());
	h = std::clamp(h, 0, texture.GetHeight());
	attenuation = texture.GetImagePoint(w, h).ToColor();

	return true;
}
