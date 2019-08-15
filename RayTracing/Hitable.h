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
	hitable 被击中物体 
	ray 原本射入的光线
	t 距离
*/
struct HitRecord {
	Hitable* hitable = nullptr;
	Ray ray;
	float t = 0;
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
	/*
		这里和原版不一样
		筛选出最小的 t 
		下一次计算调用 record.hitable.Caculate()
	*/
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const = 0;
	/*
		只会在构造BVHTree的时候调用一次
		返回false 表示没有实现边界盒 或者 无法获取边界盒子
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const = 0;
	/*
		通过 t 计算出下一次递归所需要的数据
		HitList 和 BVHTree 不实现这个功能
		调用 record.hitable.Caculate();

		返回false 代表没有射出需要继续递归的光线
		emited 不受影响 bool的影响
	*/
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const = 0;
};

/*
	直接遍历 不使用边界盒
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
	层次遍历
*/
class BVHTree : public Hitable {
public:
	/*
		time0 time1要包含最大范围的时间
		否则盒子边界不正确
	*/
	BVHTree(vector<unique_ptr<Hitable>> hitables, float time0, float time1);
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override;

	/*
		这里的time0 和 time1 没有作用
		只是为了符合接口
		所有初始化都在构造器中完成
		使用构造器中的time0和time1来确定范围
	*/
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override;
	virtual bool Calculate(Ray ray, float t, Ray& scattered, Color& emited, Color& attenuation) const override;
private:
	unique_ptr<Hitable> node;
};