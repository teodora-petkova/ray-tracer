#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\objparser.h"

TEST(ObjParserTests, Ignoring_unrecognized_lines) {
	std::istringstream fileContent(R"~(
		There was a young lady named Bright
		who traveled much faster than light.
		She set out one day in a relative way,
		and came back the previous night.)~");

	ObjParser parser = ObjParser(fileContent);

	EXPECT_EQ(1, 1);
}

TEST(ObjParserTests, Parse_vertices) {
	std::istringstream fileContent(R"~(
		v -1 1 0
		v -1.0000 0.5000 0.0000
		v 1 0 0
		v 1 1 0)~");

	ObjParser parser = ObjParser(fileContent);

	EXPECT_EQ(parser.getVertex(0), Tuple::Point(-1, 1, 0));
	EXPECT_EQ(parser.getVertex(1), Tuple::Point(-1, 0.5, 0));
	EXPECT_EQ(parser.getVertex(2), Tuple::Point(1, 0, 0));
	EXPECT_EQ(parser.getVertex(3), Tuple::Point(1, 1, 0));
}

TEST(ObjParserTests, Parse_triangle_faces) {
	std::istringstream fileContent(R"~(
		v -1 1 0
		v -1 0 0
		v 1 0 0
		v 1 1 0
		f 1 2 3
		f 1 3 4)~");

	ObjParser parser = ObjParser(fileContent);

	auto gr = parser.getBaseGroup();
	auto triangle1 = std::dynamic_pointer_cast<Triangle>(gr->getChild(0));
	auto triangle2 = std::dynamic_pointer_cast<Triangle>(gr->getChild(1));

	EXPECT_EQ(triangle1->getA(), parser.getVertex(0));
	EXPECT_EQ(triangle1->getB(), parser.getVertex(1));
	EXPECT_EQ(triangle1->getC(), parser.getVertex(2));

	EXPECT_EQ(triangle2->getA(), parser.getVertex(0));
	EXPECT_EQ(triangle2->getB(), parser.getVertex(2));
	EXPECT_EQ(triangle2->getC(), parser.getVertex(3));
}

TEST(ObjParserTests, Parse_triangulated_polygons) {
	std::istringstream fileContent(R"~(
		v -1 1 0
		v -1 0 0
		v 1 0 0
		v 1 1 0
		v 0 2 0
		f 1 2 3 4 5)~");

	ObjParser parser = ObjParser(fileContent);

	auto gr = parser.getBaseGroup();
	auto triangle1 = std::dynamic_pointer_cast<Triangle>(gr->getChild(0));
	auto triangle2 = std::dynamic_pointer_cast<Triangle>(gr->getChild(1));
	auto triangle3 = std::dynamic_pointer_cast<Triangle>(gr->getChild(2));

	EXPECT_EQ(triangle1->getA(), parser.getVertex(0));
	EXPECT_EQ(triangle1->getB(), parser.getVertex(1));
	EXPECT_EQ(triangle1->getC(), parser.getVertex(2));

	EXPECT_EQ(triangle2->getA(), parser.getVertex(0));
	EXPECT_EQ(triangle2->getB(), parser.getVertex(2));
	EXPECT_EQ(triangle2->getC(), parser.getVertex(3));

	EXPECT_EQ(triangle3->getA(), parser.getVertex(0));
	EXPECT_EQ(triangle3->getB(), parser.getVertex(3));
	EXPECT_EQ(triangle3->getC(), parser.getVertex(4));
}

TEST(ObjParserTests, Parse_triangles_in_groups) {
	std::istringstream fileContent(R"~(
		v -1 1 0
		v -1 0 0
		v 1 0 0
		v 1 1 0
		g FirstGroup
		f 1 2 3
		g SecondGroup
		f 1 3 4)~");

	ObjParser parser = ObjParser(fileContent);

	auto gr = parser.getBaseGroup();
	auto gr1 = std::dynamic_pointer_cast<Group>(gr->getChild(0));
	auto gr2 = std::dynamic_pointer_cast<Group>(gr->getChild(1));

	auto triangle1 = std::dynamic_pointer_cast<Triangle>(gr1->getChild(0));
	auto triangle2 = std::dynamic_pointer_cast<Triangle>(gr2->getChild(0));

	EXPECT_EQ(triangle1->getA(), parser.getVertex(0));
	EXPECT_EQ(triangle1->getB(), parser.getVertex(1));
	EXPECT_EQ(triangle1->getC(), parser.getVertex(2));

	EXPECT_EQ(triangle2->getA(), parser.getVertex(0));
	EXPECT_EQ(triangle2->getB(), parser.getVertex(2));
	EXPECT_EQ(triangle2->getC(), parser.getVertex(3));
}

TEST(ObjParserTests, Parse_normals) {
	std::istringstream fileContent(R"~(
		vn 0 0 1
		vn 0.707 0 -0.707
		vn 1 2 3)~");

	ObjParser parser = ObjParser(fileContent);

	EXPECT_EQ(parser.getNormal(0), Tuple::Point(0, 0, 1));
	EXPECT_EQ(parser.getNormal(1), Tuple::Point(0.707, 0, -0.707));
	EXPECT_EQ(parser.getNormal(2), Tuple::Point(1, 2, 3));
}