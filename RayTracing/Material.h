#pragma once
#include "Hitable.h"
#include "Texture.h"
class Material {
public:

	/*
		散射光线
		若出现系数相加则使用概率实现
		保证反射光线永远只有一条
		attenuation 递归时的衰弱系数
		scattered 进行下一次递归的光线
	*/
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const = 0;

	/*
		自发光使用
		默认情况 Color(0.0f,0.0f,0.0f)
		u v 用于纹理
		p 用于噪音 湍流
		不一定使用 uv 和 p
	*/
	virtual Color Emitted(float u, float v, Point p) const;
};

class DiffuseLight : public Material {
public:
	DiffuseLight(unique_ptr<Texture> texture);
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const override;
	virtual Color Emitted(float u, float v, Point p) const;
private:
	unique_ptr<Texture> emit;
};

class Lambertian : public Material {
public:
	Lambertian(unique_ptr<Texture> albedo);
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const override;
private:
	unique_ptr<Texture> albedo;
};

class Metal : public Material {
public:
	Metal(Color albedo, float fuzzier);
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const override;
private:
	Color albedo;
	float fuzzier;
};

class Dielectric : public Material {
public:
	Dielectric(float refractive);
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const override;
private:
	float refractive;
};

class Iostropic : public Material {
public:
	Iostropic(Texture* p);
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const override;
private:
	Texture* albedo;
};