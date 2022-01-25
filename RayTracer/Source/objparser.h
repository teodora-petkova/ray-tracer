#pragma once

#pragma warning(push, 0)
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "tuple.h"
#include "group.h"
#include "triangle.h"
#include "smoothtriangle.h"

class RAYTRACER_EXPORT ObjParser
{
public:
	ObjParser(std::istream& is)
	{
		this->baseGroup = std::make_shared<Group>();

		parse(is);
	}

	Tuple getVertex(int index) const { return vertices[index]; }
	Tuple getNormal(int index) const { return normals[index]; }

	GroupPtr getBaseGroup() const { return baseGroup; }

private:
	std::vector<Tuple> vertices;
	std::vector<Tuple> normals;

	GroupPtr baseGroup;

	void parse(std::istream& is);
};