#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include <math.h>
#include <vector>

#include "source\raytracer.h"
#include "source\objparser.h"

class TestPattern :public Pattern
{
public:
	TestPattern()
		: Pattern(Matrix<4, 4>::IdentityMatrix())
	{}

	Color getColorAt(const Tuple& point) const
	{
		return Color(point);
	}
};

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
	Color color = customScene.ReflectRay(ray, i);

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
	Color color = customScene.ReflectRay(ray, i, 0);

	EXPECT_EQ(color, Color(0.f, 0.f, 0.f));
}

static Color getRefractedColorAtRayObjIntersection(const Scene& scene,
	const Ray& ray, ObjectPtr obj, int remaining = 5)
{
	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();

	IntersectionInfo intersection = obj->Intersect(ray, intersections);

	auto n1n2 = scene.getRefractiveIndices(intersection, intersections);

	Color color = scene.RefractRay(ray, intersection, n1n2.first, n1n2.second, remaining);

	return color;
}
TEST_F(RaytracerTests, The_refracted_color_with_an_opaque_surface)
{
	Ray ray = Ray(Tuple::Point(0, 0, -5),
		Tuple::Vector(0, 0, 1));

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>{
		std::make_pair(4, this->sphere1),
		std::make_pair(6, this->sphere1) };

	IntersectionInfo i = this->sphere1->Intersect(ray, intersections);

	Color color = getRefractedColorAtRayObjIntersection(this->scene, ray, this->sphere1);

	EXPECT_EQ(color, Color::Black());
}

TEST_F(RaytracerTests, The_refracted_color_at_the_maximum_recursive_depth)
{
	auto m = this->sphere1->getMaterial();
	ObjectPtr customSphere1 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(m->getPattern(), m->getAmbient(), m->getDiffuse(),
			m->getSpecular(), m->getShininess(), m->getReflective(), 1, 1.5),
		Matrix<4, 4>::IdentityMatrix());

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ customSphere1, this->sphere2 };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	Ray ray = Ray(Tuple::Point(0, 0, -5),
		Tuple::Vector(0, 0, 1));

	//auto intersections = std::vector<std::pair<float, ObjectConstPtr>>{
	//	std::make_pair(4, customSphere1),
	//	std::make_pair(6, customSphere1) };

	Color color = getRefractedColorAtRayObjIntersection(this->scene, ray, customSphere1, 0);

	EXPECT_EQ(color, Color::Black());
}

TEST_F(RaytracerTests, The_refracted_color_with_total_internal_refraction)
{
	auto m = this->sphere1->getMaterial();
	ObjectPtr customSphere1 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(m->getPattern(), m->getAmbient(), m->getDiffuse(),
			m->getSpecular(), m->getShininess(), m->getReflective(), 1, 1.5),
		Matrix<4, 4>::IdentityMatrix());

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ customSphere1, this->sphere2 };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	float sqrt2over2 = sqrt(2) / 2;
	Ray ray = Ray(Tuple::Point(0, 0, sqrt2over2),
		Tuple::Vector(0, 1, 0));

	//auto intersections = std::vector<std::pair<float, ObjectConstPtr>>{
	//	std::make_pair(-sqrt2over2, customSphere1),
	//	std::make_pair(sqrt2over2, customSphere1) };

	Color color = getRefractedColorAtRayObjIntersection(this->scene, ray, customSphere1);

	EXPECT_EQ(color, Color::Black());
}

TEST_F(RaytracerTests, The_refracted_color_with_a_refracted_ray)
{
	auto m1 = this->sphere1->getMaterial();
	ObjectPtr customSphere1 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(std::make_shared<TestPattern>(), 1.f, m1->getDiffuse(),
			m1->getSpecular(), m1->getShininess(), m1->getReflective()),
		Matrix<4, 4>::IdentityMatrix());

	auto m2 = this->sphere2->getMaterial();
	ObjectPtr customSphere2 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(m2->getPattern(), m2->getAmbient(), m2->getDiffuse(),
			m2->getSpecular(), m2->getShininess(), m2->getReflective(), 1, 1.5),
		Transformations::Scaling(0.5, 0.5, 0.5));

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ customSphere1, customSphere2 };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	Ray ray = Ray(Tuple::Point(0, 0, 0.1),
		Tuple::Vector(0, 1, 0));

	//auto intersections = std::vector<std::pair<float, ObjectConstPtr>>{
	//	std::make_pair(-0.9949f, customSphere1),
	//	std::make_pair(-0.4899f, customSphere2),
	//	std::make_pair(0.4899f, customSphere2),
	//	std::make_pair(0.9949f, customSphere1) };

	Color color = getRefractedColorAtRayObjIntersection(customScene, ray, customSphere2);


	EXPECT_EQ(color, Color(0, 0.9988, 0.04725));
}

