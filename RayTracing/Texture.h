#pragma once
#include "Math.h"
#include <memory>
using std::unique_ptr;
class Texture {
public:
	/*
		通过 顶点 得到值
	*/
	virtual Color Value(Point p) const = 0;
};

class ConstantTexture : public Texture {
public:
	ConstantTexture(Color c);
	virtual Color Value(Point p) const override;
private:
	Color color;
};

class CheckerTexture : public Texture {
public:
	CheckerTexture(unique_ptr<Texture> odd, unique_ptr<Texture> even, float distance);
	virtual Color Value(Point p) const override;
private:
	unique_ptr<Texture> odd;
	unique_ptr<Texture> even;
	float distance;
};

class NoiseTexture : public Texture {
public:
	NoiseTexture(float scale);
	virtual Color Value(Point p) const override;
private:
	float scale;
};