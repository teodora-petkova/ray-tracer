#include "boundingbox.h"

void BoundingBox::AddPoint(Tuple p)
{
	this->min = Tuple::Point(
		std::min(this->min.X(), p.X()),
		std::min(this->min.Y(), p.Y()),
		std::min(this->min.Z(), p.Z()));

	this->max = Tuple::Point(
		std::max(this->max.X(), p.X()),
		std::max(this->max.Y(), p.Y()),
		std::max(this->max.Z(), p.Z()));
}

void BoundingBox::AddBound(BoundingBox b)
{
	this->AddPoint(b.getMax());
	this->AddPoint(b.getMin());
}

bool BoundingBox::Contains(const Tuple& point) const
{
	return (min.X() <= point.X() && point.X() <= max.X() &&
		min.Y() <= point.Y() && point.Y() <= max.Y() &&
		min.Z() <= point.Z() && point.Z() <= max.Z());
}

bool BoundingBox::Contains(const BoundingBox& bb) const
{
	return this->Contains(bb.getMin()) &&
		this->Contains(bb.getMax());
}

BoundingBox BoundingBox::Transform(const Matrix<4, 4>& matrix)
{
	Tuple p1 = this->min;
	Tuple p2 = Tuple::Point(this->min.X(), this->min.Y(), this->max.Z());
	Tuple p3 = Tuple::Point(this->min.X(), this->max.Y(), this->min.Z());
	Tuple p4 = Tuple::Point(this->min.X(), this->max.Y(), this->max.Z());
	Tuple p5 = Tuple::Point(this->max.X(), this->min.Y(), this->min.Z());
	Tuple p6 = Tuple::Point(this->max.X(), this->min.Y(), this->max.Z());
	Tuple p7 = Tuple::Point(this->max.X(), this->max.Y(), this->min.Z());
	Tuple p8 = this->max;

	auto bbox = BoundingBox();
	const Tuple points[8] = { p1, p2, p3, p4, p5, p6, p7, p8 };
	for (const Tuple& p : points)
	{
		auto pp = matrix * p;
		bbox.AddPoint(pp);
	}

	return bbox;
}

std::pair<float, float> getIntersectionDistancesAtAxis(float rayOrigin, float rayDirection, float min, float max)
{
	//float tmin = (min - rayOrigin) * INFINITY;
	//float tmax = (max - rayOrigin) * INFINITY;

	//if (!isCloseToZero(rayDirection))
	//{
	float const oneOverDirection = 1.f / rayDirection;

	float tmin = (max - rayOrigin) * oneOverDirection;
	float tmax = (min - rayOrigin) * oneOverDirection;
	//}

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}

	return std::make_pair(tmin, tmax);
}

bool BoundingBox::Intersects(const Ray& ray, float& tmin, float& tmax) const
{
	auto xt = getIntersectionDistancesAtAxis(ray.getOrigin().X(), ray.getDirection().X(),
		this->min.X(), this->max.X());
	auto yt = getIntersectionDistancesAtAxis(ray.getOrigin().Y(), ray.getDirection().Y(),
		this->min.Y(), this->max.Y());
	auto zt = getIntersectionDistancesAtAxis(ray.getOrigin().Z(), ray.getDirection().Z(),
		this->min.Z(), this->max.Z());

	tmin = std::max({ xt.first, yt.first, zt.first });
	tmax = std::min({ xt.second, yt.second, zt.second });

	return (tmin < tmax);
}

std::ostream& operator<<(std::ostream& os, BoundingBox const& bb)
{
	os << "[ min: " << bb.getMin() << ", max: " << bb.getMax() << "]";
	return os;
}