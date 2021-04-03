#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"
#include "./headers/VoxUtils.h"
#include "./headers/Noise.h"

extern std::vector<vec3> Dance;

inline float Triangle(float x, float f)
{
	return (abs(((f * 0.5f) * x - 4.0f * floor(0.25f * f * x * 0.5f)) - 2.0f) - 1.0f) / f;
}

void Plexus::Init(voxie_wind_t& _vw)
{
	vw = &_vw;
	// init particles
	const int nbRandomParticles = 300;
	particles.reserve(nbRandomParticles);
	for(int i = 0; i < nbRandomParticles; i++)
	{
		particles.emplace_back(shash31(float(i)) * vec3(1.0f, 1.0f, vw->aspz), normalize(shash31(float(i + 314))));
	}
	// Load Animation (Lame loader :( )
	std::ifstream file("data\\Anim.txt");
	if(file.is_open())
	{
		file >> nbPointAnim;
		file >> nbFrame;
		anim.reserve(nbPointAnim * nbFrame);
		for (int i = 0; i < nbPointAnim * nbFrame; i++)
		{
			vec3 p;
			file >> p.x;
			file >> p.y;
			file >> p.z;
			anim.push_back(p);
		}
	}
	points.reserve(600);
}

void Plexus::Tick(float _dt)
{
	time += _dt;

	const float bpm = 145.0f;
	float period = 60.0f / bpm;

	float triTime = (Triangle(time / period, 4.0f) + 0.5f) * 2.0f;
	float curve = smoothstep(0.9f, 1.0f, triTime);

	pointRadius = 0.002;
	float distScale = mix(0.8f, 1.2f, curve);
	maxLineDist = 0.15f * distScale;
	maxTriangleDist = 0.12f * distScale;

	points.clear();

	// Dance Text!

	float noiseScale = mix(2.0f, 3.0f, curve);
	for (int i = 0; i < Dance.size(); i++)
	{
		const vec3& p = Dance[i];
		points.push_back(p + (vec3(snoise(vec4(p * noiseScale, time + 10.f)), snoise(vec4(p * noiseScale, time + 20.f)), snoise(vec4(p* noiseScale, time + 30.f)) - 0.5f) * 0.1f));
	}

	// Random Point
	const float restitution = 1.0f;
	float zBound = vw->aspz;
	float partSpeed = 0.15f;
	for(Part &p : particles)
	{
		p.pos = p.pos + p.vel * _dt * partSpeed;
		// Bounce X
		if (p.pos.x < -1.0f)
		{
			p.pos.x = -1.0f;
			p.vel *= vec3(-restitution, restitution, restitution);
		}
		else if (p.pos.x > 1.0f)
		{
			p.pos.x = 1.0f;
			p.vel *= vec3(-restitution, restitution, restitution);
		}
		// Bounce Y
		if (p.pos.y < -1.0f)
		{
			p.pos.y = -1.0f;
			p.vel *= vec3(restitution, -restitution, restitution);
		}
		else if (p.pos.y > 1.0f)
		{
			p.pos.y = 1.0f;
			p.vel *= vec3(restitution, -restitution, restitution);
		}
		// Bounce Z
		if (p.pos.z < -zBound)
		{
			p.pos.z = -zBound;
			p.vel *= vec3(restitution, restitution, -restitution);
		}
		else if (p.pos.z > zBound)
		{
			p.pos.z = zBound;
			p.vel *= vec3(restitution, restitution, -restitution);
		}
		points.push_back(p.pos);
	}

	// Dancer
	const float fps = 30.0f;
	int curFrame = int(floor(time * fps)) % nbFrame;
	int nextFrame = (curFrame + 1) % nbFrame;
	float alpha = fract(time * fps);
	for (int i = 0; i < nbPointAnim; i++)
	{
		vec3 curP = anim[nbPointAnim * curFrame + i];
		vec3 nextP = anim[nbPointAnim * nextFrame + i];
		vec3 inter = mix(curP, nextP, alpha) * 0.4f;
		points.push_back(inter - vec3(0.0f, 0.0f, vw->aspz));
	}

}

static inline void addLine(std::vector<int>& lines, int i0, int i1, std::vector<vec3>& points, float maxDist)
{
	if (sqDistance(points[i0], points[i1]) < maxDist)
	{
		// sort
		if (i0 > i1)
		{
			std::swap(i0, i1);
		}
		lines.push_back((i0 << 16) | i1);
	}
}

static inline void addTriangle(std::vector<int>& lines, int i0, int i1, int i2,std::vector<vec3>& points, float maxDist)
{
	float dist = max(max(sqDistance(points[i0], points[i1]), sqDistance(points[i1], points[i2])), sqDistance(points[i2], points[i0]));
	if (dist < maxDist)
	{
		// sort
		if (i0 > i1)
			std::swap(i0, i1);
		if (i1 > i2)
			std::swap(i1, i2);
		if (i0 > i1)
			std::swap(i0, i1);

		lines.push_back((i0 << 20) | (i1 << 10) | i2);
	}
}