TEST_F(RaytracerTests, The_accumulated_refracted_color_with_a_refracted_ray)
{
	ObjectPtr floor = std::make_shared<Plane>(
		std::make_shared<Material>(std::make_shared<FlatColor>(),
			0.1, 0.9, 0.9, 200, 0, 0.5, 1.5),
		Transformations::Translation(0, -1, 0));

	ObjectPtr ball = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(std::make_shared<FlatColor>(Color(1, 0, 0)),
			0.5, 0.9, 0.9, 200),
		Transformations::Translation(0, -3.5, -0.5));

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ this->sphere1, this->sphere2, floor, ball };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	float sqrt2over2 = sqrt(2) / 2;
	Ray ray = Ray(Tuple::Point(0, 0, -3),
		Tuple::Vector(0, -sqrt2over2, sqrt2over2));

	Color color = customScene.TraceSingleRay(ray, 5);

	EXPECT_EQ(color, Color(0.93642f, 0.68642f, 0.68642f));
}

static float schlick(const Scene& scene, const Ray& ray, ObjectConstPtr obj)
{
	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	IntersectionInfo intersection = obj->Intersect(ray, intersections);
	auto n1n2 = scene.getRefractiveIndices(intersection, intersections);
	float reflectance = scene.Schlick(ray, intersection, n1n2.first, n1n2.second);

	return reflectance;
}
TEST_F(RaytracerTests, The_Schlick_approximation_under_total_internal_reflection)
{
	ObjectPtr glassSphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(std::make_shared<FlatColor>(Color(1, 0, 0)),
			0.1, 0.9, 0.9, 200, 0, 1, 1.5),
		Matrix<4, 4>::IdentityMatrix());

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ glassSphere };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	float sqrt2over2 = sqrt(2) / 2;
	Ray ray = Ray(Tuple::Point(0, 0, sqrt2over2),
		Tuple::Vector(0, 1, 0));

	// auto intersections = std::vector<std::pair<float, ObjectConstPtr>> {
	// std::make_pair(-sqrt2over2, glassSphere),
	// std::make_pair(sqrt2over2, glassSphere) };

	float reflectance = schlick(customScene, ray, glassSphere);

	EXPECT_EQ(reflectance, 1.0);
}

TEST_F(RaytracerTests, The_Schlick_approximation_with_a_perpendicular_viewing_angle)
{
	ObjectPtr glassSphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(std::make_shared<FlatColor>(Color(1, 0, 0)),
			0.1, 0.9, 0.9, 200, 0, 1, 1.5),
		Matrix<4, 4>::IdentityMatrix());

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ glassSphere };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	Ray ray = Ray(Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 1, 0));

	// auto intersections = std::vector<std::pair<float, ObjectConstPtr>> {
	// std::make_pair(-1, glassSphere),
	// std::make_pair(1, glassSphere) };

	float reflectance = schlick(customScene, ray, glassSphere);

	EXPECT_TRUE(equals(reflectance, 0.04));
}

TEST_F(RaytracerTests, The_Schlick_approximation_with_a_small_angle_and_n2_greated_than_n1)
{
	ObjectPtr glassSphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(std::make_shared<FlatColor>(Color(1, 0, 0)),
			0.1, 0.9, 0.9, 200, 0, 1, 1.5),
		Matrix<4, 4>::IdentityMatrix());

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ glassSphere };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	Ray ray = Ray(Tuple::Point(0, 0.99, -2),
		Tuple::Vector(0, 0, 1));

	// auto intersections = std::vector<std::pair<float, ObjectConstPtr>> {
	// std::make_pair(1.8589, glassSphere) };

	float reflectance = schlick(customScene, ray, glassSphere);

	EXPECT_TRUE(equals(reflectance, 0.48873));
}

TEST_F(RaytracerTests, The_accumulated_reflected_and_transparent_color)
{
	ObjectPtr floor = std::make_shared<Plane>(
		std::make_shared<Material>(std::make_shared<FlatColor>(),
			0.1, 0.9, 0.9, 200, 0.5, 0.5, 1.5),
		Transformations::Translation(0, -1, 0));

	ObjectPtr ball = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(std::make_shared<FlatColor>(Color(1, 0, 0)),
			0.5, 0.9, 0.9, 200),
		Transformations::Translation(0, -3.5, -0.5));

	std::vector<LightPtr> lights{ this->light };
	std::vector<ObjectPtr> objects{ this->sphere1, this->sphere2, floor, ball };
	Scene customScene = Scene(11, 11, this->camera, objects, lights);

	float sqrt2over2 = sqrt(2) / 2;
	Ray ray = Ray(Tuple::Point(0, 0, -3),
		Tuple::Vector(0, -sqrt2over2, sqrt2over2));

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();// {
	// std::make_pair(sqrt(2), floor) };

	IntersectionInfo i = floor->Intersect(ray, intersections);
	Color color = customScene.TraceSingleRay(ray, 5);

	EXPECT_EQ(color, Color(0.93391f, 0.69643f, 0.69243f));
}


