///*****************************************************************************/
///* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
///* Extends HW 1 to deal with shading, more transforms and multiple objects   */
///*****************************************************************************/
//
///*****************************************************************************/
//// This file is readfile.cpp.  It includes helper functions for matrix 
//// transformations for a stack (matransform) and to rightmultiply the 
//// top of a stack.  These functions are given to aid in setting up the 
//// transformations properly, and to use glm functions in the right way.  
//// Their use is optional in your program.  
//  
//
//// The functions readvals and readfile do basic parsing.  You can of course 
//// rewrite the parser as you wish, but we think this basic form might be 
//// useful to you.  It is a very simple parser.
//
//// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
//// Read the other parts to get a context of what is going on. 
//  
///******************************************************************-***********/


#include "readfile.h"
namespace readfiles
{
	void readfile(const char* filename)
	{
		string str, cmd;
		ifstream in;
		in.open(filename);
		if (in.is_open())
		{
			getline(in, str);

			Material currentMaterial;

			while (in)
			{
				if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
				{
					// Ruled out comment and blank lines 

					stringstream s(str);
					s >> cmd;
					//int i; 
					float values[10]; // Position and color for light, colors for others
										// Up to 10 params for cameras.  
					bool validinput; // Validity of input 
					if (cmd == "size")
					{
						validinput = readvals(s, 2, values); // 10 values eye cen up fov
						if (validinput)
						{
							//size width height
							width = (int)values[0];
							height = (int)values[1];
						}
					}

					if (cmd == "camera")
					{
						validinput = readvals(s, 10, values); // 10 values eye cen up fov
						if (validinput)
						{
							//camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy
							lookfrom = Vector3(values[0], values[1], values[2]); //lookfrom
							lookat = Vector3(values[3], values[4], values[5]); //lookat
							up = Vector3(values[6], values[7], values[8]); //up - normalized vector made from upx, upy, upz values.
							up.Normalize();
							fovy = values[9]; //fovy
						}
					}

					if (cmd == "vertex")
					{
						validinput = readvals(s, 3, values); // 3 values x y z
						if (validinput)
						{
							vertices.push_back(Vector3(values[0], values[1], values[2]));
						}
					}

					if (cmd == "ambient")
					{
						validinput = readvals(s, 3, values); //3 values r g b
						if (validinput)
						{
							Material m = Material();
							m.SetColor(values[0], values[1], values[2]);
							currentMaterial = m;
						}
					}

					if (cmd == "tri")
					{
						validinput = readvals(s, 3, values); //3 values vertex1 vertex2 vertex3
						if (validinput)
						{
							Triangle t = Triangle(vertices[(int)values[0]], vertices[(int)values[1]], vertices[(int)values[2]]);
							t.SetMaterial(currentMaterial);
							triangles.push_back(t);
						}
					}
					if (cmd == "sphere")
					{
						validinput = readvals(s, 4, values); //4 center: vertex1 vertex2 vertex3 + radius
						if (validinput)
						{
							Sphere s = Sphere(Vector3(values[0], values[1], values[2]), values[3]);
							s.SetMaterial(currentMaterial);
							spheres.push_back(s);
						}
					}

					for (int ind = 0; ind < 10; ind++) values[ind] = 0;
				}
				getline(in, str);

			}
		}
	}

	// Function to read the input data values
	// Use is optional, but should be very helpful in parsing.  
	bool readvals(stringstream &s, const int numvals, float* values)
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