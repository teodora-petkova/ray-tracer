#ifndef READFILE_H
#define READFILE_H

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "scenedata.h"

using namespace std;

namespace ReadScene
{
	bool ReadValues(stringstream &s, const int numvals, float * values);
	SceneData ReadSceneFile(string filename);
}
#endif