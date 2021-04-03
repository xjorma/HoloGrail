#include "demopch.h"
#include "./headers/effects.h"
#include "./headers/MathUtils.h"
#include "./headers/Noise.h"
#include "./headers/VoxUtils.h"
#include <algorithm>

void Dissolve::Init(voxie_wind_t& _vw)
{
	// Number of thread
	nbThreads = GetThreadsNumber(_vw);

	// Generate points along the mesh
	geo.GeneratePoints(points, nbPoints, 0.001f / scale);
	mat4x4 transform = RotX(angle) * mat4x4(scale);
	std::transform(points.begin(), points.end(), points.begin(),[transform](const vec3& p) {return transform * vec4(p, 1.0f);});
}

void Dissolve::Tick(float _dt)
{
	time += _dt;
	dTime = _dt;
}

void Dissolve::multiDrawSimple(int start, int end, voxie_frame_t* _vf, float ratio)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	float t = time - (fadeInTime + logoTime);
	for (int i = start; i < end; i++)
	{
		if (hash11((float)i) < ratio)
		{
			vec3 dst = points[i];

			vt[curIndexInBatch].x = dst.x; vt[curIndexInBatch].y = dst.y; vt[curIndexInBatch].z = dst.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
			curIndexInBatch++;
			if (curIndexInBatch == batchSize)
			{
				std::lock_guard<std::mutex> guard(drawMutex);
				voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
				curIndexInBatch = 0;
			}
		}
	}
	if (curIndexInBatch)
	{
		std::lock_guard<std::mutex> guard(drawMutex);
		voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
	}
}


void Dissolve::multiDraw(int start, int end, voxie_frame_t* _vf)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	float t = time - (fadeInTime + logoTime);
	for (int i = start; i < end; i++)
	{
		float deathFactor = hash11(i * 13.f);
		if (t < mix(dissolveTime, dissolveTime  + fadeOutTime, deathFactor))
		{
			vec3 src = points[i];

			float noiseScale = 4.;
			vec3 p1 = src * noiseScale;
			vec3 p2 = p1 + vec3(31.341f, -43.23f, 12.34f);		//random offset
			vec3 p3 = p2 + vec3(-231.341f, 124.23f, -54.34f);	//random offset

			p1 -= vec3(0.f, 0.f, t);
			p2 -= vec3(0.f, 0.f, t * 0.91f);
			p3 -= vec3(0.f, 0.f, t * 0.79f);

			vec3 d1 = SimplexPerlin3D_Deriv(p1).yzw;
			vec3 d2 = SimplexPerlin3D_Deriv(p2).yzw;
			vec3 d3 = SimplexPerlin3D_Deriv(p3).yzw;

			vec3 curlDir = vec3(d3.y - d2.z, d1.z - d3.x, d2.x - d1.y);

			float speedFactor = hash11((float)i);

			vec3 dst = src + curlDir * mix(0.08f, 0.020f, speedFactor) * dTime;

			points[i] = dst;

			vt[curIndexInBatch].x = dst.x; vt[curIndexInBatch].y = dst.y; vt[curIndexInBatch].z = dst.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
			curIndexInBatch++;
			if (curIndexInBatch == batchSize)
			{
				std::lock_guard<std::mutex> guard(drawMutex);
				voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
				curIndexInBatch = 0;
			}
		}
	}
	if (curIndexInBatch)
	{
		std::lock_guard<std::mutex> guard(drawMutex);
		voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
	}
}

void Dissolve::Draw(voxie_frame_t* _vf)
{
	if (time < (fadeInTime + logoTime))
	{
		float ratio = min(time / fadeInTime, 1.f);
		multiDrawSimple(0, nbPoints, _vf, ratio);
	}
	else if (time > fadeInTime + logoTime + dissolveTime + fadeOutTime)
	{
		finished = false;
	}
	else
	{
		// Dissolve
		if (false)
		{
			std::vector<std::thread> threads;
			threads.reserve(nbThreads);
			float cur = 0.f;
			float inc = (float)nbPoints / (float)nbThreads;
			for (int i = 0; i < nbThreads; i++)
			{
				float next = cur + inc;
				threads.emplace_back(&Dissolve::multiDraw, this, (int)cur, (int)next, _vf);
				cur = next;
			}
			for (int i = 0; i < nbThreads; i++)
			{
				threads[i].join();
			}
		}
		else
		{
			multiDraw(0, nbPoints, _vf);
		}
	}
}