struct BVHData
{
	int threshold;
	bool withNormals;
	const char* fileContent;

	friend std::ostream& operator<<(std::ostream& os, const BVHData& bvh)
	{
		return os << "BVH threshold: " << bvh.threshold <<
			(bvh.withNormals ?
				" with defined normals" :
				" without defined normals");
	}
};

class GroupsDividedByBVHTests : public testing::TestWithParam<BVHData> {};
/*
static void printGroup(const GroupPtr& group)
{
	for (int i = 0; i < group->getChildrenCount(); i++)
	{
		auto& child = group->getChild(i);
		std::cout << child->getName() << std::endl;
		auto innerGroup = std::dynamic_pointer_cast<Group>(child);
		if (innerGroup != nullptr)
		{
			printGroup(innerGroup);
		}
	}
}*/

TEST_P(GroupsDividedByBVHTests, Calculated_color_does_not_depends_on_dividing_by_BVH)
{
	auto& bvh = GetParam();
	std::istringstream fileContent(bvh.fileContent);

	MaterialPtr material = std::make_shared<Material>(
		std::make_shared<FlatColor>(Color(0.6, 1, 0.3)),
		0.1, 0.9, 0.9, 200, 0, 0, 0);
	Matrix<4, 4> rotatey = Transformations::RotationY(45);

	ObjParser parser = ObjParser(fileContent, material, rotatey);

	auto group = parser.getBaseGroup();
	group->Divide(bvh.threshold);

	LightPtr light = std::make_shared<Light>(Tuple::Point(0, 10, -10),
		Color::White(), 1, 1, 1, 1);

	Camera camera = Camera(Tuple::Point(0, 0, -5),
		Tuple::Point(0, 0, 1), Tuple::Point(0, 1, 0), 90, 11, 11);

	std::vector<LightPtr> lights{ light };
	std::vector<ObjectPtr> objects{ group };
	Scene customScene = Scene(200, 200, camera, objects, lights);

	Ray ray1 = Ray(Tuple::Point(0.001, 0, -5), Tuple::Vector(0, 0, 1));
	Ray ray2 = Ray(Tuple::Point(-0.001, 0, -5), Tuple::Vector(0, 0, 1));

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();

	Color color1 = customScene.TraceSingleRay(ray1);
	Color color2 = customScene.TraceSingleRay(ray2);

	EXPECT_EQ(color1, Color(0.3087, 0.5145, 0.1544)); // 0.001
	EXPECT_EQ(color1, color2);
}

const char* cube = R"(
v -1 -1  1
v -1 -1 -1
v -1  1 -1
v -1  1  1
v  1 -1  1
v  1 -1 -1
v  1  1 -1
v  1  1  1

f 4 3 2 1
f 2 6 5 1
f 3 7 6 2
f 8 7 3 4
f 5 8 4 1
f 6 7 8 5)";

const char* cubeWithNormals = R"(
g cube
 
v  -1 -1 -1
v  -1 -1  1
v  -1  1 -1
v  -1  1  1
v   1 -1 -1
v   1 -1  1
v   1  1 -1
v   1  1  1

vn  0  0  1
vn  0  0 -1
vn  0  1  0
vn  0 -1  0
vn  1  0  0
vn -1  0  0
 
f  1//2  7//2  5//2
f  1//2  3//2  7//2 
f  1//6  4//6  3//6 
f  1//6  2//6  4//6 
f  3//3  8//3  7//3 
f  3//3  4//3  8//3 
f  5//5  7//5  8//5 
f  5//5  8//5  6//5 
f  1//4  5//4  6//4 
f  1//4  6//4  2//4 
f  2//1  6//1  8//1 
f  2//1  8//1  4//1 )";

INSTANTIATE_TEST_CASE_P(RaytracerTests, GroupsDividedByBVHTests,
	testing::Values(
		BVHData{ 0, false, cube },
		BVHData{ 1, false, cube },
		BVHData{ 2, false, cube },
		BVHData{ 3, false, cube },
		BVHData{ 4, false, cube },
		BVHData{ 5, false, cube },
		BVHData{ 6, false, cube },

		BVHData{ 0, true, cubeWithNormals },
		BVHData{ 1, true, cubeWithNormals },
		BVHData{ 2, true, cubeWithNormals },
		BVHData{ 3, true, cubeWithNormals },
		BVHData{ 4, true, cubeWithNormals },
		BVHData{ 5, true, cubeWithNormals },
		BVHData{ 6, true, cubeWithNormals }));