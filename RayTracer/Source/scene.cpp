#include "scene.h"

Color Scene::TraceSingleRay(const Ray& ray, int remaining) const
{
	Scene scene = *this;

	ObjectPtr object = NULL;
	IntersectionInfo intersection = IntersectionInfo();
	std::vector<std::pair<float, ObjectConstPtr>> allIntersectionDistances;

	float minDistance = INFINITY;
	for (unsigned int i = 0; i < scene.getObjects().size(); i++)
	{
		ObjectPtr testObject = scene.getObjects()[i];

		IntersectionInfo testIntersection = testObject->Intersect(ray, allIntersectionDistances);

		if (testIntersection.getIsHit() &&
			testObject &&
			testIntersection.getDistance() >= EPSILON &&
			testIntersection.getDistance() < minDistance)
		{
			object = testObject;
			intersection = testIntersection;
			minDistance = intersection.getDistance();
		}
	}
	sort(allIntersectionDistances.begin(), allIntersectionDistances.end());

	Color color = Color::Black();
	if (intersection.getIsHit())
	{
		for (LightPtr light : scene.getLights())
		{
			color += light->CalculatePhongColor(intersection.getOverPoint(),
				intersection.getNormal(),
				scene.getCamera().getOrigin(),
				object,
				IsInShadow(intersection.getOverPoint(), light->getPosition()));
		}

		Color reflected = ReflectRay(ray, object, intersection, remaining);
		Color refracted = RefractRay(ray, object, intersection,
			allIntersectionDistances, remaining);
		color += reflected + refracted;
	}
	return color;
}

Color Scene::ReflectRay(const Ray& ray, ObjectPtr object,
	IntersectionInfo intersection, int remaining) const
{
	if (remaining <= 0 ||
		object->getMaterial()->getReflective() == 0.f)
	{
		return Color::Black();
	}
	else
	{
		Tuple reflectedVector = ray.getDirection().Reflect(intersection.getNormal());

		Ray reflectedRay = Ray(intersection.getOverPoint(), reflectedVector);
		Color color = TraceSingleRay(reflectedRay, remaining - 1);
		return color * object->getMaterial()->getReflective();
	}
}

static float getRefractiveIndicesRatio(IntersectionInfo intersection,
	const std::vector<std::pair<float, ObjectConstPtr>>& allIntersections)
{
	float n1 = 1.f;
	float n2 = 1.f;

	std::vector<ObjectConstPtr> containers; // empty list

	for (auto i : allIntersections)
	{
		if (equals(i.first, intersection.getDistance()))
		{
			if (containers.empty())
				n1 = 1.f;
			else
				n1 = containers[containers.size() - 1]->getMaterial()->getRefractiveIndex();
		}

		auto it = find(containers.begin(), containers.end(), i.second);
		if (it != containers.end())
		{
			containers.erase(it);
		}
		else
		{
			containers.push_back(i.second);
		}

		if (equals(i.first, intersection.getDistance()))
		{
			if (containers.empty())
				n2 = 1.f;
			else
				n2 = containers[containers.size() - 1]->getMaterial()->getRefractiveIndex();
			break;
		}
	}

	// the inverted definition of Snell's Law
	return n1 / n2;
}

Color Scene::RefractRay(const Ray& ray, ObjectPtr object,
	IntersectionInfo intersection,
	const std::vector<std::pair<float, ObjectConstPtr>>& allIntersections,
	int remaining) const
{
	if (remaining == 0 ||
		object->getMaterial()->getTransparency() == 0.f)
	{
		return Color::Black();
	}
	else
	{
		Tuple eyev = -ray.getDirection();
		Tuple normalv = intersection.getNormal();
		float cosi = eyev.Dot(normalv);

		float nratio = getRefractiveIndicesRatio(intersection, allIntersections);
		float sin2t = pow(nratio, 2) * (1 - pow(cosi, 2));

		if (sin2t > 1)
		{
			// a total internal reflection
			return Color::Black();
		}

		float cost = std::sqrt(1 - sin2t);

		Ray refractedRay = Ray(intersection.getUnderPoint(),
			normalv * (nratio * cosi - cost) - eyev * nratio);

		// multiply by the transparency value to account for any opacity
		Color color = TraceSingleRay(refractedRay, remaining - 1) *
			object->getMaterial()->getTransparency();

		return color;
	}
}

bool Scene::IsInShadow(Tuple intersectionPoint, Tuple lightPosition) const
{
	Tuple vectorFromIntersectionToLight = lightPosition - intersectionPoint;
	Ray rayFromIntersectionToLight = Ray(intersectionPoint,
		vectorFromIntersectionToLight.Normalize());

	bool isInShadow = false;
	for (unsigned int i = 0; i < this->objects.size(); i++)
	{
		ObjectPtr testObject = objects[i];

		IntersectionInfo testIntersection = testObject->Intersect(rayFromIntersectionToLight,
			std::vector<std::pair<float, ObjectConstPtr>>());
		if (testIntersection.getIsHit() &&
			testIntersection.getDistance() >= EPSILON &&
			testIntersection.getDistance() < vectorFromIntersectionToLight.Magnitude())
		{
			isInShadow = true;
			break;
		}
	}

	return isInShadow;
}