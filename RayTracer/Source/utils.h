//---------------------------------------------------------------------
// utils.h Header
//---------------------------------------------------------------------
#ifndef UTILS_H
#define UTILS_H

#pragma once

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#define degreesToRadians(x) x*(3.141592f/180.0f)

#define MAX(a,b) a < b ? b : a

constexpr float epsilon = 0.0001f;

inline bool equals(float a, float b)
{
	if (std::isinf(a) || std::isinf(b))
	{
		return a == b;
	}

	return std::fabs(a - b) < epsilon;
}
#endif
