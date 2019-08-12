#pragma once
#include "Texture2D.h"
#include "Math.h"
class RGBColor {
public:
	RGBColor();
	RGBColor(unsigned char r, unsigned char g, unsigned char b);
	RGBColor(Color color);

	unsigned char R() const;
	unsigned char G() const;
	unsigned char B() const;
	unsigned char operator[](int index) const;
	unsigned char& operator[](int index);

	Color ToColor() const;
private:
	array<unsigned char, 3> element;
};

using RGBImage = Texture2D<RGBColor>;

