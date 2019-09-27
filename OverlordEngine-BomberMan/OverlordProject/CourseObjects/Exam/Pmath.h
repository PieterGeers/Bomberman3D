#pragma once
#include <cmath>

struct PVector2
{
	PVector2() : x(0.0f), y(0.0f){}
	PVector2(float x_, float y_) : x(x_), y(y_){}

	float Dot(const PVector2& p) const { return x * p.x + y * p.y; }
	float Length() const { return std::sqrt(x*x + y * y); }
	float AngleBetweenInRad(const PVector2& p) const
	{
		if (Length() == 0.0f || p.Length() == 0.0f)
			return 0.0f;
		return acos(Dot(p) / (Length() * p.Length()));
	}

	float x, y;
};