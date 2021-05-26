#include "readfile.h"
#include "scene.h"

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

		std::string str, cmd;
		std::ifstream in;
		in.open(filename);
		if (in.is_open())
		{
			getline(in, str);

			MaterialPtr currentMaterial;

			while (in)
			{
				// ruled out comment and blank lines 
				if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#'))
				{
					std::stringstream s(str);
					s >> cmd;

					// Position and color for light, colors for others - up to 10 parameters for cameras
					float values[10];

					bool isInputValid;
					if (cmd == "size")
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

					if (cmd == "camera")
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

					if (cmd == "point_light")
					{
						// 10 values light position x y z, light color r g b, brightness, ambient, diffuse, specular
						isInputValid = readValues(s, 10, values);
						if (isInputValid)
						{
							Tuple lightPosition = Tuple::Point(values[0], values[1], values[2]);
							Tuple lightColour = Color::Color(values[3], values[4], values[5]);

							lights.push_back(std::make_shared<Light>(Light(lightPosition, lightColour,
								values[6], values[7], values[8], values[9])));
						}
					}

					if (cmd == "vertex")
					{
						// 3 values x y z
						isInputValid = readValues(s, 3, values);
						if (isInputValid)
						{
							vertices.push_back(Tuple::Vector(values[0], values[1], values[2]));
						}
					}

					if (cmd == "material")
					{
						// 3 values r g b, ambient, diffuse, specular, shininess
						isInputValid = readValues(s, 7, values);
						if (isInputValid)
						{
							MaterialPtr m = std::make_shared<Material>(Material(Color(values[0], values[1], values[2]),
								values[3], values[4], values[5], values[6]));
							currentMaterial = m;
						}
					}

					if (cmd == "tri")
					{
						// 3 values vertex1 vertex2 vertex3
						isInputValid = readValues(s, 3, values);
						if (isInputValid)
						{
							ObjectPtr triangle = std::make_shared<Triangle>(Triangle(vertices[(int)values[0]],
								vertices[(int)values[1]],
								vertices[(int)values[2]], currentMaterial));
							objects.push_back(triangle);
						}
					}

					if (cmd == "sphere")
					{
						// 4 center: vertex1 vertex2 vertex3 + radius
						isInputValid = readValues(s, 4, values);
						if (isInputValid)
						{
							ObjectPtr sphere = std::make_shared<Sphere>(Sphere(Tuple::Vector(values[0],
								values[1], values[2]), values[3], currentMaterial));
							objects.push_back(sphere);
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