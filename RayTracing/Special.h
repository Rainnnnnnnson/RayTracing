#pragma once
#include "Hitable.h"
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
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const override;
private:
	RGBImage texture;
	Point center;
	float radius;
};

