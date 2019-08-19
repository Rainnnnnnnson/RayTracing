#pragma once
#include "Hitable.h"
#include "Texture.h"

class Material {
public:

	/*
		散射光线
		返回false 没有光线
		若出现系数相加则使用概率实现
		保证反射光线永远只有一条
		scattered   进行下一次递归的光线
		emitted     自发光颜色
		attenuation 递归时的衰弱系数
	*/
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(shared_ptr<Texture> albedo);
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	shared_ptr<Texture> albedo;
};

class Metal : public Material {
public:
	Metal(Color albedo, float fuzzier);
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	Color albedo;
	float fuzzier;
};

class Dielectric : public Material {
public:
	Dielectric(float refractive);
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	float refractive;
};

class DiffuseLight : public Material {
public:
	DiffuseLight(shared_ptr<Texture> texture);
	virtual bool Calculate(const HitRecord& record, Ray& scattered, Color& emitted, Color& attenuation) const override;
private:
	shared_ptr<Texture> emit;
};