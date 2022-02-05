#pragma once

#include "triangle.h"

class RAYTRACER_EXPORT SmoothTriangle : public virtual Triangle
{
public:
	SmoothTriangle(const Tuple& point1, const Tuple& point2, const Tuple& point3,
		const Tuple& normalA, const Tuple& normalB, const Tuple& normalC,
		std::string name = "")
		:Triangle(point1, point2, point3,
			std::make_shared<Material>(),
			Matrix<4, 4>::IdentityMatrix(), name)
	{
		this->normalA = normalA;
		this->normalB = normalB;
		this->normalC = normalC;
	}

	Tuple getNormalA() const { return normalA; }
	Tuple getNormalB() const { return normalB; }
	Tuple getNormalC() const { return normalC; }

	bool operator==(const Object& other) const
	{
		auto otherTriangle = dynamic_cast<SmoothTriangle const*>(&other);
		return this->normalA == otherTriangle->normalA
			&& this->normalB == otherTriangle->normalB
			&& this->normalC == otherTriangle->normalC
			&& Triangle::operator==(other);
	}

private:
	Tuple normalA;
	Tuple normalB;
	Tuple normalC;

	Tuple getLocalNormal(const Tuple& /*point*/, const IntersectionParams& intersection) const override
	{
		return this->normalB * intersection.getU() +
			this->normalC * intersection.getV() +
			this->normalA * (1 - intersection.getU() - intersection.getV());
	}
};