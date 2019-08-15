#pragma once
#include "Math.h"
#include <memory>
#include <vector>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

class Hitable;

/* 
	hitable ���������� 
	ray ԭ������Ĺ���
	t ����
*/
struct HitRecord {
	Hitable* hitable = nullptr;
	Ray ray;
	float t = 0;
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
	/*
		�����ԭ�治һ��
		ɸѡ����С�� t 
		��һ�μ������ record.hitable.Caculate()
	*/
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const = 0;
	/*
		ֻ���ڹ���BVHTree��ʱ�����һ��
		����false ��ʾû��ʵ�ֱ߽�� ���� �޷���ȡ�߽����
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const = 0;
	/*
		ͨ�� t �������һ�εݹ�����Ҫ������
		HitList �� BVHTree ��ʵ���������
		���� record.hitable.Caculate();

		����false ����û�������Ҫ�����ݹ�Ĺ���
		emited ����Ӱ�� bool��Ӱ��
	*/
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const = 0;
};

/*
	ֱ�ӱ��� ��ʹ�ñ߽��
*/
class HitList : public Hitable {
public:
	HitList(vector<unique_ptr<Hitable>> hitables);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const override;
private:
	vector<unique_ptr<Hitable>> hitables;
};

/*
	��α���
*/
class BVHTree : public Hitable {
public:
	/*
		time0 time1Ҫ�������Χ��ʱ��
		������ӱ߽粻��ȷ
	*/
	BVHTree(vector<unique_ptr<Hitable>> hitables, float time0, float time1);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;

	/*
		�����time0 �� time1 û������
		ֻ��Ϊ�˷��Ͻӿ�
		���г�ʼ�����ڹ����������
		ʹ�ù������е�time0��time1��ȷ����Χ
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const override;
private:
	unique_ptr<Hitable> node;
};