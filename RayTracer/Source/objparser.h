#pragma once

#pragma warning(push, 0)
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <unordered_map>
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

	ObjParser(std::istream& is,
		MaterialPtr material,
		Matrix<4, 4> transformation)
	{
		this->baseGroup = std::make_shared<Group>(material, transformation,
			"BaseObjGroup");

		parse(is);
	}

	Tuple getVertex(int index) const { return vertices[index]; }
	int getVerticesCount() const { return vertices.size(); }

	Tuple getNormal(int index) const { return normals[index]; }
	int getNormalsCount() const { return normals.size(); }

	GroupPtr getBaseGroup() const { return baseGroup; }

private:
	std::vector<Tuple> vertices;
	std::vector<Tuple> normals;

	std::unordered_map<int, std::vector<std::vector<int>>>  verticesIndices;
	std::unordered_map<int, std::vector<std::vector<int>>>  normalsIndices;

	std::vector<std::string> groupNames;

	GroupPtr baseGroup;

	void parse(std::istream& is);

	int parseGroups(std::istream& is);
	void parseVertex(std::stringstream& ss, std::string identifier, int lineNumber);
	void parseNormal(std::stringstream& ss, std::string identifier, int lineNumber);
	void parseFace(std::stringstream& ss, std::string identifier, int lineNumber,
		std::vector<std::vector<int>>& currentVerticesIndices,
		std::vector<std::vector<int>>& currentNormalsIndices);
	void parseGroup(std::stringstream& ss, std::string identifier, int lineNumber);
	bool updateIndicesArrays(int groupCount,
		std::vector<std::vector<int>>& currentVerticesIndices,
		std::vector<std::vector<int>>& currentNormalsIndices);
	GroupPtr createGroup(int i, const std::string& groupName);
	void normaliseVertices();
};