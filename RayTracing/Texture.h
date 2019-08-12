#pragma once
#include "RGBImage.h"
class Texture {
public:
	/*
		通过 顶点 或者 纹理坐标 得到值
	*/
	virtual Color Value(float u, float v, Point p) const = 0;
};

class ConstantTexture : public Texture {
public:
	ConstantTexture(Color c);
	virtual Color Value(float u, float v, Point p) const override;
private:
	Color color;
};

class CheckerTexture : public Texture {
public:
	CheckerTexture(unique_ptr<Texture> odd, unique_ptr<Texture> even);
	virtual Color Value(float u, float v, Point p) const override;
private:
	unique_ptr<Texture> odd;
	unique_ptr<Texture> even;
};

class NoiseTexture : public Texture {
public:
	NoiseTexture(float scale);
	virtual Color Value(float u, float v, Point p) const override;
private:
	float scale;
};

class ImageTexture : public Texture {
public:
	ImageTexture(RGBImage image);
	virtual Color Value(float u, float v, Point p) const override;
private:
	RGBImage image;
};