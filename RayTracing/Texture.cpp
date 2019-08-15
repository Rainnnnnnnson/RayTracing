#include "Texture.h"

ConstantTexture::ConstantTexture(Color c) : color(c) {}

Color ConstantTexture::Value(Point p) const {
	return color;
}

CheckerTexture::CheckerTexture(unique_ptr<Texture> odd, unique_ptr<Texture> even, float distance)
	: odd(std::move(odd)), even(std::move(even)), distance(distance) {}

Color CheckerTexture::Value(Point p) const {
	float sines = sin(PI * distance * p.X()) * sin(PI * distance * p.Y()) * sin(PI * distance * p.Z());
	if (sines < 0.0F) {
		return odd->Value(p);
	} else {
		return even->Value(p);
	}
}

NoiseTexture::NoiseTexture(float scale) : scale(scale) {}

Color NoiseTexture::Value(Point p) const {
	throw std::exception("TODO");
}
