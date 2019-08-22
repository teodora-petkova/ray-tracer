#ifndef VARIABLES_H
#define VARIABLES_H

#pragma once

#include "vector3.h"
#include "triangle.h"
#include "sphere.h"
#include <vector>

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif 

#ifdef MAINPROGRAM 
Vector3 lookfrom(0.0, 0.0, 5.0); // Initial eye position, also for resets
Vector3 lookat(0.0, 1.0, 0.0); // Initial up position, also for resets
Vector3 up(0.0, 0.0, 0.0); // Center look at point 
int width = 600, height = 400; // width and height 
float fovy = 90.0; // For field of view
std::vector<Vector3> vertices = std::vector<Vector3>();
std::vector<Triangle> triangles = std::vector<Triangle>();
std::vector<Sphere> spheres = std::vector<Sphere>();
#else 
EXTERN Vector3 lookfrom;
EXTERN Vector3 lookat;
EXTERN Vector3 up;
EXTERN int width, height;
EXTERN float fovy;
EXTERN std::vector<Vector3> vertices;
EXTERN std::vector<Triangle> triangles;
EXTERN std::vector<Sphere> spheres;
#endif


#endif