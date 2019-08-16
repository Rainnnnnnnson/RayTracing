#pragma once
#include "Math.h"
#include <memory>
using std::shared_ptr;
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
	CheckerTexture(shared_ptr<Texture> odd, shared_ptr<Texture> even, float distance);
	virtual Color Value(Point p) const override;
private:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
	float distance;
};

class NoiseTexture : public Texture {
public:
	NoiseTexture(float scale);
	virtual Color Value(Point p) const override;
private:
	float scale;
};

class TurbulenceTexture : public Texture {
public:
	/*
		k1 控制其间隔 k1 越大 间隔越小
		k2 湍流横向的波纹
	*/
	TurbulenceTexture(float scale, float k1, float k2, int depth);
	virtual Color Value(Point p) const override;
private:
	float scale;
	float k1;
	float k2;
	int depth;
};