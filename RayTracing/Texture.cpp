#include "Texture.h"
#include "Math.h"

ConstantTexture::ConstantTexture(Color c) : color(c) {}

Color ConstantTexture::Value(Point p) const {
	return color;
}

CheckerTexture::CheckerTexture(shared_ptr<Texture> odd, shared_ptr<Texture> even, float distance)
	: odd(std::move(odd)), even(std::move(even)), distance(distance) {}

Color CheckerTexture::Value(Point p) const {
	float sines = sin(PI * p.X() / distance) * sin(PI * p.Y() / distance) * sin(PI * p.Z() / distance);
	if (sines < 0.0F) {
		return odd->Value(p);
	} else {
		return even->Value(p);
	}
}

NoiseTexture::NoiseTexture(float scale) : scale(scale) {}

array<Vector, 256> InitVectorRandomArray() {
	array<Vector, 256> result;
	for (auto& v : result) {
		v = RamdomInUnitSphere();
	}
	return result;
}

array<int, 256> InitIntRandomArray() {
	array<int, 256> result;
	for (int i = 0; i < 256; i++) {
		result[i] = i;
	}
	for (int i = 0; i < 256; i++) {
		int v = static_cast<int>(Random() * static_cast<float>(i));
		std::swap(result[i], result[v]);
	}
	return result;
}

float Noise(Point p) {
	static array<Vector, 256> vectorRandomArray = InitVectorRandomArray();
	static array<int, 256> permX = InitIntRandomArray();
	static array<int, 256> permY = InitIntRandomArray();
	static array<int, 256> permZ = InitIntRandomArray();

	float u = p.X() - floor(p.X());
	float v = p.Y() - floor(p.Y());
	float w = p.Z() - floor(p.Z());
	int i = static_cast<int>(floor(p.X()));
	int j = static_cast<int>(floor(p.Y()));
	int k = static_cast<int>(floor(p.Z()));
	float uu = u * u * (3.0f - 2.0f * u);
	float vv = v * v * (3.0f - 2.0f * v);
	float ww = w * w * (3.0f - 2.0f * w);
	float accum = 0.0f;
	for (int di = 0; di < 2; di++) {
		for (int dj = 0; dj < 2; dj++) {
			for (int dk = 0; dk < 2; dk++) {
				auto c = vectorRandomArray[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]];
				Vector weight(u - static_cast<float>(di), v - static_cast<float>(dj), w - static_cast<float>(dk));
				float value =
					(di * uu + (1.0f - static_cast<float>(di)) * (1.0f - uu)) *
					(dj * vv + (1.0f - static_cast<float>(dj)) * (1.0f - vv)) *
					(dk * ww + (1.0f - static_cast<float>(dk)) * (1.0f - ww)) *
					c.Dot(weight);
				accum += value;
			}
		}
	}
	return 0.5f * (accum + 1.0f);
}

Color NoiseTexture::Value(Point p) const {
	float value = Noise(p);
	return Color(value, value, value);
}

TurbulenceTexture::TurbulenceTexture(float scale, float k1, float k2, int depth)
	: scale(scale), k1(k1), k2(k2), depth(depth) {}

Color TurbulenceTexture::Value(Point p) const {
	float accum = 0.0f;
	Point tempP = scale * p;
	float weight = 1.0f;
	for (int i = 0; i < depth; i++) {
		accum += weight * Noise(tempP);
		weight *= 0.5f;
		tempP * 2.0f;
	}
	accum = abs(accum);
	float t = (1.0f + sin(k1 * p.X() + k2 * accum)) * 0.5f;
	return Color(t, t, t);
}
