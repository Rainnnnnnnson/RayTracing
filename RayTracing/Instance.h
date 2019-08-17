#pragma once
#include "Hitable.h"
/*
	使用设计模式中的装饰器
	来进行平移和旋转
	以后可以考虑改成几何体内置
*/

class FlipNormal : public Hitable {
public:
	FlipNormal(unique_ptr<Hitable> hitable);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	unique_ptr<Hitable> hitable;
};

class Translate : public Hitable {
public:
	Translate(unique_ptr<Hitable> hitable, Vector displacement);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	unique_ptr<Hitable> hitable;
	Vector displacement;
};

class RotateYAxis : public Hitable {
public:
	RotateYAxis(unique_ptr<Hitable> hitable, float angle);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	unique_ptr<Hitable> hitable;
	float sinTheta;
	float cosTheta;
};
