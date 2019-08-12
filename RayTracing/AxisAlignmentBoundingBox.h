#pragma once
#include "Math.h"
class AxisAlignmentBoundingBox {
public:
	AxisAlignmentBoundingBox(Point minAxis, Point maxAxis);
	Point MinAxis() const;
	Point MaxAxis() const;
	bool Hit(Ray ray, float tMin, float tMax) const;
private:
	Point minAxis;
	Point maxAxis;
};