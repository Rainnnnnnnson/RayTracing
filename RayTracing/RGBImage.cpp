#include "RGBImage.h"

RGBColor::RGBColor() : element{0, 0, 0} {}

RGBColor::RGBColor(unsigned char r, unsigned char g, unsigned char b) : element{r, g, b} {}

RGBColor::RGBColor(Color color) : element{0, 0, 0} {
	color = 255.0f * color;
	for (int i = 0; i < 3; i++) {
		element[i] = static_cast<unsigned char>(std::clamp(color[0], 0.0f, 255.0f));
	}
}

unsigned char RGBColor::R() const {
	return element[0];
}

unsigned char RGBColor::G() const {
	return element[1];
}

unsigned char RGBColor::B() const {
	return element[2];
}

unsigned char RGBColor::operator[](int index) const {
	return element[index];
}

unsigned char& RGBColor::operator[](int index) {
	return element[index];
}

Color RGBColor::ToColor() const {
	Color color;
	for (int i = 0; i < 3; i++) {
		color[i] = static_cast<float>(element[i]) / 255.0f;
	}
	return color;
}
