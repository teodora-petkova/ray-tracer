#pragma once

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#define degreesToRadians(x) x*(3.141592f/180.0f)

#define MAX(a,b) a < b ? b : a

constexpr float EPSILON = 0.0001f;

inline bool isCloseToZero(float num)
{
	return std::fabs(num) < EPSILON;
}

inline bool equals(float a, float b)
{
	if (std::isinf(a) || std::isinf(b))
	{
		return a == b;
	}

	return isCloseToZero(a - b);
}