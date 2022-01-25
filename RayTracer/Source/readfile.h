#pragma once

#pragma warning(push, 0)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "scene.h"
#include "objparser.h"
#include "matrix.h"

namespace ReadScene
{
	Scene RAYTRACER_EXPORT ReadSceneFile(const std::string& filename);
}