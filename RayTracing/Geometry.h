#pragma once
#include "Hitable.h"
#include "Material.h"
using std::shared_ptr;
using std::make_shared;

class Sphere : public Hitable {
public:
	Sphere(Point center, float radius, shared_ptr<Material> material);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	shared_ptr<Material> material;
	Point center;
	float radius;
};

/*
	从center0 线性平移至 center1
	会根据时间来确定中心点的位置
	包围盒的要覆盖所有将会移动到的地方
*/
class MovingSphere : public Hitable {
public:
	MovingSphere(Point center0, Point center1, float time0, float time1, float radius, shared_ptr<Material> material);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	shared_ptr<Material> material;
	Point center0;
	Point center1;
	float time0;
	float time1;
	float radius;
};

class XYRect : public Hitable {
public:
	XYRect(float x0, float x1, float y0, float y1, float z, shared_ptr<Material> material);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	shared_ptr<Material> material;
	float x0, x1, y0, y1, z;
};

class XZRect : public Hitable {
public:
	XZRect(float x0, float x1, float z0, float z1, float y, shared_ptr<Material> material);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	shared_ptr<Material> material;
	float x0, x1, z0, z1, y;
};

class YZRect : public Hitable {
public:
	YZRect(float y0, float y1, float z0, float z1, float x, shared_ptr<Material> material);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	shared_ptr<Material> material;
	float y0, y1, z0, z1, x;
};

class Box : public Hitable {
public:
	Box(Point pMin, Point pMax, shared_ptr<Material> material);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	HitList InitList(shared_ptr<Material> material);
	Point pMin, pMax;
	HitList list;
};