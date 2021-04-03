#pragma once

#include "./headers/geometries.h"

class Geometry4d
{
public:
	std::vector<vec4>	vertices;
	std::vector<Line>	lines;
	Geometry4d(const std::vector<vec4>& _vertices, const std::vector<Line>& _lines) : vertices(_vertices), lines(_lines)
	{
	}
};

extern Geometry4d HyperSoccerBall;
