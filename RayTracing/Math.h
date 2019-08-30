#pragma once
#include <array>
using std::array;
/*
	在光线追踪器中 不需要用到矩阵 以及Vector4
	上个项目中 使用结构体 花口号调用 很多时候不好用
	运算符号不完全有时候也不方便使用
	这次将会补全
*/

class Point;
class Vector;
class Color;
class Ray;

constexpr float PI = 3.1415926f;

/*
	多线程函数
	生成[0,1]的随机数
	将会在第一次使用的时候初始化
*/
float Random();

Point operator*(float t, Point p);
Vector operator*(float t, Vector v);
Color operator*(float t, Color c);

class Point {
public:
	Point();
	Point(float x, float y, float z);

	float X() const;
	float Y() const;
	float Z() const;
	float& X();
	float& Y();
	float& Z();
	float operator[](int index) const;
	float& operator[](int index);

	Point operator+(Point v) const;
	Vector operator-(Point v) const;

	Point operator+(Vector v) const;
	Point operator-(Vector v) const;
	
	Point operator*(float t) const;
	Point operator/(float t) const;
private:
	array<float, 3> element;
};

class Vector {
public:
	Vector();
	Vector(float x, float y, float z);

	float X() const;
	float Y() const;
	float Z() const;
	float& X();
	float& Y();
	float& Z();
	float operator[](int index) const;
	float& operator[](int index);

	Vector operator+(Vector v) const;
	Vector operator-(Vector v) const;
	Vector operator-() const;
	Vector operator*(float t) const;
	Vector operator/(float t) const;

	float Dot(Vector v) const;
	Vector Cross(Vector v) const;
	float Length() const;
	Vector Normalize() const;
private:
	array<float, 3> element;
};

class Color {
public:
	Color();
	Color(float r, float g, float b);

	float R() const;
	float G() const;
	float B() const;
	float& R();
	float& G();
	float& B();
	float operator[](int index) const;
	float& operator[](int index);

	Color operator+(Color v) const;
	Color operator-(Color v) const;
	Color operator*(Color v) const;
	Color operator/(Color v) const;
	Color operator*(float t) const;
	Color operator/(float t) const;

private:
	array<float, 3> element;
};

/*
	光线需要携带时间  不然使用随机数会导致  同一个物体不同位置
	Vector 构造的时候保证其为单位向量
*/
class Ray {
public:
	Ray();
	Ray(Point origin, Vector direction, float time);

	Point PointAtParamter(float t) const;
	Point Origin() const;
	Vector Direction() const;
	float Time() const;
private:
	Point origin;
	Vector direction;
	float time;
};

/*
	该返回的不是单位向量
*/
Vector RandomInUnitSphere();