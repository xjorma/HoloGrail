#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/Image.h"
#include "./headers/Noise.h"
#include "./headers/MathUtils.h"
#include "./headers/VoxUtils.h"

Water::Water(int _size, const std::vector<HSpan> _spans, float _timePerSpan, float _timeGrowSpan, float _timeFade) : size(_size), spans(_spans), timePerSpan(_timePerSpan), timeGrowSpan(_timeGrowSpan), timeFade(_timeFade), time(0), vw(nullptr), globalIntensity(1.f)
{
	// Init buffers
	for(int i = 0; i < 2; i++)
	{
		simulBuffer[i] = new Image<float>(size, size);
		simulBuffer[i]->Fill(0.f);
	}

	// Try to do an initial pertubation
	for (int i = 0; i < 256; i++)
	{
		int r = int(mix(24.0f, 32.0f, hash11(float(i) * 7.0f)));
		vec2 c = vec2(r + hash11(float(i) * 3.0f) * (size - r * 2.f), r + hash11(float(i) * 5.0f) * (size - r * 2.f));
		Water::DrawSpot(c, r, 0.02f * mix(-1.0f, 1.0f, hash11(float(i) * 11.0f)));
	}

	heightMap = new Image<int>(size, size);
	heightMap->Fill(0x7fffffff);
}

Water::~Water()
{
	for (int i = 0; i < 2; i++)
	{
		delete simulBuffer[i];
	}
	delete heightMap;
}

void  Water::DrawSpot(vec2 c, int r, float i)
{
	for (int y = -r; y < r; y++)
	{
		for (int x = -r; x < r; x++)
		{
			float h = simulBuffer[0]->Read(x + (int)c.x, y + (int)c.y);
			float d = length(vec2(x, y));
			h -= smoothstep((float)r, 0.f, d) * i;
			simulBuffer[0]->Write(x + (int)c.x, y + (int)c.y, h);
		}
	}
}

void Water::Temper()
{
	// Random spot
	int r = 16;
	vec2 c = vec2(r + hash11(time * 13) * (size - r * 2.f), r + hash11(time * 29) * (size - r * 2.f));
	Water::DrawSpot(c, r, 0.02f);
	// Lissajous trail
	r = 32;
	float a = 3.f;
	float b = 2.f;
	float A = 0.95f;
	float B = 0.95f;
	float delta = Pi / 2.f;
	c = vec2(fit(sinf(time * a + delta) * A, -1.f, 1.f, (float)r, (float)(size - r)), fit(sinf(time * b) * B, -1.f, 1.f, (float)r, (float)(size - r)));
	Water::DrawSpot(c, r, 0.015f);

	float ft = fmodf(time, timePerSpan);
	int nb = int(time / timePerSpan) % spans.size();

	if (ft < timeGrowSpan)
	{
		float alt = fit(ft, 0.f, timeGrowSpan, -0.75, 0.75);
		for (int i = 0; i < 2; i++)
		{
			simulBuffer[i]->Draw(spans[nb], [alt](float v) -> float {return max(v, alt); });
		}
	}
}

void Water::Simulate(int start, int end)
{
	vec3 ldir = normalize(vec3(.0f, .0f, 6.0f));
	for (int y = start; y < end; y++)
	{
		for (int x = 1; x < size; x++)
		{
			// water simulation
			float curPos = simulBuffer[0]->Read(x, y);
			float oldPos = simulBuffer[1]->Read(x, y);
			float vel = curPos - oldPos;
			float l = simulBuffer[0]->Read(x - 1, y);
			float r = simulBuffer[0]->Read(x + 1, y);
			float b = simulBuffer[0]->Read(x, y - 1);
			float f = simulBuffer[0]->Read(x, y + 1);
			float averagePos = (l + r + b + f) * 0.25f;
			vel += (averagePos - curPos) * 2.f;
			curPos += vel;
			curPos *= 0.985f;
			curPos = clamp(curPos, -1.f, 1.f);
			simulBuffer[1]->Write(x, y, curPos);
			// compute normal
			vec3 vx = vec3(2.f / size, 0.f, (r - l) * vw->aspz * 0.5f);
			vec3 vy = vec3(0.f, 2.f / size, (f - b) * vw->aspz * 0.5f);
			vec3 n = normalize(cross(vx, vy));
			// lighting
			float li = max(dot(n, ldir), 0.f) * globalIntensity;
			int c = intCol(li);
			int ih = int(128.f - curPos * 127.f);
			heightMap->Write(x, y, (ih << 24) | c);
		}
	}
}

