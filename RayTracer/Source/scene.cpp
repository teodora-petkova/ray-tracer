//---------------------------------------------------------------------
// scene.cpp
//---------------------------------------------------------------------
#include "scene.h"

Color Scene::TraceSingleRay(const Ray& ray) const
{
	Scene scene = *this;

	ObjectPtr object = NULL;
	IntersectionInfo intersection = IntersectionInfo();

	float minDistance = INFINITY;
	for (unsigned int i = 0; i < scene.getObjects().size(); i++)
	{
		ObjectPtr testObject = scene.getObjects()[i];

		IntersectionInfo testIntersection = testObject->Intersect(ray);

		if (testIntersection.getIsHit() && testObject && testIntersection.getDistance() < minDistance)
		{
			object = testObject;
			intersection = testIntersection;
			minDistance = intersection.getDistance();
		}
	}

	Color color = Color::Black();
	if (intersection.getIsHit())
	{
		for (LightPtr light : scene.getLights())
		{
			color += light->CalculatePhongColor(intersection.getIntersectionPoint(),
				intersection.getNormal(),
				scene.getCamera().getOrigin(),
				object->getMaterial(),
				IsInShadow(intersection, light->getPosition()));
		}
	}
	return color;
}

bool Scene::IsInShadow(IntersectionInfo intersection, Tuple lightPosition) const
{
	float bias = 0.001f;
	Tuple vectorFromIntersectionToLight = lightPosition - intersection.getIntersectionPoint();
	Ray rayFromIntersectionToLight = Ray(
		intersection.getIntersectionPoint() + intersection.getNormal() * bias,
		// TODO: to check why it does not cause acne without the bias in the normal direction!!
		vectorFromIntersectionToLight.Normalize());

	bool isInShadow = false;
	for (unsigned int i = 0; i < this->objects.size(); i++)
	{
		ObjectPtr testObject = objects[i];

		IntersectionInfo testIntersection = testObject->Intersect(rayFromIntersectionToLight);
		if (testIntersection.getIsHit() &&
			testIntersection.getDistance() < vectorFromIntersectionToLight.Magnitude())
		{
			isInShadow = true;
			break;
		}
	}

	return isInShadow;
}