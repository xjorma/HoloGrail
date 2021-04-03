#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/noise.h"
#include "./headers/VoxUtils.h"
#include "./headers/MathUtils.h"

void CollapsingTiles::Init(voxie_wind_t& _vw)
{

}

void CollapsingTiles::Tick(float _dt)
{
	time += _dt;
}

void CollapsingTiles::Draw(voxie_frame_t* _vf)
{
	if (time > duration)
		finished = true;
	int col = intCol(smoothsteplin(0.0f, fadeTime, time) * smoothsteplin(duration, duration - fadeTime, time));

	float frTime = fract(time / speed);
	float flTime = floor(time / speed);
	pol_t pt[4];
	pt[0].p2 = 1;
	pt[1].p2 = 2;
	pt[2].p2 = 3;
	pt[3].p2 = 0;

	float len = 2.f / float(nbTiles);
	float r = len * 0.5f * 0.8f;
	for (int i = 0; i < nbTiles; i++)
	{
		for (int j = 0; j < nbTiles; j++)
		{
			vec2 c = len * 0.5f + vec2(float(i),float(j)) * len - 1.f;
			float noise0 = noise(c * 3.f + flTime + 10.f);
			float noise1 = noise(c * 6.f + flTime + 10.f);
			float noise2 = noise(c * 12.f + flTime + 10.f);
			float noise = noise0 * 0.75f + noise1 * 0.15f + noise2 * 0.10f;
			noise = noise * 0.5f + 0.5f;
			float z = -(smoothstep(noise - 0.05f, noise + 0.05f, frTime) - frTime) * 0.35f;
			pt[0].x = c.x - r; pt[0].y = c.y - r; pt[0].z = z;
			pt[1].x = c.x - r; pt[1].y = c.y + r; pt[1].z = z;
			pt[2].x = c.x + r; pt[2].y = c.y + r; pt[2].z = z;
			pt[3].x = c.x + r; pt[3].y = c.y - r; pt[3].z = z;
			voxie_drawpol(_vf, pt, 4, col);
		}
	}
}
