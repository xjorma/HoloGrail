#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/noise.h"
#include "./headers/MathUtils.h"
#include "./headers/VoxUtils.h"
#include "./headers/Image.h"

const float restitution = 13. / 16.;
const float dropper = 1.6f;
const float dotscale = 1.f / 40.f;
const float baseGravity = 9.8f / 200.f;
const int	wrapFrame = 3000;
const int	nbPart = 512;
const float	tau = glm::radians(360.f);

inline float intToRad(int a)
{
	return float(a) * tau / 1024.f;
}

vec4 spawnNewParticle(int f)
{
	vec4 dotInfo;
	if (f < 500)
	{
		dotInfo.x = sin(intToRad(f * 11)) * 120.f * dotscale;
		dotInfo.y = cos(intToRad(f * 13)) * 40.f * dotscale + dropper;
		dotInfo.z = sin(intToRad(f * 17)) * 120.f * dotscale;
		dotInfo.w = 0.;
	}
	else if (f < 900)
	{
		dotInfo.x = cos(intToRad(f * 15)) * 150.f * dotscale;
		dotInfo.y = dropper + 0.5f;
		dotInfo.z = sin(intToRad(f * 15)) * 150.f * dotscale;
		dotInfo.w = 2.5;
	}
	else if (f < 1700)
	{
		float a = sin(intToRad(f)) * 110.f;
		dotInfo.x = cos(intToRad(f * 66)) * a * dotscale;
		dotInfo.y = 0.f;
		dotInfo.z = sin(intToRad(f * 66)) * a * dotscale;
		dotInfo.w = 7.f;
	}
	else if (f < 2360)
	{
		vec3 p;
		for (;;)
		{
			p = (hash31(float(f)) - 0.5f) * 2.f * 4.f;
			vec2 vec2d = p.xz;
			if(length(vec2d) < 4.f)
			{
				break;
			}
			f += 513;
		}
		dotInfo.x = p.x;
		dotInfo.y = abs(p.y * 0.5f);
		dotInfo.z = p.z;
		dotInfo.w = 0.f;
	}
	else
	{
		dotInfo = vec4(vec3(4000.), 0);
	}
	
	return dotInfo;
}

BouncingDots::~BouncingDots()
{
	delete imageRT;
	delete imageFallOff;
}

inline vec4 update(vec4 dotInfo, int f, float _timeRatio)
{
	float oldY = dotInfo.y;
	float gravity = baseGravity * smoothstep(2000.f, 1900.f, float(f));
	dotInfo.y += dotInfo.w * _timeRatio / 200.f;
	dotInfo.w -= gravity * _timeRatio;
	if (dotInfo.y < 0.)
	{
		float r = oldY / (oldY - dotInfo.y);
		dotInfo.y = -dotInfo.y * restitution;
		dotInfo.w = (-2.f * gravity * (1.f - r) - dotInfo.w) * restitution;		// This make the bounce almost perfect, this point that annoyed my eye a little in the original demo. But let say, it's an old demo from the integer time, much easier to fix today :)
	}
	return dotInfo;
}

void BouncingDots::Init(voxie_wind_t& _vw)
{
	vw = &_vw;
	// init images
	imageRT = new Image<int>(512,512);
	imageFallOff = new Image<int>(512, 512);
	for (int y = 0; y < imageFallOff->getSizeY(); y++)
	{
		for (int x = 0; x < imageFallOff->getSizeX(); x++)
		{
			float c = clamp(1.f - length(vec2(float(x - imageFallOff->getSizeX() / 2) / float(imageFallOff->getSizeX() / 2), float(y - imageFallOff->getSizeY() / 2) / float(imageFallOff->getSizeY() / 2))), 0.f, 1.f);
			c = sqrtf(c);
			c *= 0.20;
			imageFallOff->Write(x, y, intCol(c) | 0xff000000);
		}
	}
	// init dots
	dotInfo.reserve(nbPart);
	for (int i = 0; i < nbPart; i++)
	{
		dotInfo.push_back(vec4(vec3(4000.f), 0.f));
	}
}

