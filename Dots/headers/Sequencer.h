#pragma once

class Effect;

struct SeqEntry
{
	double	startTime;
	Effect	*effect;
};

class Sequencer
{
	std::vector<SeqEntry>	entries;
public:
	Sequencer(const std::vector<SeqEntry> &_entries) : entries(_entries)
	{
	}

	~Sequencer()
	{
		for (SeqEntry& entry : entries)
		{
			delete entry.effect;
		}
	}

	void Tick(double time, float dtime, voxie_frame_t* _vf);
	void Init(voxie_wind_t& _vw);
};
