#pragma once
#include "Hitable.h"
#include "RGBImage.h"
/*
	特例 需要用到类中的数据
	所以单独拉出来
	等到有足够多相似的特例的时候就重构
*/

/*
	伯朗体 
	球型
	根据中心点 和 hitPoint 来确定纹理
*/
class CircleTextureSphere : public Hitable {
public:
	CircleTextureSphere(Point center, float radius, RGBImage texture);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const override;
private:
	RGBImage texture;
	Point center;
	float radius;
};

