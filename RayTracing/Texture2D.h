#pragma once
#include"Assertion.h"
#include<vector>
using std::vector;

template<typename Element>
class Texture2D {
public:
	Texture2D(int w, int h)
		: width(w), height(h), elements(w * h, Element()) {
		assertion(w > 0);
		assertion(h > 0);
	}

	int GetWidth() const {
		return width;
	}

	int GetHeight() const {
		return height;
	}

	Element GetImagePoint(int x, int y) const {
		assertion(x > 0 && x < width);
		assertion(y > 0 && y < height);
		int index = y * width + x;
		return elements[index];
	}

	void SetImagePoint(int x, int y, Element element) {
		assertion(x > 0 && x < width);
		assertion(y > 0 && y < height);
		int index = y * width + x;
		elements[index] = element;
	}

	Element GetScreenPoint(int x, int y) const {
		assertion(x > 0 && x < width);
		assertion(y > 0 && y < height);
		int index = (height - 1 - y) * width + x;
		return elements[index];
	}

	void SetScreenPoint(int x, int y, Element element) {
		assertion(x > 0 && x < width);
		assertion(y > 0 && y < height);
		int index = (height - 1 - y) * width + x;
		elements[index] = element;
	}
private:
	int width;
	int height;
	vector<Element> elements;
};