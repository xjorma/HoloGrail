#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <random>

// https://github.com/osresearch/vst/blob/master/teensyv/asteroids_font.c
#define P(x, y) vec2((x - 4.0f) / 10.0f, (y - 6.0f) / 10.0f)
#define FONT_UP vec2(-1, -1)
std::vector<std::vector<vec2>> font =
{
	{ P(4,0), P(3,2), P(5,2), P(4,0), FONT_UP, P(4,4), P(4,12)}, // !
	{ P(2,10), P(2,6), FONT_UP, P(6,10), P(6,6)}, // "
	{ P(0,4), P(8,4), P(6,2), P(6,10), P(8,8), P(0,8), P(2,10), P(2,2) }, // #
	{ P(6,2), P(2,6), P(6,10), FONT_UP, P(4,12), P(4,0)}, // $
	{ P(0,0), P(8,12), FONT_UP, P(2,10), P(2,8), FONT_UP, P(6,4), P(6,2) }, // %
	{ P(8,0), P(4,12), P(8,8), P(0,4), P(4,0), P(8,4) }, // &
	{ P(2,0), P(4,2) }, // '
	{ P(6,0), P(2,4), P(2,8), P(6,12)}, // (
	{ P(2,0), P(6,4), P(6,8), P(2,12)}, // )
	{ P(0,0), P(4,12), P(8,0), P(0,8), P(8,8), P(0,0)}, // *
	{ P(1,6), P(7,6), FONT_UP, P(4,9), P(4,3) }, // +
	{ P(2,0), P(4,2)}, // ,
	{ P(2,6), P(6,6)}, // -
	{ P(3,0), P(4,0)}, // .
	{ P(0,0), P(8,12)}, // /
	{ P(0,0), P(8,0), P(8,12), P(0,12), P(0,0), P(8,12) }, // 0
	{ P(4,0), P(4,12), P(3,10) }, // 1
	{ P(0,12), P(8,12), P(8,7), P(0,5), P(0,0), P(8,0) }, // 2
	{ P(0,12), P(8,12), P(8,0), P(0,0), FONT_UP, P(0,6), P(8,6) }, // 3
	{ P(0,12), P(0,6), P(8,6), FONT_UP, P(8,12), P(8,0) }, // 4
	{ P(0,0), P(8,0), P(8,6), P(0,7), P(0,12), P(8,12) }, // 5
	{ P(0,12), P(0,0), P(8,0), P(8,5), P(0,7) }, // 6
	{ P(0,12), P(8,12), P(8,6), P(4,0) }, // 7
	{ P(0,0), P(8,0), P(8,12), P(0,12), P(0,0), FONT_UP, P(0,6), P(8,6) }, //8
	{ P(8,0), P(8,12), P(0,12), P(0,7), P(8,5) }, // 9
	{ P(4,9), P(4,7), FONT_UP, P(4,5), P(4,3)}, // :
	{ P(4,9), P(4,7), FONT_UP, P(4,5), P(1,2)}, // ;
	{ P(6,0), P(2,6), P(6,12) }, // <
	{ P(1,4), P(7,4), FONT_UP, P(1,8), P(7,8)}, // =
	{ P(2,0), P(6,6), P(2,12) }, // >
	{ P(0,8), P(4,12), P(8,8), P(4,4), FONT_UP, P(4,1), P(4,0)}, // ?
	{ P(8,4), P(4,0), P(0,4), P(0,8), P(4,12), P(8,8), P(4,4), P(3,6) }, // @
	{ P(0,0), P(0,8), P(4,12), P(8,8), P(8,0), FONT_UP, P(0,4), P(8,4) }, // A
	{ P(0,0), P(0,12), P(4,12), P(8,10), P(4,6), P(8,2), P(4,0), P(0,0) },
	{ P(8,0), P(0,0), P(0,12), P(8,12)},
	{ P(0,0), P(0,12), P(4,12), P(8,8), P(8,4), P(4,0), P(0,0)},
	{ P(8,0), P(0,0), P(0,12), P(8,12), FONT_UP, P(0,6), P(6,6)},
	{ P(0,0), P(0,12), P(8,12), FONT_UP, P(0,6), P(6,6)},
	{ P(6,6), P(8,4), P(8,0), P(0,0), P(0,12), P(8,12)},
	{ P(0,0), P(0,12), FONT_UP, P(0,6), P(8,6), FONT_UP, P(8,12), P(8,0) },
	{ P(0,0), P(8,0), FONT_UP, P(4,0), P(4,12), FONT_UP, P(0,12), P(8,12) },
	{ P(0,4), P(4,0), P(8,0), P(8,12)},
	{ P(0,0), P(0,12), FONT_UP, P(8,12), P(0,6), P(6,0)},
	{ P(8,0), P(0,0), P(0,12)},
	{ P(0,0), P(0,12), P(4,8), P(8,12), P(8,0)},
	{ P(0,0), P(0,12), P(8,0), P(8,12)},
	{ P(0,0), P(0,12), P(8,12), P(8,0), P(0,0)},
	{ P(0,0), P(0,12), P(8,12), P(8,6), P(0,5)},
	{ P(0,0), P(0,12), P(8,12), P(8,4), P(0,0), FONT_UP, P(4,4), P(8,0) },
	{ P(0,0), P(0,12), P(8,12), P(8,6), P(0,5), FONT_UP, P(4,5), P(8,0) },
	{ P(0,2), P(2,0), P(8,0), P(8,5), P(0,7), P(0,12), P(6,12), P(8,10) },
	{ P(0,12), P(8,12), FONT_UP, P(4,12), P(4,0)},
	{ P(0,12), P(0,2), P(4,0), P(8,2), P(8,12)},
	{ P(0,12), P(4,0), P(8,12)},
	{ P(0,12), P(2,0), P(4,4), P(6,0), P(8,12)},
	{ P(0,0), P(8,12), FONT_UP, P(0,12), P(8,0)},
	{ P(0,12), P(4,6), P(8,12), FONT_UP, P(4,6), P(4,0)},
	{ P(0,12), P(8,12), P(0,0), P(8,0), FONT_UP, P(2,6), P(6,6)}, // Z
	{ P(6,0), P(2,0), P(2,12), P(6,12)}, // [
	{ P(2,6), P(6,10)}, // '\'
	{ P(2,0), P(6,0), P(6,12), P(2,12)},  // ]
	{ P(2,6), P(4,12), P(6,6)}, // ^
	{ P(0,0), P(8,0)}, // _
};

