#include "objparser.h"

static bool readValues(std::stringstream& ss, std::vector<float>& values)
{
	for (int i = 0; !ss.eof(); i++) {
		float value;
		ss >> value;
		values.push_back(value);
		if (ss.fail()) {
			std::cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}

void ObjParser::parse(std::istream& is)
{
	std::string line = "";
	GroupPtr currentGroup = this->baseGroup;

	while (std::getline(is, line))
	{
		std::stringstream ss(line);

		std::string command = "";
		ss >> command;
		if (command == "v")
		{
			std::vector<float> values;
			readValues(ss, values);

			this->vertices.push_back(Tuple::Point(values[0], values[1], values[2]));
		}
		if (command == "vn")
		{
			std::vector<float> values;
			readValues(ss, values);

			this->normals.push_back(Tuple::Point(values[0], values[1], values[2]));
		}
		else if (command == "f")
		{
			std::vector<float> values;
			readValues(ss, values);

			int firstIndex = values[0];

			for (int i = 1; i < values.size() - 1; i++)
			{
				ObjectPtr triangle = std::make_shared<Triangle>(
					this->vertices[firstIndex - 1],
					this->vertices[(int)values[i] - 1],
					this->vertices[(int)values[i + 1] - 1],
					std::make_shared<Material>(),
					Matrix<4, 4>::IdentityMatrix());

				currentGroup->AddChild(triangle);
			}
		}
		else if (command == "g")
		{
			std::string groupName;
			ss >> groupName;
			GroupPtr newGroup = std::make_shared<Group>(groupName);
			this->baseGroup->AddChild(newGroup);
			currentGroup = newGroup;
		}
	}
}
