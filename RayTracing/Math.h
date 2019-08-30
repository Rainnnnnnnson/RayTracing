#pragma once
#include <array>
using std::array;
/*
	�ڹ���׷������ ����Ҫ�õ����� �Լ�Vector4
	�ϸ���Ŀ�� ʹ�ýṹ�� ���ںŵ��� �ܶ�ʱ�򲻺���
	������Ų���ȫ��ʱ��Ҳ������ʹ��
	��ν��Ჹȫ
*/

class Point;
class Vector;
class Color;
class Ray;

constexpr float PI = 3.1415926f;

/*
	���̺߳���
	����[0,1]�������
	�����ڵ�һ��ʹ�õ�ʱ���ʼ��
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
	������ҪЯ��ʱ��  ��Ȼʹ��������ᵼ��  ͬһ�����岻ͬλ��
	Vector �����ʱ��֤��Ϊ��λ����
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
	�÷��صĲ��ǵ�λ����
*/
Vector RandomInUnitSphere();