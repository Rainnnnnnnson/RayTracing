#pragma once
#include "Hitable.h"
/*
	ʹ�����ģʽ�е�װ����
	������ƽ�ƺ���ת
	�Ժ���Կ��Ǹĳɼ���������
*/

class Translate : public Hitable {
public:
	Translate(unique_ptr<Hitable> hitable, Vector displacement);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const override;
private:
	unique_ptr<Hitable> hitable;
	Vector displacement;
};
