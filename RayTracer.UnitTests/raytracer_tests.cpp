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

	RaytracerTests() {
	}

	virtual ~RaytracerTests() {
	}

protected:
	void SetUp() override {

		Tuple from = Tuple::Point(0, 0, -5);
		Tuple up = Tuple::Point(0, 1, 0);
		Tuple to = Tuple::Point(0, 0, 1);

		Camera camera = Camera(from, to, up, 90, 11, 11);
		Ray r = camera.CalculateRayForPixel(5, 5);

		LightPtr l = std::make_shared<Light>(Light(Tuple::Point(-10, 10, -10),
			Color::White(), 1, 1, 1, 1));

		MaterialPtr m1 = std::make_shared<Material>(Material(Color(0.8, 1.0, 0.6),
			0.1, 0.7, 0.2, 200));
		ObjectPtr s1 = std::make_shared<Sphere>(Sphere(Tuple::Point(0, 0, 0), 1,
			m1, Matrix<4, 4>::IdentityMatrix()));

		MaterialPtr m2 = std::make_shared<Material>(Material(Color::White(),
			1, 1, 1, 1));
		ObjectPtr s2 = std::make_shared<Sphere>(Sphere(Tuple::Point(0, 0, 0), 1,
			m2, Transformations::Scaling(0.5, 0.5, 0.5)));

		std::vector<LightPtr> lights{ l };
		std::vector<ObjectPtr> objects{ s1, s2 };
		this->scene = Scene(11, 11, camera, objects, lights);
	}
};

TEST_F(RaytracerTests, Render_default_world) {

	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));
	RayTracer rayTracer = RayTracer();
	Canvas canvas = rayTracer.TraceRays(this->scene);

	EXPECT_EQ(canvas.PixelAt(5, 5), Color(0.38066, 0.47583, 0.2855));
}

TEST_F(RaytracerTests, The_color_when_the_ray_hits_the_sphere_in_the_default_world) {
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));
	Color color = this->scene.TraceSingleRay(ray);

	EXPECT_EQ(color, Color(0.38066, 0.47583, 0.2855));
}

TEST_F(RaytracerTests, The_color_when_the_ray_misses_the_sphere_in_the_default_world) {
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 1, 0));
	Color color = this->scene.TraceSingleRay(ray);

	EXPECT_EQ(color, Color::Black());
}

TEST_F(RaytracerTests, NO_shadow_when_there_is_nothing_between_the_intersection_point_and_the_light) {
	IntersectionInfo intersection = IntersectionInfo(true,
		Tuple::Point(0, 10, 0), 10, Tuple::Vector(0, 1, 0));

	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(intersection, light->getPosition());

	EXPECT_EQ(isInShadow, false);
}

TEST_F(RaytracerTests, Shadow_when_there_is_an_object_between_the_intersection_point_and_the_light) {
	IntersectionInfo intersection = IntersectionInfo(true,
		Tuple::Point(10, -10, 10), 10, Tuple::Vector(0, 1, 0));

	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(intersection, light->getPosition());

	EXPECT_EQ(isInShadow, true);
}

TEST_F(RaytracerTests, NO_shadow_when_an_object_is_behind_the_light) {
	IntersectionInfo intersection = IntersectionInfo(true,
		Tuple::Point(-20, 20, -20), 20, Tuple::Vector(0, 1, 0));

	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(intersection, light->getPosition());

	EXPECT_EQ(isInShadow, false);
}

TEST_F(RaytracerTests, NO_shadow_when_an_object_is_behind_the_intersection_point) {
	IntersectionInfo intersection = IntersectionInfo(true,
		Tuple::Point(-2, 2, -2), 2, Tuple::Vector(0, 1, 0));

	LightPtr light = this->scene.getLights()[0];
	bool isInShadow = this->scene.IsInShadow(intersection, light->getPosition());

	EXPECT_EQ(isInShadow, false);
}

TEST_F(RaytracerTests, Camera_is_between_two_spheres_looking_at_the_one_in_shadow)
{
	LightPtr light = std::make_shared<Light>(Light(Tuple::Point(0, 0, -10),
		Color::White(), 1, 1, 1, 1));

	ObjectPtr s1 = std::make_shared<Sphere>(Sphere());
	ObjectPtr s2 = std::make_shared<Sphere>(Sphere(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(Material()),
		Transformations::Translation(0, 0, 10)));

	Ray ray = Ray(Tuple::Point(0, 0, 5), Tuple::Vector(0, 0, 1));

	Tuple from = Tuple::Point(0, 0, 5);
	Tuple up = Tuple::Point(0, 1, 0);
	Tuple to = Tuple::Point(0, 0, 1);

	Camera camera = Camera(from, to, up, 90, 11, 11);

	std::vector<LightPtr> lights{ light };
	std::vector<ObjectPtr> objects{ s1, s2 };
	Scene scene = Scene(11, 11, camera, objects, lights);

	Color color = scene.TraceSingleRay(ray);

	EXPECT_EQ(color, Color(0.1, 0.1, 0.1));
}