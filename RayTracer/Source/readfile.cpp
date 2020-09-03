#include "readfile.h"
#include "scenedata.h"

namespace ReadScene
{
	SceneData readSceneFile(string filename)
	{
		SceneData sceneData = SceneData();

		string str, cmd;
		ifstream in;
		in.open(filename);
		if (in.is_open())
		{
			getline(in, str);

			Material currentMaterial;

			while (in)
			{
				// ruled out comment and blank lines 
				if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
				{
					stringstream s(str);
					s >> cmd;

					// Position and color for light, colors for others - up to 10 parameters for cameras
					float values[10];

					bool isInputValid; // Validity of input 
					if (cmd == "size")
					{
						isInputValid = readValues(s, 2, values); // 10 values eye cen up fov
						if (isInputValid)
						{
							//size width height
							sceneData.ImageWidth = (int)values[0];
							sceneData.ImageHeight = (int)values[1];
						}
					}

					if (cmd == "camera")
					{
						isInputValid = readValues(s, 10, values); // 10 values eye center up fov
						if (isInputValid)
						{
							//camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy
							sceneData.LookFrom = Vector3(values[0], values[1], values[2]); //lookfrom
							sceneData.LookAt = Vector3(values[3], values[4], values[5]); //lookat
							sceneData.Up = Vector3(values[6], values[7], values[8]); //up - normalized vector made from upx, upy, upz values.
							sceneData.Up.normalize();
							sceneData.FovY = values[9];
						}
					}

					if (cmd == "distant_light")
					{
						isInputValid = readValues(s, 7, values); // 7 values light position x y z, light color r g b, intensity i
						if (isInputValid)
						{
							Vector3 lightPosition = Vector3(values[0], values[1], values[2]);
							Vector3 lightColour = Vector3(values[3], values[4], values[5]);
							float intensity = values[6];
							sceneData.Lights.push_back(Light(lightPosition, lightColour, intensity));
						}
					}

					if (cmd == "spherical_light")
					{
						isInputValid = readValues(s, 7, values); // 7 values light position x y z, light color r g b, intensity i
						if (isInputValid)
						{
							Vector3 lightPosition = Vector3(values[0], values[1], values[2]);
							Vector3 lightColour = Vector3(values[3], values[4], values[5]);
							float intensity = values[6];
							sceneData.Lights.push_back(Light(lightPosition, lightColour, intensity));
						}
					}

					if (cmd == "vertex")
					{
						isInputValid = readValues(s, 3, values); // 3 values x y z
						if (isInputValid)
						{
							sceneData.Vertices.push_back(Vector3(values[0], values[1], values[2]));
						}
					}

					if (cmd == "ambient")
					{
						isInputValid = readValues(s, 3, values); //3 values r g b
						if (isInputValid)
						{
							Material m = Material();
							m.setColor(values[0], values[1], values[2]);
							currentMaterial = m;
						}
					}

					if (cmd == "tri")
					{
						isInputValid = readValues(s, 3, values); //3 values vertex1 vertex2 vertex3
						if (isInputValid)
						{
							Triangle t = Triangle(sceneData.Vertices[(int)values[0]],
								sceneData.Vertices[(int)values[1]],
								sceneData.Vertices[(int)values[2]]);
							t.setMaterial(currentMaterial);
							sceneData.Triangles.push_back(t);
						}
					}

					if (cmd == "sphere")
					{
						isInputValid = readValues(s, 4, values); //4 center: vertex1 vertex2 vertex3 + radius
						if (isInputValid)
						{
							Sphere s = Sphere(Vector3(values[0], values[1], values[2]), values[3]);
							s.setMaterial(currentMaterial);
							sceneData.Spheres.push_back(s);
						}
					}

					for (int ind = 0; ind < 10; ind++) values[ind] = 0;
				}
				getline(in, str);
			}
		}

		return sceneData;
	}

	// Function to read the input data values
	// Use is optional, but should be very helpful in parsing.  
	bool readValues(stringstream &s, const int numvals, float* values)
	{
		for (int i = 0; i < numvals; i++) {
			s >> values[i];
			if (s.fail()) {
				cout << "Failed reading value " << i << " will skip\n";
				return false;
			}
		}
		return true;
	}
}