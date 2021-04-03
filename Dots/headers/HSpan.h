#pragma once

#include <vector>

class HSpan
{
public:
	struct Entry
	{
		int xIn;
		int y;
		int xOut;
	};
	std::vector<Entry>	entries;
	HSpan(const std::vector<Entry> _entries) : entries(_entries)
	{
	}
};

extern HSpan HoloGrail;
extern HSpan Grail;
