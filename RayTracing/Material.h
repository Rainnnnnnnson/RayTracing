#pragma once
#include "Hitable.h"
#include "Texture.h"

class Material {
public:

	/*
		ɢ�����
		����false û�й���
		������ϵ�������ʹ�ø���ʵ��
		��֤���������Զֻ��һ��
		scattered   ������һ�εݹ�Ĺ���
		emitted     �Է�����ɫ
		attenuation �ݹ�ʱ��˥��ϵ��
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