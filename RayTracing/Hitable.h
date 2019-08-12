#pragma once
#include "Math.h"
#include "AxisAlignmentBoundingBox.h"
/*
	记录Hit后的结果
	t 代表 Point + t * Direction 中的t
	hitPoint 击中点
	normal 法线
	emit 自发光 或者 纹理
*/
struct HitRecord {
	float t;
	Point hitPoint;
	Vector normal;
	Color emit;
};

class Hitable {
public:
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const = 0;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const = 0;
};

class Material {
public:
	/*
		散射光线
		若出现系数相加则使用概率实现
		保证反射光线永远只有一条
		attenuation 递归时的衰弱系数
		scattered 进行下一次递归的光线
	*/
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const = 0;
	/*
		自发光 或者 纹理
		不使用则Color(0.0f,0.0f,0.0f)
		u v 用于纹理
		p 用于噪音 湍流
		不一定使用 uv 和 p
	*/
	virtual Color Emitted(float u, float v, Point p) const;
};