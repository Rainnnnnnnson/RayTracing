#pragma once
#include "Hitable.h"
#include "Texture.h"
class Material {
public:

	/*
		ɢ�����
		������ϵ�������ʹ�ø���ʵ��
		��֤���������Զֻ��һ��
		attenuation �ݹ�ʱ��˥��ϵ��
		scattered ������һ�εݹ�Ĺ���
	*/
	virtual bool Scatter(Ray ray, HitRecord record, Color& attenuation, Ray& scattered) const = 0;

	/*
		�Է���ʹ��
		Ĭ����� Color(0.0f,0.0f,0.0f)
		u v ��������
		p �������� ����
		��һ��ʹ�� uv �� p
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