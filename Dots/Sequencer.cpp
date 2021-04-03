#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/Sequencer.h"

void Sequencer::Tick(double time, float dtime, voxie_frame_t *_vf)
{
	for (SeqEntry &entry : entries)
	{
		if(time > entry.startTime && !entry.effect->isFinished())
		{
			entry.effect->Tick(dtime);
			entry.effect->Draw(_vf);
		}
	}
}

void Sequencer::Init(voxie_wind_t& _vw)
{
	for (SeqEntry& entry : entries)
	{
		entry.effect->Init(_vw);
	}
}