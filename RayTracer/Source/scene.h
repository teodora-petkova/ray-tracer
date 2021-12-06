//------------------------------------------------------------
// Cscene.h Header
//------------------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "tuple.h"
#include "light.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"

class Scene
{
public:
	Scene()
	{
		this->width = 600;
		this->height = 400;

		this->camera = Camera::Camera(this->width, this->height);
		this->objects = std::vector<ObjectPtr>();
		this->lights = std::vector<LightPtr>();
	}

	Scene(int width, int height, const Camera& camera,
		std::vector<ObjectPtr> objects, std::vector<LightPtr> lights) :
		width(width),
		height(height),
		camera(camera),
		objects(objects),
		lights(lights)
	{}

	~Scene()
	{
		objects.clear();
		lights.clear();
	}

	int getImageWidth() const { return width; }
	int getImageHeight() const { return height; }

	const std::vector<ObjectPtr>& getObjects() const { return objects; }
	const std::vector<LightPtr>& getLights() const { return lights; }

	const Camera& getCamera() const { return camera; }
	Camera& getModifiableCamera() { return camera; }

	Color TraceSingleRay(const Ray& ray) const
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

	bool IsInShadow(IntersectionInfo intersection, Tuple lightPosition) const
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

private:
	int width;
	int height;

	Camera camera;
	std::vector<ObjectPtr> objects;
	std::vector<LightPtr> lights;
};

#endif