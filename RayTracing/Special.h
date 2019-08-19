#pragma once
#include "Hitable.h"
#include "Material.h"
#include "RGBImage.h"
/*
	���� ��Ҫ�õ����е�����
	���Ե���������
	�ȵ����㹻�����Ƶ�������ʱ����ع�
*/

/*
	������ 
	����
	�������ĵ� �� hitPoint ��ȷ������
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
	�����
*/
class ConstantMedium : public Hitable {
public:
	/*
		�������е�hitable �Ǽ��������״ 
		������Ĳ�����������Ϊnull

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
