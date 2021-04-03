#pragma once

#include "./headers/Noise.h"

struct Line
{
	Line(int id1, int id2)
	{
		indices[0] = id1;
		indices[1] = id2;
	}
	int indices[2];
};

class Geometry
{
public:
	std::vector<vec3>	vertices;
	std::vector<Line>	lines;
	Geometry(const std::vector<vec3>& _vertices, const std::vector<Line>& _lines) : vertices(_vertices), lines(_lines)
	{
	}

	Geometry()
	{
	}

	void Draw(voxie_frame_t* _vf, const mat4x4& _transform, const int color = 0xffffff) const
	{
		std::vector<vec3> transformed;
		transformed.reserve(vertices.size());
		for (const vec3& vertex : vertices)
		{
			transformed.push_back(_transform * vec4(vertex, 1.f));
		}
		for (const Line& line : lines)
		{
			vec3& v0 = transformed[line.indices[0]];
			vec3& v1 = transformed[line.indices[1]];
			voxie_drawlin(_vf, v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, color);
		}

	}

	float Measure() const
	{
		float totalLen = 0.f;
		for (const Line& line : lines)
		{
			const vec3& v0 = vertices[line.indices[0]];
			const vec3& v1 = vertices[line.indices[1]];
			totalLen += distance(v0, v1);
		}
		return totalLen;
	}

	void GeneratePoints(std::vector<vec3> &points, int nbPoints, float radius = 0.f) const
	{
		float totalLen = Measure();
		// Init point along geometry
		points.reserve(nbPoints);
		float inc = totalLen / (float)nbPoints;
		float cursor = 0.f;
		int curSpan = 0;
		for (int i = 0; i < nbPoints; i++)
		{
			Line curLine = lines[curSpan];
			vec3 v0 = vertices[curLine.indices[0]];
			vec3 v1 = vertices[curLine.indices[1]];
			float curLineDist = distance(v0, v1);
			if (cursor > curLineDist && curSpan < lines.size() - 1)		// to avoid overflow due to precision issue
			{
				curSpan++;
				cursor -= curLineDist;
				curLine = lines[curSpan];
				v0 = vertices[curLine.indices[0]];
				v1 = vertices[curLine.indices[1]];
				curLineDist = distance(v0, v1);
			}
			vec3 centerPos = mix(v0, v1, cursor / curLineDist);
			vec3 d = v1 - v0;
			vec3 n;
			if (fabs(d.x) <= fabs(d.y) && fabs(d.x) <= fabs(d.z))
				n = vec3(1.0f, 0.0f, 0.0f);
			else if (fabs(d.y) <= fabs(d.x) && fabs(d.y) <= fabs(d.z))
				n = vec3(0.0f, 1.0f, 0.0f);
			else
				n = vec3(0.0f, 0.0f, 1.0f);
			vec3 vx = normalize(cross(d, n));
			vec3 vy = normalize(cross(vx, d));

			vec2 h = normalize(hash21(float(i)) - vec2(0.5f)) * radius;

			vec3 pointPos = centerPos + h.x * vx + h.y * vy;

			points.push_back(pointPos);
			cursor += inc;
		}
	}
};

// Greetings
extern Geometry Atari;
extern Geometry Canada;
extern Geometry Commodore;
extern Geometry CNCD;
extern Geometry Cocoon;
extern Geometry Deadliner;
extern Geometry EidosSherbooke;
extern Geometry Eko;
extern Geometry Fairlight;
extern Geometry Farbrausch;
extern Geometry FTA;
extern Geometry FutureCrew;
extern Geometry Hemoroids;
extern Geometry Kalisto;
extern Geometry Lemon;
extern Geometry Matilde;
extern Geometry Melon;
extern Geometry Nooon;
extern Geometry Orange;
extern Geometry Overlanders;
extern Geometry Oxygene;
extern Geometry Quebec;
extern Geometry Replicants;
extern Geometry Revision;
extern Geometry Sanity;
extern Geometry Scoopex;
extern Geometry Skarla;
extern Geometry Spaceballs;
extern Geometry TheUnion;
extern Geometry Teapot;
extern Geometry Vectrex;
extern Geometry VolkorX; 
extern Geometry Voxon;
extern Geometry Xmen;

extern Geometry Cube;
extern Geometry David;
extern Geometry Dodecahedron;
extern Geometry VolkorXIntro;
extern Geometry Icosahedron;
extern Geometry Octahedron;
extern Geometry Present;
extern Geometry Soccerball;
extern Geometry Tetrahedron;
extern Geometry World;
extern Geometry XShip;