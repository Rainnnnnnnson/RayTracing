#include "Hitable.h"
#include "Assertion.h"
#include <cmath>
using std::shared_ptr;
using std::make_shared;

HitRecord::HitRecord() : t(0.0f) {}

HitRecord::HitRecord(float t, Point hitPoint, Vector normal, Color emit)
	: t(t), hitPoint(hitPoint), normal(normal), emit(emit) {}

AxisAlignmentBoundingBox::AxisAlignmentBoundingBox() : minAxis(0.0f, 0.0f, 0.0f), maxAxis(0.0f, 0.0f, 0.0f) {}

AxisAlignmentBoundingBox::AxisAlignmentBoundingBox(AxisAlignmentBoundingBox box1, AxisAlignmentBoundingBox box2) {
	Point small;
	Point big;
	for (int i = 0; i < 3; i++) {
		small[i] = std::min(box1.minAxis[i], box2.minAxis[i]);
		big[i] = std::max(box1.maxAxis[i], box2.maxAxis[i]);
	}
	minAxis = minAxis;
	maxAxis = maxAxis;
}

AxisAlignmentBoundingBox::AxisAlignmentBoundingBox(Point minAxis, Point maxAxis) : minAxis(minAxis), maxAxis(maxAxis) {}

Point AxisAlignmentBoundingBox::MinAxis() const {
	return minAxis;
}

Point AxisAlignmentBoundingBox::MaxAxis() const {
	return maxAxis;
}

bool AxisAlignmentBoundingBox::Hit(Ray ray, float tMin, float tMax) const {
	for (int i = 0; i < 3; i++) {
		/*
			直接除以 ray.Direction() 会出现 +0 和 -0
			1  /  +0  =  +∞
			1  /  -0  =  -∞
		*/
		float inverse = 1.0f / ray.Direction()[i];
		float t0 = (MinAxis()[i] - ray.Origin()[i]) * inverse;
		float t1 = (MaxAxis()[i] - ray.Origin()[i]) * inverse;
		if (t1 < t0) {
			std::swap(t0, t1);
		}
		tMin = std::max(t0, tMin);
		tMax = std::min(t1, tMax);
		if (tMax <= tMin) {
			return false;
		}
	}
	return true;
}

HitList::HitList(vector<unique_ptr<Hitable>> hitables) : hitables(std::move(hitables)) {}

bool HitList::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	bool isHit = false;
	float distance = tMax;
	HitRecord tempRecord;
	for (auto& hitable : hitables) {
		if (hitable->Hit(ray, tMin, distance, tempRecord)) {
			isHit = true;
			distance = tempRecord.t;
		}
	}
	record = tempRecord;
	return isHit;
}

bool HitList::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	if (hitables.empty()) {
		return false;
	}
	if (hitables[0]->BoundingBox(time0, time1, box)) {
		return false;
	}
	for (auto& hitable : hitables) {
		AxisAlignmentBoundingBox tempBox;
		if (hitable->BoundingBox(time0, time1, tempBox)) {
			return false;
		}
		box = AxisAlignmentBoundingBox(box, tempBox);
	}
	return true;
}

/*
	BVHTree 的结点
*/
struct BVHNode : public Hitable {
public:
	BVHNode(vector<shared_ptr<Hitable>>::iterator begin, vector<shared_ptr<Hitable>>::iterator end, float time0, float time1)
		: left(nullptr), right(nullptr) {
		/*
			根据轴来排序
		*/
		int axis = static_cast<int>(Random() * 2.999f);
		std::sort(begin, end, [&](const unique_ptr<Hitable>& h0, const unique_ptr<Hitable>& h1) {
			AxisAlignmentBoundingBox leftBox;
			AxisAlignmentBoundingBox rightBox;
			if (!h0->BoundingBox(time0, time1, leftBox) || !h1->BoundingBox(time0, time1, rightBox)) {
				throw std::exception("未实现BoundingBox");
			}
			if (leftBox.MinAxis()[axis] < rightBox.MinAxis()[axis]) {
				return true;
			}
			return false;
		});
		int n = end - begin;
		if (n == 1) {
			left = *begin;
			right = *begin;
		} else if (n == 2) {
			left = *begin;
			right = *(begin + 1);
		} else {
			left = make_shared<BVHNode>(begin, begin + n / 2, time0, time1);
			right = make_shared<BVHNode>(begin + n / 2, end, time0, time1);
		}
		AxisAlignmentBoundingBox leftBox;
		AxisAlignmentBoundingBox rightBox;
		if (!left->BoundingBox(time0, time1, leftBox) || !right->BoundingBox(time0, time1, rightBox)) {
			throw std::exception("未实现BoundingBox");
		}
		box = AxisAlignmentBoundingBox(leftBox, rightBox);
	}
	virtual bool Hit(Ray ray, float tMin, float tMax, HitRecord& record) const override {
		if (!box.Hit(ray, tMin, tMax)) {
			return false;
		}
		HitRecord leftRecord;
		HitRecord rightRecord;
		bool leftHit = left->Hit(ray, tMin, tMax, leftRecord);
		bool rightHit = right->Hit(ray, tMin, tMax, rightRecord);
		if (leftHit && rightHit) {
			if (leftRecord.t < rightRecord.t) {
				record = leftRecord;
			} else {
				record = rightRecord;
			}
			return true;
		} else if (leftHit) {
			record = leftRecord;
			return true;
		} else if (rightHit) {
			record = rightRecord;
			return true;
		} 
		return false;
	}
	virtual bool BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const override {
		box = this->box;
		return true;
	}
private:
	shared_ptr<Hitable> left;
	shared_ptr<Hitable> right;
	AxisAlignmentBoundingBox box;
};

BVHTree::BVHTree(vector<unique_ptr<Hitable>> hitables, float time0, float time1) {
	/*
		BVHNode 有可能左右结点为同一个对象
		所以必须转化指针
	*/
	vector<shared_ptr<Hitable>> sharedHitables;
	sharedHitables.reserve(hitables.size());
	for (auto& ptr : hitables) {
		sharedHitables.emplace_back(std::move(ptr));
	}
	node = make_unique<BVHNode>(sharedHitables.begin(), sharedHitables.end(), time0, time1);
}

bool BVHTree::Hit(Ray ray, float tMin, float tMax, HitRecord& record) const {
	return node->Hit(ray, tMin, tMax, record);
}

bool BVHTree::BoundingBox(float time0, float time1, AxisAlignmentBoundingBox& box) const {
	return node->BoundingBox(time0, time1, box);
}

