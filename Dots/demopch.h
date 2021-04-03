#pragma once
// "C" 

#include <stdlib.h>
#include <math.h>

// std

#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <random>

// Voxon
extern "C"
{
	#include "voxieinc.h"
}

// GLM
//#define GLM_FORCE_MESSAGES
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
using namespace::glm;

// Math

const float Pi = 3.14159265358979323f;
const float	Tau = Pi * 2.f;

// Libs

#include <Fade_3D.h>

