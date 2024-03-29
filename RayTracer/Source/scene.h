#pragma once

#pragma warning(push, 0)
#include <vector>
#include <stack>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "tuple.h"
#include "light.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "cube.h"
#include "camera.h"

class RAYTRACER_EXPORT Scene
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

	Color TraceSingleRay(const Ray& ray, int remaining = 4) const;

	Color ReflectRay(const Ray& ray, IntersectionInfo intersection,
		int remaining = 4) const;

	Color RefractRay(const Ray& ray, IntersectionInfo intersection,
		float n1, float n2, int remaining) const;

	float Schlick(const Ray& ray, IntersectionInfo intersection, float n1, float n2) const;

	bool IsInShadow(Tuple intersectionPoint, Tuple lightPosition) const;

	std::pair<float, float> getRefractiveIndices(IntersectionInfo intersection,
		const std::vector<std::pair<float, ObjectConstPtr>>& allIntersections) const;

private:
	int width;
	int height;

	Camera camera;
	std::vector<ObjectPtr> objects;
	std::vector<LightPtr> lights;

	Color CalculateColor(IntersectionInfo intersection, const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>> allIntersectionDistances, int remaining) const;
};