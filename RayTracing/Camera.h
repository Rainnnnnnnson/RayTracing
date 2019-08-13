#pragma once
#include "Math.h"
class Camera {
public:
	/*
		eye           ���λ��
		lookAt        �����λ��
		up            ���� һ����˵��(0,0,1)

		vfov          �ӽ� һ����45��
		aspect        ����
		aperture      ��Ȧ
		focalDistance ����

		time0 time1 ʱ�䷶Χ ����Ĺ��߻��ڷ�Χ�����
	*/
	Camera(Point eye, Point lookAt, Vector up, 
		   float vfov, float aspect, float aperture, float focalDistance,
		   float time0, float time1);
	/*
		[0,1]������
	*/
	Ray GetRay(float u, float v) const;
private:
	float lensRadius;
	float time0, time1;
	Point origin;
	Point lowerLeftCorner;
	Vector horizontal;
	Vector vertical;
	Vector u, v, w;
};