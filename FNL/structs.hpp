#pragma once

struct DGVec2
{
	float x, y;

	DGVec2() : x(0), y(0) {}
	DGVec2(const float& _x, const float& _y) : x(_x), y(_y) {}
};

struct DGVec3
{
	float x, y, z;

	DGVec3() : x(0), y(0), z(0) {}
	DGVec3(const float& _x, const float& _y, const float& _z) : x(_x), y(_y), z(_z) {}
};

struct DGColor
{
	float r, g, b, a;

	DGColor() : r(0.f), g(0.f), b(0.f), a(0.f) {}
	DGColor(const float& _r, const float& _g, const float& _b, const float& _a) : r(_r), g(_g), b(_b), a(_a)
	{
		if (_r > 1.f)
			r = _r / 255;
		if (_g > 1.f)
			g = _g / 255;
		if (_b > 1.f)
			b = _b / 255;
		if (_a > 1.f)
			a = _a / 255;

	}

	DGColor(const float& _r, const float& _g, const float& _b)
	{
		if (_r > 1.f)
			r = _r / 255;
		if (_g > 1.f)
			g = _g / 255;
		if (_b > 1.f)
			b = _b / 255;
		a = 1.f;

	}

	DGColor(const int& _r, const int& _g, const int& _b, const int& _a)
	{
		float __r = _r;
		float __g = _g;
		float __b = _b;
		float __a = _a;

		r = __r / 255;
		g = __g / 255;
		b = __b / 255;
		a = __a / 255;
	}

	DGColor(const int& _r, const int& _g, const int& _b)
	{
		float __r = _r;
		float __g = _g;
		float __b = _b;

		r = __r / 255;
		g = __g / 255;
		b = __b / 255;
		a = 1.f;
	}
};