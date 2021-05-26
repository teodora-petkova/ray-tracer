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

namespace ReadScene
{
	Scene RAYTRACER_EXPORT ReadSceneFile(const std::string& filename);
}
#endif