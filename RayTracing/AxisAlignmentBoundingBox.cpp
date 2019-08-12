#include "AxisAlignmentBoundingBox.h"
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
