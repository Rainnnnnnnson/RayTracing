#pragma once
#include "Hitable.h"
#include "Material.h"
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
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	RGBImage texture;
	Point center;
	float radius;
};

/*
	体积雾
*/
class ConstantMedium : public Hitable {
public:
	/*
		传入其中的hitable 是几何体的形状 
		几何体的材质随便填可以为null

	*/
	ConstantMedium(unique_ptr<Hitable> hitable, float density, shared_ptr<Texture> texture);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	unique_ptr<Hitable> hitable;
	shared_ptr<Texture> texture;
	float density;
};
