#include "readfile.h"

namespace ReadScene
{
	bool readValues(std::stringstream& s, const int numvals, float* values)
	{
		for (int i = 0; i < numvals; i++) {
			s >> values[i];
			if (s.fail()) {
				std::cout << "Failed reading value " << i << " will skip\n";
				return false;
			}
		}
		return true;
	}

	template<class PatternType>
	PatternPtr processPattern(std::stack<PatternPtr>& patterns, const Matrix<4, 4>& transformation)
	{
		PatternPtr pattern = nullptr;
		if (patterns.size() >= 2) // a valid definition with 2 preceding patterns
		{
			PatternPtr pattern2 = patterns.top();
			patterns.pop();
			PatternPtr pattern1 = patterns.top();
			patterns.pop();
			pattern = std::make_shared<PatternType>(pattern1, pattern2,
				transformation);
		}
		return pattern;
	}

	Scene ReadSceneFile(const std::string& filename)
	{
		// image size
		int imageWidth = 0;
		int imageHeight = 0;
		// camera
		Camera camera;
		// objects
		std::vector<Tuple> vertices = std::vector<Tuple>();
		std::vector<ObjectPtr> objects = std::vector<ObjectPtr>();
		// lights
		std::vector<LightPtr> lights = std::vector<LightPtr>();
		// patterns
		std::stack<PatternPtr> patterns = std::stack<PatternPtr>();

		std::string str, command;
		std::ifstream in;
		in.open(filename);
		if (in.is_open())
		{
			getline(in, str);

			MaterialPtr currentMaterial;
			Matrix<4, 4> currentTransformation = Matrix<4, 4>::IdentityMatrix();

			while (in)
			{
				// ruled out comment and blank lines 
				if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#'))
				{
					std::stringstream s(str);
					s >> command;

					// Position and color for light, colors for others - up to 10 parameters for cameras
					float values[10];

					bool isInputValid;
					if (command == "size")
					{
						// 10 values eye center up fov
						isInputValid = readValues(s, 2, values);
						if (isInputValid)
						{
							//size width height
							imageWidth = (int)values[0];
							imageHeight = (int)values[1];
						}
					}
					else if (command == "camera")
					{
						// 10 values eye center up fov
						isInputValid = readValues(s, 10, values);
						if (isInputValid)
						{
							//camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy
							Tuple lookFrom = Tuple::Vector(values[0], values[1], values[2]);
							Tuple lookAt = Tuple::Vector(values[3], values[4], values[5]);
							Tuple up = Tuple::Vector(values[6], values[7], values[8]);

							float fovY = values[9];
							camera = Camera(lookFrom, lookAt, up, fovY, imageWidth, imageHeight);
						}
					}
					else if (command == "point_light")
					{
						// 10 values light position x y z, light color r g b, brightness, ambient, diffuse, specular
						isInputValid = readValues(s, 10, values);
						if (isInputValid)
						{
							Tuple lightPosition = Tuple::Point(values[0], values[1], values[2]);
							Tuple lightColour = Color::Color(values[3], values[4], values[5]);

							lights.push_back(std::make_shared<Light>(lightPosition, lightColour,
								values[6], values[7], values[8], values[9]));
						}
					}
					else if (command == "vertex")
					{
						// 3 values x y z
						isInputValid = readValues(s, 3, values);
						if (isInputValid)
						{
							vertices.push_back(Tuple::Vector(values[0], values[1], values[2]));
						}
					}
					else if (command == "color")
					{
						// 3 values r g b
						isInputValid = readValues(s, 3, values);
						if (isInputValid)
						{
							PatternPtr pattern = std::make_shared<FlatColor>(
								Color(values[0], values[1], values[2]));
							patterns.push(pattern);
						}
					}
					else if (command == "stripepattern")
					{
						auto pattern = processPattern<StripePattern>(
							patterns, currentTransformation);
						patterns.push(pattern);
						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "gradient")
					{
						auto pattern = processPattern<Gradient>(
							patterns, currentTransformation);
						patterns.push(pattern);
						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "ringpattern")
					{
						auto pattern = processPattern<RingPattern>(
							patterns, currentTransformation);
						patterns.push(pattern);
						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "checkerpattern")
					{
						auto pattern = processPattern<CheckerPattern>(
							patterns, currentTransformation);
						patterns.push(pattern);
						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "blendingpattern")
					{
						auto pattern = processPattern<BlendingPattern>(
							patterns, currentTransformation);
						patterns.push(pattern);
						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "radialgradient")
					{
						auto pattern = processPattern<RadialGradient>(
							patterns, currentTransformation);
						patterns.push(pattern);
						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "material")
					{
						// ambient, diffuse, specular, shininess, reflectiveness, transparency, refractiveIndex
						isInputValid = readValues(s, 7, values);
						if (isInputValid)
						{
							auto rootPattern = patterns.top();
							patterns.pop();
							if (patterns.empty()) // a valid definition
							{
								MaterialPtr m = std::make_shared<Material>(
									rootPattern, values[0], values[1],
									values[2], values[3], values[4],
									values[5], values[6]);
								currentMaterial = m;
							}
						}
					}
					else if (command == "tri")
					{
						// 3 values vertex1 vertex2 vertex3
						isInputValid = readValues(s, 3, values);
						if (isInputValid)
						{
							ObjectPtr triangle = std::make_shared<Triangle>(
								vertices[(int)values[0]],
								vertices[(int)values[1]],
								vertices[(int)values[2]],
								currentMaterial,
								currentTransformation);
							objects.push_back(triangle);
							currentTransformation = Matrix<4, 4>::IdentityMatrix();
						}
					}
					else if (command == "sphere")
					{
						// 4 center: vertex1 vertex2 vertex3 + radius
						isInputValid = readValues(s, 4, values);
						if (isInputValid)
						{
							ObjectPtr sphere = std::make_shared<Sphere>(
								Tuple::Vector(values[0], values[1], values[2]),
								values[3],
								currentMaterial, currentTransformation);
							objects.push_back(sphere);
							currentTransformation = Matrix<4, 4>::IdentityMatrix();
						}
					}
					else if (command == "plane")
					{
						// no values
						ObjectPtr plane = std::make_shared<Plane>(currentMaterial, currentTransformation);
						objects.push_back(plane);
						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "cube")
					{
						// no values
						ObjectPtr cube = std::make_shared<Cube>(currentMaterial, currentTransformation);
						objects.push_back(cube);

						currentTransformation = Matrix<4, 4>::IdentityMatrix();
					}
					else if (command == "group")
					{
						// TODO: to think of a better way to define the file path
						// obj filename - the full path without commas?
						std::string objFilepath;
						s >> objFilepath;

						std::ifstream ifs;
						if (!objFilepath.empty())
							ifs.open(objFilepath);

						if (!ifs)
						{
							std::cerr << "Error opening file: " << objFilepath << '\n';
						}

						if (ifs.is_open())
						{
							std::istream& is = static_cast<std::istream&>(ifs);
							ObjParser parser = ObjParser(is,
								currentMaterial,
								currentTransformation);
							auto group = parser.getBaseGroup();

							objects.push_back(group);
							currentTransformation = Matrix<4, 4>::IdentityMatrix();
						}
					}

					// transformations
					else if (command == "rotatex")
					{
						// rotatex angle
						isInputValid = readValues(s, 1, values);
						if (isInputValid)
						{
							Matrix<4, 4> m = Transformations::RotationX(values[0]);
							currentTransformation = currentTransformation * m;
						}
					}
					else if (command == "rotatey")
					{
						// rotatey angle
						isInputValid = readValues(s, 1, values);
						if (isInputValid)
						{
							Matrix<4, 4> m = Transformations::RotationY(values[0]);
							currentTransformation = currentTransformation * m;
						}
					}
					else if (command == "rotatez")
					{
						// rotatez angle
						isInputValid = readValues(s, 1, values);
						if (isInputValid)
						{
							Matrix<4, 4> m = Transformations::RotationZ(values[0]);
							currentTransformation = currentTransformation * m;
						}
					}
					else if (command == "translate")
					{
						// translate x y z
						isInputValid = readValues(s, 3, values);
						if (isInputValid)
						{
							Matrix<4, 4> m = Transformations::Translation(values[0], values[1], values[2]);
							currentTransformation = currentTransformation * m;
						}
					}
					else if (command == "scale")
					{
						// scale x y z
						isInputValid = readValues(s, 3, values);
						if (isInputValid)
						{
							Matrix<4, 4> m = Transformations::Scaling(values[0], values[1], values[2]);
							currentTransformation = currentTransformation * m;
						}
					}
					else if (command == "shear")
					{
						// shear a b c d e f
						isInputValid = readValues(s, 6, values);
						if (isInputValid)
						{
							Matrix<4, 4> m = Transformations::Shearing(values[0], values[1], values[2],
								values[3], values[4], values[5]);
							currentTransformation = currentTransformation * m;
						}
					}

					for (int ind = 0; ind < 10; ind++) values[ind] = 0;
				}
				getline(in, str);
			}
		}

		return Scene(imageWidth, imageHeight,
			camera, objects, lights);
	}
}