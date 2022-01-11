#include "scene.h"

static bool IsHitCloser(IntersectionInfo intersection, float minDistance)
{
	return (intersection.getIsHit() &&
		intersection.getDistance() >= EPSILON &&
		intersection.getDistance() < minDistance);
}

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

		if (IsHitCloser(testIntersection, minDistance))
		{
			object = testObject;
			intersection = testIntersection;
			minDistance = intersection.getDistance();
		}
	}

	sort(allIntersectionDistances.begin(), allIntersectionDistances.end());

	return CalculateColor(intersection, ray, object, allIntersectionDistances, remaining);
}

Color Scene::CalculateColor(IntersectionInfo intersection, const Ray& ray, ObjectPtr object,
	std::vector<std::pair<float, ObjectConstPtr>> allIntersectionDistances, int remaining) const
{
	Scene scene = *this;

	Color color = Color::Black();

	if (intersection.getIsHit())
	{
		for (LightPtr light : scene.getLights())
		{
			color += light->CalculatePhongColor(intersection.getOverPoint(),
				intersection.getNormal(), scene.getCamera().getOrigin(), object,
				IsInShadow(intersection.getOverPoint(), light->getPosition()));
		}

		Color reflected = ReflectRay(ray, object, intersection, remaining);

		auto n1n2 = getRefractiveIndices(intersection, allIntersectionDistances);

		Color refracted = RefractRay(ray, object, intersection,
			n1n2.first, n1n2.second, remaining);

		if (object->getMaterial()->getReflective() > 0 &&
			object->getMaterial()->getTransparency() > 0)
		{
			float reflectance = Schlick(ray, intersection, n1n2.first, n1n2.second);
			color += reflected * reflectance + refracted * (1 - reflectance);
		}
		else
		{
			color += reflected + refracted;
		}
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

std::pair<float, float> Scene::getRefractiveIndices(IntersectionInfo intersection,
	const std::vector<std::pair<float, ObjectConstPtr>>& allIntersections) const
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

	return std::make_pair(n1, n2);
}

Color Scene::RefractRay(const Ray& ray, ObjectPtr object,
	IntersectionInfo intersection, float n1, float n2,
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

		// the inverted definition of Snell's Law
		float nratio = n1 / n2;

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

float Scene::Schlick(const Ray& ray, IntersectionInfo intersection,
	float n1, float n2) const
{
	Tuple eyev = -ray.getDirection();
	Tuple normalv = intersection.getNormal();
	float cosi = eyev.Dot(normalv);

	// the inverted definition of Snell's Law
	float nratio = n1 / n2;

	if (n1 > n2)
	{
		//“total internal reflection” means all the light is reflected and none is refracted
		// the fraction of light that is reflected must be 1 in this case => the reflectance
		float n = n1 / n2;
		float sin2t = pow(n, 2) * (1 - pow(cosi, 2));
		if (sin2t > 1)
		{
			return 1;
		}
		float cost = sqrtf(1.f - sin2t);

		cosi = cost;
	}

	float r0 = powf(((n1 - n2) / (n1 + n2)), 2);

	return r0 + (1.f - r0) * powf((1 - cosi), 5);
}

bool Scene::IsInShadow(Tuple intersectionPoint, Tuple lightPosition) const
{
	Tuple vectorFromIntersectionToLight = lightPosition - intersectionPoint;
	Ray rayFromIntersectionToLight = Ray(intersectionPoint,
		vectorFromIntersectionToLight.Normalize());
	float minDistanceToLightSource = vectorFromIntersectionToLight.Magnitude();

	bool isInShadow = false;

	std::vector<std::pair<float, ObjectConstPtr>> allIntersectionDistances;
	for (unsigned int i = 0; i < this->objects.size(); i++)
	{
		ObjectPtr testObject = objects[i];

		IntersectionInfo testIntersection = testObject->Intersect(rayFromIntersectionToLight,
			allIntersectionDistances);
		if (IsHitCloser(testIntersection, minDistanceToLightSource))
		{
			isInShadow = true;
			break;
		}
	}

	return isInShadow;
}