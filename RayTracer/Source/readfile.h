#ifndef READFILE_H
#define READFILE_H

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "scene.h"

using namespace std;

namespace ReadScene
{
	Scene __declspec(dllexport) readSceneFile(string filename);
}
#endif