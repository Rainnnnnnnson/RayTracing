#pragma once
#include "Math.h"
#include <memory>
#include <vector>
using std::vector;
using std::unique_ptr;
using std::make_unique;

/*
	记录Hit后的结果
	t 代表 Point + t * Direction 中的t
	hitPoint 击中点
	normal 法线
	emit 自发光

	与原版不一样 原版的设计不太好
	这里将材质得到的结果直接放在这里 避免外部调用
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
	包围盒
*/
class AxisAlignmentBoundingBox {
public:
	/*
		默认构造器
		minAxis(0.0f,0.0f,0.0f)
		maxAxis(0.0f,0.0f,0.0f)
		调用AxisAlignmentBoundingBox的Hit时 必定是false
	*/
	AxisAlignmentBoundingBox();

	/*
		将两个BOX合成成一个BOX
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
		只会在构造BVHTree的时候调用一次
		返回false 表示没有实现边界盒 或者 无法获取边界盒子
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const = 0;
};

/*
	直接遍历 不使用边界盒
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
	层次遍历
*/
class BVHTree : public Hitable {
public:
	BVHTree(vector<unique_ptr<Hitable>> hitables, float time0, float time1);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;

	/*
		这里的time0 和 time1 没有作用
		只是为了符合接口
		所有初始化都在构造器中完成
		使用构造器中的time0和time1来确定范围
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
private:
	unique_ptr<Hitable> node;
};