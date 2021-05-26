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

		// Camera
		Tuple lookFrom = Tuple::Vector(0.0, 0.0, 5.0); // initial eye position, also for resets
		Tuple lookAt = Tuple::Vector(0.0, 1.0, 0.0); // initial up position, also for resets
		Tuple up = Tuple::Vector(0.0, 0.0, 0.0); // center look at point 
		float fovY = 90.0; // field of view
		this->camera = Camera::Camera(lookFrom, lookAt, up, fovY,
			this->width, this->height);

		// Objects
		this->objects = std::vector<ObjectPtr>();

		// lights
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

private:
	int width;
	int height;

	Camera camera;
	std::vector<ObjectPtr> objects;
	std::vector<LightPtr> lights;
};

#endif