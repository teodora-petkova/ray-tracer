#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include <math.h>
#include <vector>

#include "source\raytracer.h"

class RaytracerTests : public ::testing::Test
{
protected:
	Scene scene;
	Camera camera;
	LightPtr light;
	ObjectPtr sphere1;
	ObjectPtr sphere2;

	RaytracerTests() {
	}

	virtual ~RaytracerTests() {
	}

protected:
	void SetUp() override {

		Tuple from = Tuple::Point(0, 0, -5);
		Tuple up = Tuple::Point(0, 1, 0);
		Tuple to = Tuple::Point(0, 0, 1);

		this->camera = Camera(from, to, up, 90, 11, 11);

		this->light = std::make_shared<Light>(Tuple::Point(-10, 10, -10),
			Color::White(), 1, 1, 1, 1);

		MaterialPtr m1 = std::make_shared<Material>(
			std::make_shared<FlatColor>(Color(0.8f, 1.0f, 0.6f)),
			0.1f, 0.7f, 0.2f, 200.f, 0);
		this->sphere1 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
			m1, Matrix<4, 4>::IdentityMatrix());

		MaterialPtr m2 = std::make_shared<Material>(
			std::make_shared<FlatColor>(Color::White()),
			1, 1, 1, 1, 0);
		this->sphere2 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
			m2, Transformations::Scaling(0.5, 0.5, 0.5));

		std::vector<LightPtr> lights{ this->light };
		std::vector<ObjectPtr> objects{ this->sphere1, this->sphere2 };
		this->scene = Scene(11, 11, this->camera, objects, lights);
	}
};

TEST_F(RaytracerTests, Render_default_world) {

	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));
	RayTracer rayTracer = RayTracer();
	Canvas canvas = rayTracer.TraceRays(this->scene);

	EXPECT_EQ(canvas.PixelAt(5, 5), Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_F(RaytracerTests, The_color_when_the_ray_hits_the_sphere_in_the_default_world) {
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));
	Color color = this->scene.TraceSingleRay(ray);

	EXPECT_EQ(color, Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_F(RaytracerTests, The_color_when_the_ray_misses_the_sphere_in_the_default_world) {
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 1, 0));
	Color color = this->scene.TraceSingleRay(ray);

	EXPECT_EQ(color, Color::Black());
}

TEST_F(RaytracerTests, NO_shadow_when_there_is_nothing_between_the_intersection_point_and_the_light) {
	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(Tuple::Point(0, 10, 0), light->getPosition());

	EXPECT_EQ(isInShadow, false);
}

TEST_F(RaytracerTests, Shadow_when_there_is_an_object_between_the_intersection_point_and_the_light) {
	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(Tuple::Point(10, -10, 10), light->getPosition());

	EXPECT_EQ(isInShadow, true);
}

TEST_F(RaytracerTests, NO_shadow_when_an_object_is_behind_the_light) {
	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(Tuple::Point(-2, 2, -2), light->getPosition());

	EXPECT_EQ(isInShadow, false);
}

TEST_F(RaytracerTests, NO_shadow_when_an_object_is_behind_the_intersection_point) {
	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(Tuple::Point(-2, 2, -2), light->getPosition());

	EXPECT_EQ(isInShadow, false);
}

TEST_F(RaytracerTests, Camera_is_between_two_spheres_looking_at_the_one_in_shadow)
{
	LightPtr light = std::make_shared<Light>(Tuple::Point(0, 0, -10),
		Color::White(), 1, 1, 1, 1);

	ObjectPtr s1 = std::make_shared<Sphere>();
	ObjectPtr s2 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(),
		Transformations::Translation(0, 0, 10));

	Ray ray = Ray(Tuple::Point(0, 0, 5), Tuple::Vector(0, 0, 1));

	Camera camera = Camera(Tuple::Point(0, 0, 5),
		Tuple::Point(0, 1, 0),
		Tuple::Point(0, 0, 1),
		90, 11, 11);

	std::vector<LightPtr> lights{ light };
	std::vector<ObjectPtr> objects{ s1, s2 };
	Scene customScene = Scene(11, 11, camera, objects, lights);

	Color color = customScene.TraceSingleRay(ray);

	EXPECT_EQ(color, Color(0.1f, 0.1f, 0.1f));
}

