#include "Math.h"
#include <cmath>
#include <random>

float Random() {
	struct RandomClass {
		RandomClass() : rd(), gen(rd()), dis(0.0f, 1.0f) {}
		std::random_device rd;  // 将用于获得随机数引擎的种子
		std::mt19937 gen; // 以 rd() 播种的标准 mersenne_twister_engine
		std::uniform_real_distribution<float> dis;
	};
	static thread_local RandomClass random;
	return random.dis(random.gen);
}

Point operator*(float t, Point p) {
	return p * t;
}

Vector operator*(float t, Vector v) {
	return v * t;
}

Color operator*(float t, Color c) {
	return c * t;
}

Point::Point() : element{0.0f, 0.0f, 0.0f} {}

Point::Point(float x, float y, float z) : element{x, y, z} {}

float Point::X() const {
	return element[0];
}

float Point::Y() const {
	return element[1];
}

float Point::Z() const {
	return element[2];
}

float& Point::X() {
	return element[0];
}

float& Point::Y() {
	return element[1];
}

float& Point::Z() {
	return element[2];
}

float Point::operator[](int index) const {
	return element[index];
}

float& Point::operator[](int index) {
	return element[index];
}

Point Point::operator+(Point v) const {
	return Point(element[0] + v[0], element[1] + v[1], element[2] + v[2]);
}

Vector Point::operator-(Point v) const {
	return Vector(element[0] - v[0], element[1] - v[1], element[2] - v[2]);
}

Point Point::operator+(Vector v) const {
	return Point(element[0] + v[0], element[1] + v[1], element[2] + v[2]);
}

Point Point::operator-(Vector v) const {
	return Point(element[0] - v[0], element[1] - v[1], element[2] - v[2]);
}

Point Point::operator*(float t) const {
	return Point(element[0] * t, element[1] * t, element[2] * t);
}

Point Point::operator/(float t) const {
	return Point(element[0] / t, element[1] / t, element[2] / t);
}

Vector::Vector() : element{0.0f, 0.0f, 0.0f} {}

Vector::Vector(float x, float y, float z) : element{x, y, z} {}

float Vector::X() const {
	return element[0];
}

float Vector::Y() const {
	return element[1];
}

float Vector::Z() const {
	return element[2];
}

float& Vector::X() {
	return element[0];
}

float& Vector::Y() {
	return element[1];
}

float& Vector::Z() {
	return element[2];
}

float Vector::operator[](int index) const {
	return element[index];
}

float& Vector::operator[](int index) {
	return element[index];
}

Vector Vector::operator+(Vector v) const {
	return Vector(element[0] + v[0], element[1] + v[1], element[2] + v[2]);
}

Vector Vector::operator-(Vector v) const {
	return Vector(element[0] - v[0], element[1] - v[1], element[2] - v[2]);
}

Vector Vector::operator-() const {
	return Vector(-element[0], -element[1], -element[2]);
}

Vector Vector::operator*(float t) const {
	return Vector(element[0] * t, element[1] * t, element[2] * t);
}

Vector Vector::operator/(float t) const {
	return Vector(element[0] / t, element[1] / t, element[2] / t);
}

float Vector::Dot(Vector v) const {
	return element[0] * v[0] + element[1] * v[1] + element[2] * v[2];
}

Vector Vector::Cross(Vector v) const {
	return Vector(
		element[1] * v[2] - element[2] * v[1],
		element[2] * v[0] - element[0] * v[2],
		element[0] * v[1] - element[1] * v[0]
	);
}

float Vector::Length() const {
	return sqrt(this->Dot(*this));
}

Vector Vector::Normalize() const {
	return *this / Length();
}

Color::Color() : element{0.0f, 0.0f, 0.0f} {}

Color::Color(float r, float g, float b) : element{r, g, b} {}

float Color::R() const {
	return element[0];
}

float Color::G() const {
	return element[1];
}

float Color::B() const {
	return element[2];
}

float& Color::R() {
	return element[0];
}

float& Color::G() {
	return element[1];
}

float& Color::B() {
	return element[2];
}

float Color::operator[](int index) const {
	return element[index];
}

float& Color::operator[](int index) {
	return element[index];
}

Color Color::operator+(Color v) const {
	return Color(element[0] + v[0], element[1] + v[1], element[2] + v[2]);
}

Color Color::operator-(Color v) const {
	return Color(element[0] - v[0], element[1] - v[1], element[2] - v[2]);
}

Color Color::operator*(Color v) const {
	return Color(element[0] * v[0], element[1] * v[1], element[2] * v[2]);
}

Color Color::operator/(Color v) const {
	return Color(element[0] / v[0], element[1] / v[1], element[2] / v[2]);
}

Color Color::operator*(float t) const {
	return Color(element[0] * t, element[1] * t, element[2] * t);
}

Color Color::operator/(float t) const {
	return Color(element[0] / t, element[1] / t, element[2] / t);
}

Ray::Ray() : time(0.0f) {}

Ray::Ray(Point origin, Vector direction, float time) : origin(origin), direction(direction), time(time) {}

Point Ray::PointAtParamter(float t) const {
	return origin + t * direction;
}

Point Ray::Origin() const {
	return origin;
}

Vector Ray::Direction() const {
	return direction;
}

float Ray::Time() const {
	return time;
}

Vector RamdomInUnitSphere() {
	Vector p;
	do {
		p = 2.0f * Point(Random(), Random(), Random()) - Point(1.0f, 1.0f, 1.0f);
	} while (p.Length() >= 1.0f);
	return p;
}