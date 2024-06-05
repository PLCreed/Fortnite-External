#pragma once
#include <d3d9.h>
#include <vector>
#define M_PI 3.14159265358979323846264338327950288419716939937510

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(double _x, double _y) : x(_x), y(_y) {}
	~Vector2() {}
	double x;
	double y;
};

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}
	double x;
	double y;
	double z;
	inline double Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}
	inline double Distance(Vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}
	inline int distance(Vector3 v)
	{
		return sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0));
	}
	inline double Length()
	{
		return sqrt(x * x + y * y + z * z);
	}
	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator*(double v)
	{
		return Vector3(x * v, y * v, z * v);
	}

	Vector3 calc_angle(const Vector3& src, const Vector3& dst) {
		Vector3 srca = { src.x, src.y, src.z };
		Vector3 dsta = { dst.x, dst.y, dst.z };

		Vector3 angle = Vector3();
		Vector3 delta = srca - dsta;

		double hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y));
		angle.x = (float)(atan(delta.z / hyp) * 57.295779513082f);
		angle.y = (float)(atan(delta.y / delta.x) * 57.295779513082f);
		if (delta.x >= 0.0) angle.y += 180.0f;

		return angle;
	}
};