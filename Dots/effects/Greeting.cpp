#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"
#include "./headers/Noise.h"
#include "./headers/VoxUtils.h"

#include <algorithm>
#include <random>

void Greeting::Init(voxie_wind_t& _vw)
{
	for(int i = 0; i < geos.size(); i++)
	{ 
		const GeoScale& geoScale = geos[i];
		std::vector<vec3> points;
		float globalScale = scale * geoScale.scale;
		geoScale.geo->GeneratePoints(points, nbPoints, 0.005f / globalScale);
		vec3 rotAmount(0.2f, 0.1f, 0.6f);
		mat4x4 transform = fromEuler((hash31(float(i)) - vec3(0.5f)) * rotAmount) * RotX(radians(30.0f)) * Scale(vec3(scale * globalScale));
		for (vec3& p : points)
			p = transform * vec4(p, 1.0f);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(points.begin(), points.end(), g);
		pointsVec.push_back(points);
	}
}

void Greeting::Tick(float _dt)
{
	time += _dt;
}

void Greeting::multiDraw(int start, int end, voxie_frame_t* _vf, float ratio)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	int flTime = int(floor(time / freq));
	float frTime = fract(time / freq);

	const std::vector<vec3>& pointsOrg = pointsVec[flTime % pointsVec.size()];
	const std::vector<vec3>& pointsDst = pointsVec[(flTime + 1) % pointsVec.size()];

	//float alpha = ElasticTweenInOut(frTime);
	//float alpha = ElasticTweenInOut(frTime * 0.7f + 0.15f);
	float alpha = ElasticTweenInOut(frTime * 0.8f + 0.10f);
	//float alpha = BackTweenInOut(frTime);
	for (int i = start; i < end; i++)
	{
		if (hash11(float(i)) < ratio)
		{
			vec3 org = pointsOrg[i];
			vec3 dst = pointsDst[i];

			vec3 final = mix(org, dst, alpha);

			vt[curIndexInBatch].x = final.x; vt[curIndexInBatch].y = final.y; vt[curIndexInBatch].z = final.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
			curIndexInBatch++;
			if (curIndexInBatch == batchSize)
			{
				voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
				curIndexInBatch = 0;
			}
		}
	}
	if (curIndexInBatch)
	{
		voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
	}
}

void Greeting::Draw(voxie_frame_t* _vf)
{
	float duration = float(pointsVec.size() - 1) * freq;
	if (time > duration)
		finished = true;
	float ratio = smoothsteplin(0.0f, 1.0f, time) * smoothsteplin(duration, duration - 1.0f, time);
	multiDraw(0, nbPoints, _vf, ratio);
}
