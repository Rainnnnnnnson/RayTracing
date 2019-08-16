#pragma once
#include "Math.h"
#include <memory>
using std::shared_ptr;
using std::unique_ptr;

class Texture {
public:
	/*
		ͨ�� ���� �õ�ֵ
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
		k1 �������� k1 Խ�� ���ԽС
		k2 ��������Ĳ���
	*/
	TurbulenceTexture(float scale, float k1, float k2, int depth);
	virtual Color Value(Point p) const override;
private:
	float scale;
	float k1;
	float k2;
	int depth;
};