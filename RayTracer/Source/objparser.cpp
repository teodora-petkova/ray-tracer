#include "objparser.h"

static void logFailure(std::string lineData, std::string identifier,
	int lineNumber, int valueNumber)
{
	std::cerr << "Failed reading value for data '" << identifier
		<< "' on line " << lineNumber << " (value index " << valueNumber
		<< "), line '" << lineData << "' will be skipped\n";
}

template<class T>
static bool parseValues(std::stringstream& ss,
	std::vector<T>& values, int expectedValuesCount,
	std::string identifier, int lineNumber)
{
	int i = 0;
	T value{};
	// default -  implicitly stops at the first whitespace
	while (ss >> value)
	{
		if (ss.fail())
		{
			logFailure(ss.str(), identifier, lineNumber, i);
			return false;
		}
		values.emplace_back(value);
		i++;
	}
	return values.size() == expectedValuesCount;
}

std::vector<std::string> tokenize(std::string const& str, const char delim)
{
	std::vector<std::string> tokens;

	size_t start = 0;
	size_t end = 0;

	while ((end = str.find_first_of(delim, start)) != std::string::npos)
	{
		tokens.emplace_back(str.substr(start, end - start));
		start = end + 1;
	}

	if (str.size() > start)
		tokens.emplace_back(str.substr(start, str.size()));

	return tokens;
}

int geti(std::string token)
{
	return atoi(token.c_str()) - 1;
}

void ObjParser::parseVertex(std::stringstream& ss,
	std::string identifier, int lineNumber)
{
	std::vector<float> values;
	if (parseValues(ss, values, 3, identifier, lineNumber))
	{
		this->vertices.emplace_back(Tuple::Point(values[0], values[1], values[2]));
	}
	else
	{
		logFailure(ss.str(), identifier, lineNumber, 0);
	}
}

void ObjParser::parseNormal(std::stringstream& ss,
	std::string identifier, int lineNumber)
{
	std::vector<float> values;
	if (parseValues(ss, values, 3, identifier, lineNumber))
	{
		this->normals.emplace_back(Tuple::Point(values[0], values[1], values[2]));
	}
	else
	{
		logFailure(ss.str(), identifier, lineNumber, 0);
	}
}

void ObjParser::parseFace(std::stringstream& ss,
	std::string identifier, int lineNumber,
	std::vector<std::vector<int>>& currentVerticesIndices,
	std::vector<std::vector<int>>& currentNormalsIndices)
{
	std::vector<std::string> values;
	parseValues(ss, values, 3, identifier, lineNumber);

	std::vector<int> vertexIndices;
	std::vector<int> normalIndices;
	for (std::string value : values)
	{
		std::vector<std::string> tokens = tokenize(value, '/');

		if (tokens.size() == 3)
		{
			vertexIndices.emplace_back(geti(tokens[0]));
			normalIndices.emplace_back(geti(tokens[2]));
		}
		else if (tokens.size() == 1)
		{
			vertexIndices.emplace_back(geti(tokens[0]));
		}
		else
		{
			//TODO: log error
		}
	}
	if (vertexIndices.size() > 0)
		currentVerticesIndices.emplace_back(vertexIndices);
	if (normalIndices.size() > 0)
		currentNormalsIndices.emplace_back(normalIndices);
}

void ObjParser::parseGroup(std::stringstream& ss,
	std::string identifier, int lineNumber)
{
	std::string groupName;
	ss >> groupName;
	if (ss.fail())
	{
		logFailure(ss.str(), identifier, lineNumber, 0);
	}
	else
	{
		this->groupNames.emplace_back(groupName);
	}
}

bool ObjParser::updateIndicesArrays(int groupCount,
	std::vector<std::vector<int>>& currentVerticesIndices,
	std::vector<std::vector<int>>& currentNormalsIndices)
{
	bool isUpdated = false;

	if (currentVerticesIndices.size() > 0)
	{
		this->verticesIndices.insert({ groupCount, currentVerticesIndices });
		currentVerticesIndices.clear();
		isUpdated = true;
	}
	if (currentNormalsIndices.size() > 0)
	{
		this->normalsIndices.insert({ groupCount, currentNormalsIndices });
		currentNormalsIndices.clear();
		isUpdated = true;
	}

	return isUpdated;
}