void Water::Init(voxie_wind_t& _vw)
{
	// Number of thread
	nbThreads = GetThreadsNumber(_vw);
	vw = &_vw;
}

void Water::Tick(float _dt)
{
	time += _dt;
	// Fade processing
	float totalTime = timePerSpan * spans.size();
	globalIntensity = min(time / timeFade, 1.f);	// Fade-In
	globalIntensity *= clamp(1.f - (time - (totalTime - timeFade)) / timeFade, 0.f, 1.f);
	if (time > totalTime)
		finished = true;
	// Swap buffer
	Image<float> *swap = simulBuffer[0];
	simulBuffer[0] = simulBuffer[1];
	simulBuffer[1] = swap;
	// Perturbate Water 
	Temper();
	// Simulation
	if (false)
	{
		// MultiThreaded Simulation
		std::vector<std::thread> threads;
		threads.reserve(nbThreads);
		float cur = 1.f;
		float inc = (float)(size - 2) / (float)nbThreads;
		for (int i = 0; i < nbThreads; i++)
		{
			float next = cur + inc;
			threads.emplace_back(&Water::Simulate, this, (int)cur, (int)next);
			cur = next;
		}
		for (int i = 0; i < nbThreads; i++)
		{
			threads[i].join();
		}
	}
	else
	{
		Simulate(1, size - 1);
	}
}

void Water::Draw(voxie_frame_t* _vf)
{
	tiletype tt = heightMap->GetTileType();
#if 1
	vec3 rr(vw->aspx * 2.f, 0.f, 0.f);
	vec3 dd(0.f, vw->aspy * 2.f, 0.f);
	vec3 ff(0.f, 0.f, vw->aspz * 2.f);
	vec3 pp(rr.x * -.5f, dd.y * -.5f, ff.z * -.5f);
	voxie_drawheimap(_vf, (char*)&tt, vPtr(pp), vPtr(rr), vPtr(dd), vPtr(ff), 0, -1, (1 << 2) + (1 << 3));
#else
	poltex_t vertex[4];
	vec3 c = vec3(0.f);
	float r = 1.f;
	vertex[0].x = c.x - r; vertex[0].y = c.y - r; vertex[0].z = c.z; vertex[0].u = 0.f; vertex[0].v = 0.f; vertex[0].col = 0xffffff;
	vertex[1].x = c.x - r; vertex[1].y = c.y + r; vertex[1].z = c.z; vertex[1].u = 0.f; vertex[1].v = 1.f; vertex[1].col = 0xffffff;
	vertex[2].x = c.x + r; vertex[2].y = c.y + r; vertex[2].z = c.z; vertex[2].u = 1.f; vertex[2].v = 1.f; vertex[2].col = 0xffffff;
	vertex[3].x = c.x + r; vertex[3].y = c.y - r; vertex[3].z = c.z; vertex[3].u = 1.f; vertex[3].v = 0.f; vertex[3].col = 0xffffff;
	int		 indices[5];
	for (int i = 0; i < 4; i++)
		indices[i] = i;
	indices[4] = -1;
	voxie_drawmeshtex(_vf, (char*)&tt, vertex, 4, indices, 5, 2 + 8, 0xffffff);
#endif

}
