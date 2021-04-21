#ifndef READFILE_H
#define READFILE_H

#pragma once

#pragma warning(push, 0)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "scene.h"

using namespace std;

namespace ReadScene
{
	Scene RAYTRACER_EXPORT readSceneFile(string filename);
}
#endif