void BouncingDots::Integrate(float _ratio, int _frame)
{
	for (int i = 0; i < nbPart; i++)
	{
		vec4 curPart = dotInfo[i];
		curPart = update(curPart, _frame, _ratio);
		dotInfo[i] = curPart;
	}
}

void BouncingDots::Tick(float _dt)
{
	float oldfframe = fframe;
	time += _dt;
	fframe += _dt * 60.f * speedScale;
	float nffl = floor(fframe);
	float nffr = fract(fframe);
	float offl = floor(oldfframe);
	float offr = fract(oldfframe);

	if (offl == nffl)	// same int frame?
	{
		Integrate(nffr - offr, int(offl));
	}
	else
	{
		Integrate( 1.f - offr, int(offl));
		for(int i = int(offl); i < int(nffl); i++)
		{
			dotInfo[i % nbPart] = spawnNewParticle(i);
			if (i == int(nffl) - 1)			// last span ?
			{
				Integrate(nffr, int(offl));
			}
			else
			{
				Integrate(1.f, int(offl));
			}
		}
	}
	frame = int(nffl);
}

void DrawGeo(voxie_frame_t* _vf, const mat4x4& _transform, const Geometry &geo, Image<int> &img,const int color = 0xffffff)
{
	vec3 *transformed = (vec3*)_malloca(geo.vertices.size() * sizeof(vec3));
	//for (const vec3& vertex : geo.vertices)
	for (int i = 0; i < geo.vertices.size(); i++)
	{
		transformed[i] = _transform * vec4(geo.vertices[i], 1.f);
	}
	for (const Line& line : geo.lines)
	{
		vec3& v0 = transformed[line.indices[0]];
		vec3& v1 = transformed[line.indices[1]];
		voxie_drawlin(_vf, v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, color);
	}
	vec2 size = vec2(img.getSizeX(), img.getSizeY()) / 2.f;
	for (const Line& line : geo.lines)
	{
		vec2 p0 = transformed[line.indices[0]].xy * size + size;
		vec2 p1 = transformed[line.indices[1]].xy * size + size;
		img.line(p0, p1, 0x0);
	}
	_freea(transformed);
}

static void setVert(poltex_t& v, const vec2& p, float height)
{
	v.x = p.x * 2.0f - 1.0f;
	v.y = p.y * 2.0f - 1.0f;
	v.z = height;
	v.u = p.x;
	v.v = p.y;
	v.col = 0xffffff;
}

