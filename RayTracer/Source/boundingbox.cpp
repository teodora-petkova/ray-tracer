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
	float tmin = min - rayOrigin;
	float tmax = max - rayOrigin;

	float const oneOverDirection = 1.f / rayDirection;

	if (oneOverDirection == INFINITY)
	{
		// 0 * infinity is NaN by C++ standard
		// here we want it to be -inf/inf respectively
		tmin = tmin != 0 ? tmin * oneOverDirection : -INFINITY;
		tmax = tmax != 0 ? tmax * oneOverDirection : INFINITY;
	}
	else
	{
		tmin = tmin * oneOverDirection;
		tmax = tmax * oneOverDirection;
	}

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

	return (tmin <= tmax);
}

std::pair<BoundingBox, BoundingBox> BoundingBox::Split() const
{
	float dx = abs(this->min.X()) + abs(this->max.X());
	float dy = abs(this->min.Y()) + abs(this->max.Y());
	float dz = abs(this->min.Z()) + abs(this->max.Z());

	float maxrange = std::max({ dx, dy, dz });

	BoundingBox left, right;

	if (maxrange == dx)
	{
		float splitx = this->min.X() + (dx / 2);
		left = BoundingBox(this->min, Tuple::Point(splitx, this->max.Y(), this->max.Z()));
		right = BoundingBox(Tuple::Point(splitx, this->min.Y(), this->min.Z()), this->max);
	}
	else if (maxrange == dy)
	{
		float splity = this->min.Y() + (dy / 2);
		left = BoundingBox(this->min, Tuple::Point(this->max.X(), splity, this->max.Z()));
		right = BoundingBox(Tuple::Point(this->min.X(), splity, this->min.Z()), this->max);
	}
	else
	{
		float splitz = this->min.Z() + (dz / 2);
		left = BoundingBox(this->min, Tuple::Point(this->max.X(), this->max.Y(), splitz));
		right = BoundingBox(Tuple::Point(this->min.X(), this->min.Y(), splitz), this->max);
	}

	return std::make_pair(left, right);
}

bool BoundingBox::operator==(const BoundingBox& bb2) const
{
	return this->min == bb2.getMin() && this->max == bb2.getMax();
}

std::ostream& operator<<(std::ostream& os, BoundingBox const& bb)
{
	os << "[ min: " << bb.getMin() << ", max: " << bb.getMax() << "]";
	return os;
}