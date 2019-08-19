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
	// 圆的边缘会有黑 用系数来调整
	float f = 0.7f;
	u = (u - 0.5f) * f + 0.5f;
	v = (v - 0.5f) * f + 0.5f;
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
	w = std::clamp(w, 0, texture.GetWidth() - 1);
	h = std::clamp(h, 0, texture.GetHeight() - 1);
	attenuation = texture.GetImagePoint(w, h).ToColor();

	return true;
}

ConstantMedium::ConstantMedium(unique_ptr<Hitable> hitable, float density, shared_ptr<Texture> texture)
	: hitable(std::move(hitable)), density(density), texture(std::move(texture)) {}

bool ConstantMedium::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	/*
		因为一开始光线的起点有可能在体积雾中
		需要使用两次hit
		第一次确定起始边界位置
		第二次确定终点位置
	*/
	HitRecord record1;
	HitRecord record2;
	if (hitable->Hit(ray, -FLT_MAX, FLT_MAX, record1)) {
		if (hitable->Hit(ray, record1.t + 0.0001F, FLT_MAX, record2)) {
			if (record1.t < tMin) {
				record1.t = tMin;
			}
			if (record2.t > tMax) {
				record2.t = tMax;
			}
			if (record1.t >= record2.t) {
				return false;
			}
			if (record1.t < 0.0f) {
				record1.t = 0.0f;
			}

			float distanceInsideBoundary = record2.t - record1.t;
			float hitDistance = -(1.0f / density) * log(Random());
			if (hitDistance < distanceInsideBoundary) {
				float t = record1.t + hitDistance;
				Point hitPoint = ray.PointAtParamter(record.t);
				record = {const_cast<ConstantMedium*>(this), t, ray, hitPoint, {}};
				return true;
			}
		}
	}
	return false;
}

bool ConstantMedium::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	return hitable->BoundingBox(time0, time1, box);
}

bool ConstantMedium::Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const {
	scattered = Ray(record.hitPoint, RamdomInUnitSphere(), record.ray.Time());
	emitted = Color(0.0f, 0.0f, 0.0f);
	attenuation = texture->Value(record.hitPoint);
	return true;
}
