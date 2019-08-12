#pragma once
#include "Math.h"
#include "AxisAlignmentBoundingBox.h"
/*
	��¼Hit��Ľ��
	t ���� Point + t * Direction �е�t
	hitPoint ���е�
	normal ����
	emit �Է��� ���� ����
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
		ɢ�����
		������ϵ�������ʹ�ø���ʵ��
		��֤���������Զֻ��һ��
		attenuation �ݹ�ʱ��˥��ϵ��
		scattered ������һ�εݹ�Ĺ���
	*/
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const = 0;
	/*
		�Է��� ���� ����
		��ʹ����Color(0.0f,0.0f,0.0f)
		u v ��������
		p �������� ����
		��һ��ʹ�� uv �� p
	*/
	virtual Color Emitted(float u, float v, Point p) const;
};