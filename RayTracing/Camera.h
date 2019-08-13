#pragma once
#include "Math.h"
class Camera {
public:
	/*
		eye           相机位置
		lookAt        看向的位置
		up            朝上 一般来说是(0,0,1)

		vfov          视角 一般是45°
		aspect        宽长比
		aperture      光圈
		focalDistance 焦距

		time0 time1 时间范围 射出的光线会在范围内随机
	*/
	Camera(Point eye, Point lookAt, Vector up, 
		   float vfov, float aspect, float aperture, float focalDistance,
		   float time0, float time1);
	/*
		[0,1]的坐标
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