void BouncingDots::Draw(voxie_frame_t* _vf)
{
	if (time > duration)
		finished = true;
	int col = intCol(smoothsteplin(0.0f, fadeIn, time) * smoothsteplin(duration, duration - fadeOut, time));

	imageRT->copy(*imageFallOff);

	float a1 = sin(fframe * 0.25f / 60.f) * radians(90.f);
	float a2 = ((fframe - 1900.f) * 1.f / 60.f);
	float theta = mix(a1, a2, clamp((fframe - 1900.f) / 100.f, 0.f, 1.f));
	mat4x4 mz = mat4x4(
		cos(theta), -sin(theta), 0.f, 0.f,
		sin(theta), cos(theta), 0.0, 0.f,
		0.0, 0.0, 1.0, 0.f,
		0.0, 0.0, 0.0, 1.f);
	float partSize = 0.04f;
	for (int i = 0; i < nbPart; i++)
	{
		vec3 p = dotInfo[i].xzy * 0.22f;
		if (p.x > 500.f)
			continue;
		p.z -= 0.35f - partSize;

		float lifeSpan;
		if (i > (frame % nbPart))
		{
			lifeSpan = float(i - (frame % nbPart)) / float(nbPart);
		}
		else
		{
			lifeSpan = float(512 + i - (frame % nbPart)) / float(nbPart);
		}
		float fadeGap = 0.05f;
		float sc = partSize * smoothstep(0.f, fadeGap, lifeSpan) * smoothstep(1.f, 1.f - fadeGap, lifeSpan) * smoothstep(vw->aspz, vw->aspz - partSize, p.z);

		mat4x4 local = fromEuler(vec3((fframe + float(i)) * 0.025f, (fframe + float(i)) * 0.037f, (fframe + float(i)) * 0.041f));

		mat4x4 transform = mz * mat4x4(	sc , 0.f, 0.f, 0.f,
									0.f, sc , 0.f, 0.f,
									0.f, 0.f, sc , 0.f,
									p.x, p.y, p.z, 1.f) * local;
		DrawGeo(_vf, transform, geometry, *imageRT, col);
	}

	tiletype tt = imageRT->GetTileType();

	const bool subdiv = false;
	const int  gridSize = 8;

	if (subdiv)
	{
		int nbVertPerRow = (gridSize - 1) * 4;
		int nbIdPerRow = (gridSize - 1) * 5;
		poltex_t* vertices = (poltex_t*)_malloca(nbVertPerRow * sizeof(poltex_t));
		int* indices = (int*)_malloca(nbIdPerRow * sizeof(int));
		for (int i = 0; i < (gridSize - 1); i++)
		{
			indices[i * 5 + 0] = i * 4 + 0;
			indices[i * 5 + 1] = i * 4 + 1;
			indices[i * 5 + 2] = i * 4 + 2;
			indices[i * 5 + 3] = i * 4 + 3;
			indices[i * 5 + 4] = -1;
		}

		for (int y = 0; y < gridSize - 1; y++)
		{
			for (int x = 0; x < gridSize - 1; x++)
			{
				vec2 h00 = vec2(float(x) / float(gridSize - 1)    , float(y) / float(gridSize - 1));
				vec2 h10 = vec2(float(x + 1) / float(gridSize - 1), float(y) / float(gridSize - 1));
				vec2 h01 = vec2(float(x) / float(gridSize - 1)    , float(y + 1) / float(gridSize - 1));
				vec2 h11 = vec2(float(x + 1) / float(gridSize - 1), float(y + 1) / float(gridSize - 1));
				setVert(vertices[x * 4 + 0], h00, -vw->aspz);
				setVert(vertices[x * 4 + 1], h10, -vw->aspz);
				setVert(vertices[x * 4 + 2], h11, -vw->aspz);
				setVert(vertices[x * 4 + 3], h01, -vw->aspz);
			}
			voxie_drawmeshtex(_vf, (char*)&tt, vertices, nbVertPerRow, indices, nbIdPerRow, 2 + 8, col);
		}
		_freea(indices);
		_freea(vertices);
	}
	else
	{
		poltex_t vertex[4];
		vec3 c = vec3(0.0f, 0.0f, -vw->aspz);
		float r = 1.f;
		vertex[0].x = c.x - r; vertex[0].y = c.y - r; vertex[0].z = c.z; vertex[0].u = 0.f; vertex[0].v = 0.f; vertex[0].col = 0xffffff;
		vertex[1].x = c.x - r; vertex[1].y = c.y + r; vertex[1].z = c.z; vertex[1].u = 0.f; vertex[1].v = 1.f; vertex[1].col = 0xffffff;
		vertex[2].x = c.x + r; vertex[2].y = c.y + r; vertex[2].z = c.z; vertex[2].u = 1.f; vertex[2].v = 1.f; vertex[2].col = 0xffffff;
		vertex[3].x = c.x + r; vertex[3].y = c.y - r; vertex[3].z = c.z; vertex[3].u = 1.f; vertex[3].v = 0.f; vertex[3].col = 0xffffff;
		int		 indices[5];
		for (int i = 0; i < 4; i++)
			indices[i] = i;
		indices[4] = -1;
		voxie_drawmeshtex(_vf, (char*)&tt, vertex, 4, indices, 5, 2 + 8, col);
	}
}