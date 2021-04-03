#pragma once

#include <vector>
#include "./headers/MathUtils.h"

struct ColSeqEntry
{
	double	startTime;
	vec3	color;
};

class ColorSequencer
{
	vec3 curColor;
	float globalIntensity;
	std::vector<ColSeqEntry>	entries;
public:
	ColorSequencer(const std::vector<ColSeqEntry> &_entries, float _globalIntensity) : entries(_entries), curColor(0.f), globalIntensity(_globalIntensity)
	{
	}

	void Tick(double time, float dtime)
	{
		int id = 0;
		for(int i = 0; i < entries.size(); i++)
		{
			if (time > entries[i].startTime)
			{
				id = i;
			}
		}
		curColor = LowPassFilterInterpolation(dtime, 0.5f, curColor, entries[id].color);
		vec3 c = curColor * 255.f * globalIntensity;
		voxie_setleds(0, int(c.r), int(c.g), int(c.b));
	}
};