static inline void createPoint(poltex_t &pt, vec3 p, int col)
{
	pt.x = p.x;
	pt.y = p.y;
	pt.z = p.z;
	pt.col = col;
}

void Plexus::Draw(voxie_frame_t* _vf)
{
	if (time > duration)
		finished = true;
	float fade = smoothsteplin(0.0f, fadeTime, time) * smoothsteplin(duration, duration - fadeTime, time);

	int nbPoint = (int)points.size();

	// Create a triangulation
	FADE3D::Fade_3D dt;
	for (const vec3 &p : points)
	{
		FADE3D::Point3 fp(p.x, p.y, p.z);
		dt.insert(fp);
	}

	// Draw a file for the Geomview viewer
	//dt.show("example0.list");

	std::vector<FADE3D::Tet3*> vTetrahedra;
	std::vector<FADE3D::Point3*> vVertices;

	dt.getTetrahedra(vTetrahedra);
	dt.getVertices(vVertices);

	std::unordered_map<const FADE3D::Point3*, int> hashMap;
	std::vector<vec3> pointConv;
	hashMap.reserve(nbPoint);
	pointConv.reserve(nbPoint);
	for (int i = 0; i < nbPoint; i++)
	{
		hashMap.insert({ vVertices[i] , i});
		pointConv.emplace_back((float)vVertices[i]->x(), (float)vVertices[i]->y(), (float)vVertices[i]->z());
	}

	std::vector<int> lines;
	lines.reserve(nbPoint * 3);				// Guesstimate
	std::vector<int> triangles;
	triangles.reserve(nbPoint * 2);			// Guesstimate

	float maxLineDistSq = sqr(maxLineDist);
	float maxTriangleDistSq = sqr(maxTriangleDist);

	for (const FADE3D::Tet3* tet : vTetrahedra)
	{
		FADE3D::Point3 *p0 = nullptr, *p1 = nullptr, *p2 = nullptr, *p3 = nullptr;
		tet->getCorners(p0, p1, p2, p3);
		int i0 = hashMap[p0];
		int i1 = hashMap[p1];
		int i2 = hashMap[p2];
		int i3 = hashMap[p3];
		// Lines
		addLine(lines, i0, i1, pointConv, maxLineDistSq);
		addLine(lines, i0, i2, pointConv, maxLineDistSq);
		addLine(lines, i0, i3, pointConv, maxLineDistSq);
		addLine(lines, i1, i2, pointConv, maxLineDistSq);
		addLine(lines, i1, i3, pointConv, maxLineDistSq);
		addLine(lines, i2, i3, pointConv, maxLineDistSq);
		// triangles
		addTriangle(triangles, i0, i1, i2, pointConv, maxTriangleDistSq);
		addTriangle(triangles, i0, i2, i3, pointConv, maxTriangleDistSq);
		addTriangle(triangles, i0, i3, i1, pointConv, maxTriangleDistSq);
		addTriangle(triangles, i1, i2, i3, pointConv, maxTriangleDistSq);
	}

	// Point Renderer
	for (const vec3 &p : points)
	{
		voxie_drawsph(_vf, p.x, p.y, p.z, pointRadius, 0, intCol(fade));
	}

	// Line Renderer
	std::sort(lines.begin(), lines.end());
	int prevLine = -1;
	for (int line : lines)
	{
		if (line != prevLine)
		{
			int i0 = line >> 16;
			int i1 = line & 65535;
			const vec3& p0 = pointConv[i0];
			const vec3& p1 = pointConv[i1];
			float d = distance(p0, p1);
			float c = smoothstep(maxLineDist, maxLineDist * 0.8f, d);
			voxie_drawlin(_vf, p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, intCol(c * fade));
			prevLine = line;
		}
	}

	// Triangle Renderer
	std::sort(triangles.begin(), triangles.end());
	int prevTriangle = -1;
	int indices[4];
	indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = -1;
	for (int triangle : triangles)
	{
		if (triangle != prevTriangle)
		{
			poltex_t vertices[3];
			int i0 = triangle >> 20;
			int i1 = (triangle >> 10) & 1023;
			int i2 = triangle & 1023;
			const vec3& p0 = pointConv[i0];
			const vec3& p1 = pointConv[i1];
			const vec3& p2 = pointConv[i2];
			float d = max(max(distance(p0, p1), distance(p1, p2)), distance(p2, p0));
			float c = smoothstep(maxTriangleDist, maxTriangleDist * 0.8f, d);
			createPoint(vertices[0], pointConv[i0], 0xffffff);
			createPoint(vertices[1], pointConv[i1], 0xffffff);
			createPoint(vertices[2], pointConv[i2], 0xffffff);
			voxie_drawmeshtex(_vf, nullptr, vertices, 3, indices, 4, 2, intCol(c * fade));
			prevTriangle = triangle;
		}
	}
}
