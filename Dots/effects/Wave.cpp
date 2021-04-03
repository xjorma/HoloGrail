#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"
#include "./headers/Image.h"
#include "./headers/VoxUtils.h"

Wave::Wave(int _gridSize, float _height, float _duration, float _fadeTime, float _phase) : gridSize(_gridSize), height(_height), duration(_duration), fadeTime(_fadeTime), phase(_phase), time(0.f)
{
	buffer = new Image<vec3>(gridSize, gridSize);
}

Wave::~Wave()
{
	delete buffer;
}
void Wave::Init(voxie_wind_t& _vw)
{
}

void Wave::Tick(float _dt)
{
	time += _dt;
	// Animate Parameter
	float frTime = fract(time / phase);
	float flTime = floor(time / phase);
	maxv = mix(0.0f, 12.0f, smoothstep(1.0f, 0.5f, frTime));;
	exp = 7.9f;
	angle = mix(0.0f, 12.f, smoothstep(0.0f, 0.5f, frTime));
	deformPos = vec3(fit(frTime, 0.0f, 1.0f, -1.2f, 1.5f), 0.0f, mexHat(frTime) * 0.22f);
	angleZ = (hash11(flTime) - 0.5f) * 2.0f * radians(15.f);
}

vec3 Wave::Read(int x, int y) const
{
	return buffer->Read(x, y);
}

// https://www.shadertoy.com/view/MdXyzX

vec2 wavedx(vec2 position, vec2 direction, float speed, float frequency, float timeshift)
{
	float x = dot(direction, position) * frequency + timeshift * speed;
	float wave = exp(sin(fmod(x, 2.f * Pi)) - 1.0f);
	float dx = wave * cos(fmod(x, 2.f * Pi));
	return vec2(wave, -dx);
}

#define DRAG_MULT 0.048f

float getwaves(vec2 position, int iterations, float time)
{
	float iter = 0.0f;
	float phase = 6.0f;
	float speed = 2.0f;
	float weight = 1.0f;
	float w = 0.0f;
	float ws = 0.0f;
	for (int i = 0; i < iterations; i++)
	{
		vec2 p = vec2(sin(iter), cos(iter));
		vec2 res = wavedx(position, p, speed, phase, -time);
		position += normalize(p) * res.y * weight * DRAG_MULT;
		w += res.x * weight;
		iter += 12.0;
		ws += weight;
		weight = mix(weight, 0.0f, 0.2f);
		phase *= 1.18f;
		speed *= 1.07f;
	}
	return w / ws;
}


vec3 Wave::Compute(vec2 v, const mat4 &deform, const mat4 &ideform) const
{
	vec3 p = vec3(v, 0.0f);
	p.z += getwaves(v * 2.5f, 7, time) * 0.1f;			// little wave
	p = ideform * vec4(p, 1.0f);
	float d = length(p);
	d = fit(d, 0, maxv, 1.0f, 0.0f);
	d = max(d, 0.0f);
	d = powf(d, exp);
	p = RotY(-d * angle) * vec4(p, 1.0f);
	p = deform * vec4(p, 1.0f);
	p.z += height;
	return p;

}

static void setVert(poltex_t &v, const vec3& p)
{
	v.x = p.x;
	v.y = p.y;
	v.z = p.z;
	v.u = 0.0f;
	v.v = 0.0f;
	v.col = 0xffffff;
}

void Wave::Draw(voxie_frame_t* _vf)
{
	if (time > duration)
		finished = true;
	int col = intCol(smoothsteplin(0.0f, fadeTime, time) * smoothsteplin(duration, duration - fadeTime, time));
	// Update
	mat4 deform =  RotZ(angleZ) * Translate(deformPos) * Scale(vec3(0.1f, 0.4f, 0.1f));
	mat4 iDeform = inverse(deform);
	for (int y = 0; y < gridSize; y++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			float fx = fit(float(x), 0.0f, float(gridSize - 1), -1.0f, 1.0f);
			float fy = fit(float(y), 0.0f, float(gridSize - 1), -1.0f, 1.0f);
			buffer->Write(x, y, Compute(vec2(fx, fy), deform, iDeform));
		}
	}

	const bool drawWireframe = true;
	if (drawWireframe)
	{
		for (int y = 0; y < gridSize - 1; y++)
		{
			for (int x = 0; x < gridSize - 1; x++)
			{
				vec3 h00 = Read(x, y);
				vec3 h10 = Read(x + 1, y);
				vec3 h01 = Read(x, y + 1);
				voxie_drawlin(_vf, h00.x, h00.y, h00.z, h10.x, h10.y, h10.z, col);
				voxie_drawlin(_vf, h00.x, h00.y, h00.z, h01.x, h01.y, h01.z, col);
			}
		}
		// draw last line
		for (int x = 0; x < gridSize - 1; x++)
		{
			vec3 h00 = Read(x, gridSize - 1);
			vec3 h10 = Read(x + 1, gridSize - 1);
			voxie_drawlin(_vf, h00.x, h00.y, h00.z, h10.x, h10.y, h10.z, col);
		}
		// draw last row
		for (int y = 0; y < gridSize - 1; y++)
		{
			vec3 h00 = Read(gridSize - 1, y);
			vec3 h01 = Read(gridSize - 1, y + 1);
			voxie_drawlin(_vf, h00.x, h00.y, h00.z, h01.x, h01.y, h01.z, col);
		}
	}
	else // draw solid
	{
		int nbVertPerRow = (gridSize - 1) * 6;
		int nbIdPerRow = (gridSize - 1) * 8;
		poltex_t *vertices = (poltex_t *)_malloca(nbVertPerRow * sizeof(poltex_t));
		int *indices = (int*)_malloca(nbIdPerRow * sizeof(int));
		for (int i = 0; i < (gridSize - 1) * 2; i++)
		{
			indices[i * 4 + 0] = i * 3 + 0;
			indices[i * 4 + 1] = i * 3 + 1;
			indices[i * 4 + 2] = i * 3 + 2;
			indices[i * 4 + 3] = -1;
		}

		for (int y = 0; y < gridSize - 1; y++)
		{
			for (int x = 0; x < gridSize - 1; x++)
			{
				vec3 h00 = Read(x, y);
				vec3 h10 = Read(x + 1, y);
				vec3 h01 = Read(x, y + 1);
				vec3 h11 = Read(x + 1, y + 1);
				setVert(vertices[x * 6 + 0], h00);
				setVert(vertices[x * 6 + 1], h10);
				setVert(vertices[x * 6 + 2], h01);
				setVert(vertices[x * 6 + 3], h10);
				setVert(vertices[x * 6 + 4], h11);
				setVert(vertices[x * 6 + 5], h01);
			}
			voxie_drawmeshtex(_vf, nullptr, vertices, nbVertPerRow, indices, nbIdPerRow, 2, intCol(0.5f));
		}
		_freea(indices);
		_freea(vertices);
	}

	// Debug draw to see deformer
	//World.Draw(_vf, deform, 0xffffff);
}
