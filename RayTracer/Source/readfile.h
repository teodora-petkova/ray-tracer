#ifndef READFILE_H
#define READFILE_H

#pragma once

// Readfile definitions 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "variables.h"

using namespace std;

namespace readfiles
{
	bool readvals(stringstream &s, const int numvals, float * values);
	void readfile(const char * filename);
}
#endif