TEST_F(RaytracerTests, The_pure_reflected_color_for_a_reflected_material_of_a_plane)
{
	ObjectPtr plane = std::make_shared<Plane>(
		std::make_shared<Material>(std::make_shared<FlatColor>(),
			0.1, 0.9, 0.9, 200, 0.5),
		Transformations::Translation(0, -1, 0));

	Camera camera = Camera(Tuple::Point(0, 0, -5),
		Tuple::Point(0, 1, 0),
		Tuple::Point(0, 0, 1), 90, 11, 11);

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ this->sphere1, this->sphere2, plane };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	float sqrt2over2 = sqrt(2) / 2;
	Ray ray = Ray(Tuple::Point(0, 0, -3),
		Tuple::Vector(0, -sqrt2over2, sqrt2over2));

	IntersectionInfo i = plane->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());
	Color color = customScene.ReflectRay(ray, plane, i);

	EXPECT_EQ(color, Color(0.19032f, 0.2379f, 0.14274f));
}

TEST_F(RaytracerTests, The_accumulated_reflected_color_for_a_reflected_material_of_a_plane)
{
	ObjectPtr plane = std::make_shared<Plane>(
		std::make_shared<Material>(std::make_shared<FlatColor>(),
			0.1, 0.9, 0.9, 200, 0.5),
		Transformations::Translation(0, -1, 0));

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ this->sphere1, this->sphere2, plane };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	float sqrt2over2 = sqrt(2) / 2;
	Ray ray = Ray(Tuple::Point(0, 0, -3),
		Tuple::Vector(0, -sqrt2over2, sqrt2over2));

	Color color = customScene.TraceSingleRay(ray);

	EXPECT_EQ(color, Color(0.87677f, 0.92436f, 0.82918f));
}

TEST_F(RaytracerTests, The_accumulated_reflected_color_for_a_reflected_material_of_a_plane2)
{
	Camera c = Camera(Tuple::Point(0, 0, 0),
		Tuple::Point(0, 1, 0),
		Tuple::Point(0, 0, 1), 90, 11, 11);

	LightPtr l = std::make_shared<Light>(Tuple::Point(0, 0, 0),
		Color::White(), 1, 1, 1, 1);

	MaterialPtr m = std::make_shared<Material>(
		std::make_shared<FlatColor>(),
		0.1f, 0.9f, 0.9f, 200.f, 1);

	ObjectPtr lowerPlane = std::make_shared<Plane>(m,
		Transformations::Translation(0, -1, 0));
	ObjectPtr upperPlane = std::make_shared<Plane>(m,
		Transformations::Translation(0, 1, 0));

	std::vector<LightPtr> lights{ l };
	std::vector<ObjectPtr> objects{ lowerPlane, upperPlane };
	Scene customScene = Scene(11, 11, c, objects, lights);

	Ray ray = Ray(Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 1, 0));

	Color color = customScene.TraceSingleRay(ray);

	// TODO: the color RGB components are above 1 because of summation during recursion
	// to find a valid way to overcome that
	EXPECT_EQ(color, Color(9.5f, 9.5f, 9.5f));
}

TEST_F(RaytracerTests, The_reflected_color_at_the_maximum_recursive_depth)
{
	ObjectPtr plane = std::make_shared<Plane>(
		std::make_shared<Material>(std::make_shared<FlatColor>(),
			0.1, 0.9, 0.9, 200, 0.5),
		Transformations::Translation(0, -1, 0));

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ this->sphere1, this->sphere2, plane };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	float sqrt2over2 = sqrt(2) / 2;
	Ray ray = Ray(Tuple::Point(0, 0, -3),
		Tuple::Vector(0, -sqrt2over2, sqrt2over2));

	IntersectionInfo i = plane->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());
	Color color = customScene.ReflectRay(ray, plane, i, 0);

	EXPECT_EQ(color, Color(0.f, 0.f, 0.f));
}