const float fontSizeX = 0.06f;
const float fontSizeY = fontSizeX * 12.0f / 8.0f;

int EndCredit::multiDrawChar(voxie_frame_t* _vf, char c, const vec3& pos, int curId, std::function<vec3(const vec3&,const vec3&, int)> f)
{
	if (c >= 0x61 && c <= 0x7a)		// strupr;
		c -= 0x20;
	c -= 0x21;
	if (c < 0 || c >= bakedFont.size())
		return curId;
	const std::vector<vec3>& points = bakedFont[c];
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	for (int i = 0; i < points.size(); i++)
	{
		vec3 p = points[i] + pos;

		p = f(p, pos, curId + i);

		vec3 dst = transform * vec4(p, 1.0f);

		textPoint.push_back(dst);

		vt[curIndexInBatch].x = dst.x; vt[curIndexInBatch].y = dst.y; vt[curIndexInBatch].z = dst.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
		curIndexInBatch++;
		if (curIndexInBatch == batchSize)
		{
			voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
			curIndexInBatch = 0;
		}
	}
	if (curIndexInBatch)
	{
		voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
	}
	return curId + int(points.size());
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

void EndCredit::spawnParticules()
{
	for (int i = 0; i < textPoint.size(); i++)
	{
		vec3 p = textPoint[i];
		float s = mix(1.0f, 2.0f, hash11(float(i)));
		p += hash31(float(i * 13)) * 0.1f;
		particles.emplace_back(p, normalize(p) * s);
	}
}

void EndCredit::DrawText(voxie_frame_t* _vf)
{
	int page = int(floor(time / pageDuration));
	if (page != oldPage)
	{
		spawnParticules();
		oldPage = page;
	}
	textPoint.clear();
	if (page >= pagesTi.size())
	{
		return;
	}
	// Draw Text
	std::function<vec3(const vec3&, const vec3&, int)> f;
	float fractTime = fract(time / pageDuration) * pageDuration;
	voxie_wind_t* curVw = vw;
	f = [fractTime, curVw](const vec3& v1, const vec3& v2, int curId)
	{
		float mtime = fractTime * 1.f - curId * 0.00013f;
		float vtime = curId * 0.0001f;
		vec3  vortexPos = vec3(sin(vtime), sin(vtime * 1.1 + 0.5), sin(vtime * 0.9 + 1.6) * curVw->aspz);
		if (mtime > 0)
			return mix(vortexPos, v1, clamp(mtime, 0.0f, 1.0f));
		else
			return vec3(-1, -1, -1);
	};

	int curId = 0;
	for (const textItem &ti: pagesTi[page])
	{
		curId = multiDrawChar(_vf, ti.c, vec3(ti.p.x, 0.0f, ti.p.y), curId, f);
	}
}

void EndCredit::Init(voxie_wind_t& _vw)
{
	vw = &_vw;

	// Conv Text
	for (const std::string &page: pages)
	{
		std::vector<textItem> tiv;
		std::vector<std::string> lines = split(page, '\n');
		float baseY = (lines.size() - 1.0f) * fontSizeY / 2.f;
		for (int y = 0; y < lines.size(); y++)
		{
			float py = baseY - float(y) * fontSizeY;
			float baseX = -((lines[y].size() - 1.0f) * fontSizeX / 2.f);
			for (int x = 0; x < lines[y].size(); x++)
			{
				if (lines[y][x] != ' ')
				{
					float px = baseX + float(x) * fontSizeX;
					tiv.push_back({ lines[y][x], vec2(px, py) });
				}
			}
		}
		pagesTi.push_back(tiv);
	}
	// Create Matrix
	transform = RotX(radians(30.0f));
	// Bake Font
	const float size = fontSizeX * 0.80f;
	bakedFont.reserve(font.size());
	for (const std::vector<vec2>lines : font)
	{
		std::vector<Line>	indices;
		std::vector<vec3>	points;
		vec2 pp = lines[0];
		for (int i = 1; i < lines.size(); i++)
		{
			vec2 np = lines[i];
			if (np.x == -1)
			{
				pp = lines[i + 1];
				np = lines[i + 2];
				i += 2;
			}
			indices.push_back(Line(int(points.size()), int(points.size()) + 1));
			points.push_back(vec3(pp.x, 0.0f, pp.y) * size);
			points.push_back(vec3(np.x, 0.0f, np.y) * size);
			pp = np;
		}
		Geometry geo = Geometry(points, indices);
		float len = geo.Measure();
		std::vector<vec3> genPoints;
		geo.GeneratePoints(genPoints, int(len * 3000), 0.001);
#if 0		// shuffle
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(genPoints.begin(), genPoints.end(), g);
#endif
		bakedFont.push_back(genPoints);
	}
}

void EndCredit::Tick(float _dt)
{
	time += _dt;
	dt = _dt;
}

void EndCredit::Draw(voxie_frame_t* _vf)
{
	vec3 upLeft(-1.f, 0.0f, vw->aspz);
	DrawText(_vf);
	drawParticles(_vf);
}


void EndCredit::drawParticles(voxie_frame_t* _vf)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	float restitution = 0.8f;
	float zBound = vw->aspz;
	for (int i = 0; i < particles.size(); i++)
	{
		// integration
		vec3 vel = particles[i].vel;
		vel.z -= 9.8f * dt;
		vec3 pos = particles[i].pos + vel * dt;
		int bounceLeft = particles[i].bounceLeft;
		// Bounce X
		if (pos.x < -1.0f)
		{
			pos.x = -1.0f;
			vel *= vec3(-restitution, restitution, restitution);
		}
		else if (pos.x > 1.0f)
		{
			pos.x = 1.0f;
			vel *= vec3(-restitution, restitution, restitution);
		}
		// Bounce Y
		if (pos.y < -1.0f)
		{
			pos.y = -1.0f;
			vel *= vec3(restitution, -restitution, restitution);
		}
		else if (pos.y > 1.0f)
		{
			pos.y = 1.0f;
			vel *= vec3(restitution, -restitution, restitution);
		}
		// Bounce Z
		if (pos.z < -zBound)
		{
			pos.z = -zBound;
			vel *= vec3(restitution, restitution, -restitution);
			bounceLeft--;
		}
		else if (pos.z > zBound)
		{
			pos.z = zBound;
			vel *= vec3(restitution, restitution, -restitution);
		}
		// Kill
		if (!bounceLeft)
		{
			std::swap(particles[i], particles.back());
			particles.pop_back();
			i--;
			continue;
		}

		// Store
		particles[i].vel = vel;
		particles[i].pos = pos;
		particles[i].bounceLeft = bounceLeft;
		vt[curIndexInBatch].x = pos.x; vt[curIndexInBatch].y = pos.y; vt[curIndexInBatch].z = pos.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
		curIndexInBatch++;
		if (curIndexInBatch == batchSize)
		{
			voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
			curIndexInBatch = 0;
		}
	}
	if (curIndexInBatch)
	{
		voxie_drawmeshtex(_vf, nullptr, vt, curIndexInBatch, nullptr, 0, 0, 0xffffff);
	}
}