int ObjParser::parseGroups(std::istream& is)
{
	int lineNumber = 0;

	int groupCount = 0;
	std::vector<std::vector<int>> currentVerticesIndices;
	std::vector<std::vector<int>> currentNormalsIndices;

	// istream& getline(istream &is, string &str, char delim)
	//  the text from the input stream will be read up through the first occurrence
	// of 'delim' (defaults to '\n') and placed into str
	std::string line = "";
	while (std::getline(is, line))
	{
		std::stringstream ss(line);

		std::string identifier = "";
		ss >> identifier;

		if (identifier == "v")
		{
			parseVertex(ss, identifier, lineNumber);
		}
		if (identifier == "vn")
		{
			parseNormal(ss, identifier, lineNumber);
		}
		else if (identifier == "f")
		{
			parseFace(ss, identifier, lineNumber,
				currentVerticesIndices, currentNormalsIndices);
		}
		else if (identifier == "g")
		{
			parseGroup(ss, identifier, lineNumber);

			if (updateIndicesArrays(groupCount,
				currentVerticesIndices, currentNormalsIndices))
			{
				groupCount++;
			}
		}
		lineNumber++;
	}

	if (updateIndicesArrays(groupCount,
		currentVerticesIndices, currentNormalsIndices))
	{
		groupCount++;
	}
	return groupCount;
}

GroupPtr ObjParser::createGroup(int groupIndex, const std::string& groupName)
{
	GroupPtr group = std::make_shared<Group>(
		this->baseGroup->getMaterial(),
		Matrix<4, 4>::IdentityMatrix(), groupName);

	bool hasNormals = this->verticesIndices.size() == this->normalsIndices.size();

	// for each face
	for (int j = 0; j < this->verticesIndices[groupIndex].size(); j++)
	{
		int v1 = this->verticesIndices[groupIndex][j][0];
		int n1 = hasNormals ? this->normalsIndices[groupIndex][j][0] : 0;

		// for each index of a vertex in a defined face
		for (int k = 1; k < this->verticesIndices[groupIndex][j].size() - 1; k++)
		{
			ObjectPtr triangle = nullptr;

			if (hasNormals)
			{
				triangle = std::make_shared<SmoothTriangle>(
					this->vertices[v1],
					this->vertices[this->verticesIndices[groupIndex][j][k]],
					this->vertices[this->verticesIndices[groupIndex][j][k + 1]],
					this->normals[n1],
					this->normals[this->normalsIndices[groupIndex][j][k]],
					this->normals[this->normalsIndices[groupIndex][j][k + 1]]);
			}
			else
			{
				triangle = std::make_shared<Triangle>(
					this->vertices[v1],
					this->vertices[this->verticesIndices[groupIndex][j][k]],
					this->vertices[this->verticesIndices[groupIndex][j][k + 1]],
					std::make_shared<Material>(),
					Matrix<4, 4>::IdentityMatrix());
			}

			group->AddChild(triangle);
		}
	}

	return group;
}

void ObjParser::normaliseVertices()
{
	BoundingBox bbox = BoundingBox();
	for (Tuple& v : this->vertices)
	{
		bbox.AddPoint(v);
	}
	float sx = bbox.getMax().X() - bbox.getMin().X();
	float sy = bbox.getMax().Y() - bbox.getMin().Y();
	float sz = bbox.getMax().Z() - bbox.getMin().Z();

	float scale = std::max({ sx, sy, sz }) * 0.5;

	for (int i = 0; i < this->getVerticesCount(); i++)
	{
		this->vertices[i] = Tuple::Point(
			((this->vertices[i].X() - (bbox.getMin().X() + sx * 0.5)) / scale),
			((this->vertices[i].Y() - (bbox.getMin().Y() + sy * 0.5)) / scale),
			((this->vertices[i].Z() - (bbox.getMin().Z() + sy * 0.5)) / scale));
	}
}

void ObjParser::parse(std::istream& is)
{
	int groupCount = parseGroups(is);

	// for each group
	for (int i = 0; i < groupCount; i++)
	{
		std::string groupName = this->groupNames.size() == groupCount ?
			this->groupNames[i] : std::to_string(i);

		normaliseVertices();

		GroupPtr group = createGroup(i, groupName);

		this->baseGroup->AddChild(group);

		std::cout << "Group " << group->getName() << ": " << group->getChildrenCount()
			<< " triangles" << std::endl;
	}
}