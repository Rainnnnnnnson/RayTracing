#include "Geometry.h"
#include "Instance.h"
using std::make_shared;
Sphere::Sphere(Point center, float radius, shared_ptr<Material> material)
	: center(center), radius(radius), material(std::move(material)) {}

bool Sphere::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	Vector co = ray.Origin() - center;
	float a = ray.Direction().Dot(ray.Direction());
	float b = 2.0f * co.Dot(ray.Direction());
	float c = co.Dot(co) - radius * radius;
	float delta = b * b - 4 * a * c;
	if (delta <= 0) {
		return false;
	}
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
	record = {const_cast<Sphere*>(this), ray, t};
	return true;
}

bool Sphere::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	box = AxisAlignmentBoundingBox(center - Vector(radius, radius, radius), center + Vector(radius, radius, radius));
	return true;
}

bool Sphere::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	CalculateData data;
	data.ray = ray;
	data.hitPoint = ray.PointAtParamter(t);
	data.normal = (data.hitPoint - center) / radius;
	return material->Calculate(data, scattered, emited, attenuation);
}

MovingSphere::MovingSphere(Point center0, Point center1, float time0, float time1, float radius, shared_ptr<Material> material)
	: center0(center0), center1(center1), time0(time0), time1(time1), radius(radius), material(std::move(material)) {}

bool MovingSphere::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	return false;
}

bool MovingSphere::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	auto box0 = AxisAlignmentBoundingBox(center0 - Vector(radius, radius, radius), center0 + Vector(radius, radius, radius));
	auto box1 = AxisAlignmentBoundingBox(center1 - Vector(radius, radius, radius), center1 + Vector(radius, radius, radius));
	box = AxisAlignmentBoundingBox(box0, box1);
	return true;
}

bool MovingSphere::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	throw std::exception("TODO");
}

XYRect::XYRect(float x0, float x1, float y0, float y1, float z, shared_ptr<Material> material)
	: x0(x0), x1(x1), y0(y0), y1(y1), z(z), material(std::move(material)) {}

bool XYRect::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	float t = (z - ray.Origin().Z()) / ray.Direction().Z();
	if (t < tMin || t > tMax) {
		return false;
	}
	float x = ray.Origin().X() + t * ray.Direction().X();
	float y = ray.Origin().Y() + t * ray.Direction().Y();
	if (x < x0 || x > x1 || y < y0 || y > y1) {
		return false;
	}
	record = {const_cast<XYRect*>(this), ray, t};
	return true;
}

bool XYRect::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	box = AxisAlignmentBoundingBox(Point(x0, y0, z - 0.0001f), Point(x1, y1, z + 0.0001f));
	return true;
}

bool XYRect::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	throw std::exception("TODO");
}

XZRect::XZRect(float x0, float x1, float z0, float z1, float y, shared_ptr<Material> material)
	: x0(x0), x1(x1), z0(z0), z1(z1), y(y), material(std::move(material)) {}

bool XZRect::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	float t = (y - ray.Origin().Y()) / ray.Direction().Y();
	if (t < tMin || t > tMax) {
		return false;
	}
	float x = ray.Origin().X() + t * ray.Direction().X();
	float z = ray.Origin().Z() + t * ray.Direction().Z();
	if (x < x0 || x > x1 || z < z0 || z > z1) {
		return false;
	}
	record = {const_cast<XZRect*>(this), ray, t};
	return true;
}

bool XZRect::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	box = AxisAlignmentBoundingBox(Point(x0, y - 0.0001f, z0), Point(x1, y + 0.0001f, z1));
	return true;
}

bool XZRect::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	throw std::exception("TODO");
}

YZRect::YZRect(float y0, float y1, float z0, float z1, float x, shared_ptr<Material> material)
	: y0(y0), y1(y1), z0(z0), z1(z1), x(x), material(std::move(material)) {}

bool YZRect::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	float t = (x - ray.Origin().X()) / ray.Direction().X();
	if (t < tMin || t > tMax) {
		return false;
	}
	float y = ray.Origin().Y() + t * ray.Direction().Y();
	float z = ray.Origin().Z() + t * ray.Direction().Z();
	if (y < y0 || y > y1 || z < z0 || z > z1) {
		return false;
	}
	record = {const_cast<YZRect*>(this), ray, t};
	return true;
}

bool YZRect::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	box = AxisAlignmentBoundingBox(Point(x - 0.0001f, y0, z0), Point(x + 0.0001f, y1, z1));
	return true;
}

bool YZRect::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	throw std::exception("TODO");
}

Box::Box(Point pMin, Point pMax, shared_ptr<Material> material)
	: pMin(pMin), pMax(pMax), list(InitList(std::move(material))) {}

bool Box::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	return false;
}

bool Box::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	return false;
}

bool Box::Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const {
	throw std::exception("TODO");
}

HitList Box::InitList(shared_ptr<Material>  material) {
	vector<unique_ptr<Hitable>> hitables;
	hitables.reserve(6);
	hitables.emplace_back(make_unique<FlipNormal>(make_unique<XYRect>(pMin.X(), pMax.X(), pMin.Y(), pMax.Y(), pMin.Z(), material)));
	hitables.emplace_back(make_unique<XYRect>(pMin.X(), pMax.X(), pMin.Y(), pMax.Y(), pMax.Z(), material));
	hitables.emplace_back(make_unique<FlipNormal>(make_unique<XZRect>(pMin.X(), pMax.X(), pMin.Z(), pMax.Z(), pMin.Y(), material)));
	hitables.emplace_back(make_unique<XZRect>(pMin.X(), pMax.X(), pMin.Z(), pMax.Z(), pMax.Y(), material));
	hitables.emplace_back(make_unique<FlipNormal>(make_unique<YZRect>(pMin.Y(), pMax.Y(), pMin.Z(), pMax.Z(), pMin.X(), material)));
	hitables.emplace_back(make_unique<YZRect>(pMin.Y(), pMax.Y(), pMin.Z(), pMax.Z(), pMax.X(), material));
	return HitList(std::move(hitables));
}
