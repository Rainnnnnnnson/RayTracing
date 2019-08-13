#pragma once
#include "Math.h"
#include <memory>
#include <vector>
using std::vector;
using std::unique_ptr;
using std::make_unique;

/*
	��¼Hit��Ľ��
	t ���� Point + t * Direction �е�t
	hitPoint ���е�
	normal ����
	emit �Է���

	��ԭ�治һ�� ԭ�����Ʋ�̫��
	���ｫ���ʵõ��Ľ��ֱ�ӷ������� �����ⲿ����
*/
class HitRecord {
public:
	HitRecord();
	HitRecord(float t, Point hitPoint, Vector normal, Color emit);
public:
	float t;
	Point hitPoint;
	Vector normal;
	Color emit;
};

/*
	��Χ��
*/
class AxisAlignmentBoundingBox {
public:
	/*
		Ĭ�Ϲ�����
		minAxis(0.0f,0.0f,0.0f)
		maxAxis(0.0f,0.0f,0.0f)
		����AxisAlignmentBoundingBox��Hitʱ �ض���false
	*/
	AxisAlignmentBoundingBox();

	/*
		������BOX�ϳɳ�һ��BOX
	*/
	AxisAlignmentBoundingBox(AxisAlignmentBoundingBox box1, AxisAlignmentBoundingBox box2);
	AxisAlignmentBoundingBox(Point minAxis, Point maxAxis);
	Point MinAxis() const;
	Point MaxAxis() const;
	bool Hit(Ray ray, float tMin, float tMax) const;
private:
	Point minAxis;
	Point maxAxis;
};

class Hitable {
public:
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const = 0;
	/*
		ֻ���ڹ���BVHTree��ʱ�����һ��
		����false ��ʾû��ʵ�ֱ߽�� ���� �޷���ȡ�߽����
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const = 0;
};

/*
	ֱ�ӱ��� ��ʹ�ñ߽��
*/
class HitList : public Hitable {
public:
	HitList(vector<unique_ptr<Hitable>> hitables);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	vector<unique_ptr<Hitable>> hitables;
};

/*
	��α���
*/
class BVHTree : public Hitable {
public:
	BVHTree(vector<unique_ptr<Hitable>> hitables, float time0, float time1);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;

	/*
		�����time0 �� time1 û������
		ֻ��Ϊ�˷��Ͻӿ�
		���г�ʼ�����ڹ����������
		ʹ�ù������е�time0��time1��ȷ����Χ
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	unique_ptr<Hitable> node;
};