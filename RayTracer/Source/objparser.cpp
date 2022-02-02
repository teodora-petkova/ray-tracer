#include "objparser.h"

template<class T>
static bool readValues(std::stringstream& ss, std::vector<T>& values,
	std::string data, int lineNumber)
{
	int i = 0;
	T value{};
	while (ss >> value)
	{
		values.push_back(value);
		if (ss.fail()) {
			std::cout << "Failed reading value for data '" << data <<
				"' on line " << lineNumber << " " << i << ": " << ss.str() << " will skip\n";
			return false;
		}
		i++;
	}
	return true;
}

std::vector<std::string> tokenize(std::string const& str, const char delim)
{
	std::vector<std::string> tokens;

	size_t start = 0;
	size_t end = 0;

	while ((end = str.find_first_of(delim, start)) != std::string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
	}

	if (str.size() > start)
		tokens.push_back(str.substr(start, str.size()));

	return tokens;
}

int geti(std::string token)
{
	return atoi(token.c_str()) - 1;
}

void ObjParser::parse(std::istream& is)
{
	int lineNumber = 0;

	std::string line = "";
	GroupPtr currentGroup = nullptr;
	std::vector<GroupPtr> groups;

	while (std::getline(is, line))
	{
		std::stringstream ss(line);

		std::string command = "";
		ss >> command;
		if (command == "v")
		{
			std::vector<float> values;
			readValues(ss, values, command, lineNumber);

			this->vertices.push_back(Tuple::Point(values[0], values[1], values[2]));
		}
		if (command == "vn")
		{
			std::vector<float> values;
			readValues(ss, values, command, lineNumber);

			this->normals.push_back(Tuple::Point(values[0], values[1], values[2]));
		}
		else if (command == "f")
		{
			std::vector<std::string> values;
			readValues(ss, values, command, lineNumber);

			std::vector<std::string> tokens = tokenize(values[0], '/');
			if (tokens.size() == 3)
			{
				// first index and normal
				int ind1 = geti(tokens[0]);
				int n1 = geti(tokens[2]);

				for (int i = 1; i < values.size() - 1; i++)
				{
					auto tokeni = tokenize(values[i], '/');
					int indi = geti(tokeni[0]);
					int ni = geti(tokeni[2]);

					auto tokeni1 = tokenize(values[i + 1], '/');
					int indi1 = geti(tokeni1[0]);
					int ni1 = geti(tokeni1[2]);

					ObjectPtr triangle = std::make_shared<SmoothTriangle>(
						this->vertices[ind1],
						this->vertices[indi],
						this->vertices[indi1],
						this->normals[n1],
						this->normals[ni],
						this->normals[ni1]);

					if (currentGroup == nullptr)
					{
						currentGroup = std::make_shared<Group>(
							this->baseGroup->getMaterial(),
							Matrix<4, 4>::IdentityMatrix(), "InnerGroup");
						groups.push_back(currentGroup);
					}

					currentGroup->AddChild(triangle);
				}
			}
			else if (tokens.size() == 1)
			{
				// first index
				int ind1 = geti(tokens[0]);

				for (int i = 1; i < values.size() - 1; i++)
				{
					int indi = geti(values[i]);
					int indi1 = geti(values[i + 1]);

					ObjectPtr triangle = std::make_shared<Triangle>(
						this->vertices[ind1],
						this->vertices[indi],
						this->vertices[indi1],
						std::make_shared<Material>(),
						Matrix<4, 4>::IdentityMatrix());

					if (currentGroup == nullptr)
					{
						currentGroup = std::make_shared<Group>(
							this->baseGroup->getMaterial(),
							Matrix<4, 4>::IdentityMatrix(), "InnerGroup");
						groups.push_back(currentGroup);
					}

					currentGroup->AddChild(triangle);
				}
			}
		}
		else if (command == "g")
		{
			std::string groupName;
			ss >> groupName;

			// the inner group is taking only the base material
			// in order to pass it to the children
			// for correct color calculations
			currentGroup = std::make_shared<Group>(
				this->baseGroup->getMaterial(),
				Matrix<4, 4>::IdentityMatrix(), groupName);

			groups.push_back(currentGroup);
		}
		lineNumber++;
	}

	for (GroupPtr gr : groups)
	{
		this->baseGroup->AddChild(gr);
	}
}
