#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"
#include "./headers/Noise.h"
#include "./headers/VoxUtils.h"

enum Nuke::Town
{
	// Europe
	Ankara,
	Berlin,
	Rome,
	Madrid,
	Moscow,
	Oslo,
	London,
	Paris,
	// North America
	Montreal,
	NewYork,
	LosAngeles,
	Seattle,
	Miami,
	Texas,
	Chicago,
	Hawaii,
	// South America
	Havana,
	RioDeJaneiro,
	Lima,
	// Middle east
	Jerusalem,
	Tehran,
	Ryad,
	Qatar,
	// Asia
	Tokyo,
	Fokoka,
	Pyongyang,
	Mumbai,
	Bejing,
	Singapore,
	// Oceania
	Adelaide,
	Aukland,
	Darwin,
	// Africa
	Pretoria,
	Kinshasa,
};


inline vec3 GpsTo3d(const vec2& gps)
{
	vec2 angles = vec2(radians(gps.y), radians(gps.x));
	vec3 horizon = vec3(cosf(angles.x), 0.0f, -sinf(angles.x));
	vec3 houdiniPos = horizon * cosf(angles.y) + vec3(0.0f, sinf(angles.y), 0.0f);
	return vec3(houdiniPos.x, -houdiniPos.z, houdiniPos.y);
}

void Nuke::Init(voxie_wind_t& _vw)
{
	vw = &_vw;
	// Generate points along the mesh
	pointsVelEarth.reserve(nbPointsEarth);
	geoShip.GeneratePoints(pointsPosShip, nbPointsShip, 0.001f / scale);
	pointsPosSpaceWarp.reserve(nbPointsShip);
	for(int i = 0; i < nbPointsShip; ++i)
	{
		vec3 p = normalize(hash31(float(i)) * 2.0f - 1.0f) * mix(1.5f, 2.0f,hash11(float(i * 3)));
		p.z = 0.0f;
		pointsPosSpaceWarp.push_back(p);
	}
	// init towns
	towns = {
		// Europe
		{39.92079f, 32.85404f},			// Ankara
		{52.51704f, 13.38886f},			// Berlin
		{41.89896f, 12.47308f},			// Rome
		{40.41670f, -3.70358f},			// Madrid
		{55.75222f, 37.61556f},			// Moscow
		{59.91273f, 10.74609f},			// Oslo
		{51.509865f, -0.118092},		// London
		{48.864716f, -2.349014},		// Paris
		// North America
		{45.505331312f, -73.55249779f},	// Montreal
		{40.730610f, -73.935242},		// NewYork
		{34.0499998f, -118.249999f},	// LosAngeles
		{47.608013f, -122.335167f},		// Seattle
		{25.785996856f, -80.221165782f}, // Miami
		{32.7766642f, -96.7969879},		// Dallas
		{41.85003f, -87.65005f},		// Chicago
		{21.3068321f, -157.7911635f},	// Hawaii

		// South America
		{23.13302f, - 82.38304f},		// Havana
		{-22.9032315871f, - 43.1729427749f}, // RioDeJaneiro
		{-12.04318 - 77.02824},			// Lima
		// Middle east
		{31.76904f, 35.21633f},			// Jerusalem
		{35.69439f, 51.42151f},			// Tehran
		{24.68773f, 46.72185f},			// Ryad
		{25.3271054f, 51.1966577f},		// Qatar
		// Asia
		{35.652832f, 139.839478f},		// Tokyo
		{33.6f, 130.41667f},			// Fokoka
		{39.03385f, 125.75432f},		// Pyongyang
		{19.07283f, 72.88261f},			// Mumbai
		{39.9075f, 116.39723f},			// Bejing
		{1.3146631f, 103.8454093f},		// Singapore
		// Oceania
		{-34.92866f, 138.59863f},		// Adelaide
		{-36.848461f, 174.763336f},		// Aukland
		{-12.4499982f, 130.83333f},		// Darwin
		// Africa
		{-25.74486, 28.18783},			// Pretoria
		{-4.32758, 15.31357},			// kinshasa	
	};
	missiles =
	{
		// North America
		{ 5.f, Montreal},
		{ 5.f, NewYork},
		{ 5.f, Texas},
		{ 6.f, Chicago},
		{ 7.f, LosAngeles},
		{ 7.f, Seattle},
		{ 7.f, Miami},
		{ 10.f, Hawaii},
		// South America
		{ 4.f, Lima},
		{ 4.f, Havana},
		{ 3.f, RioDeJaneiro},
		// Oceania
		{ 12.f, Adelaide},
		{ 12.f, Darwin},
		{ 12.f, Aukland},
		// Asia
		{ 18.f, Tokyo},
		{ 18.f, Fokoka},
		{ 18.f, Pyongyang},
		{ 21.f, Mumbai},
		{ 21.f, Bejing},
		{ 21.f, Singapore},
		// Middle east
		{ 25.f, Jerusalem},
		{ 25.f, Tehran},
		{ 25.f, Ryad},
		{ 25.f, Qatar},
		// Europe
		{ 27.f, Ankara},
		{ 27.f, Berlin},
		{ 27.f, Moscow},
		{ 27.f, Oslo},
		{ 29.f, London},
		{ 29.f, Madrid},
		{ 29.f, Rome},
		{ 29.f, Paris},
		// Africa
		{ 26.f, Pretoria},
		{ 26.f, Kinshasa},
	};
	// Load earth points
	std::ifstream file("data\\earth.txt");
	if (file.is_open())
	{
		int nbPoints = 0;
		file >> nbPoints;
		pointsPosEarth.reserve(nbPoints);
		for (int i = 0; i < nbPoints; i++)
		{
			vec3 p;
			file >> p.x;
			file >> p.y;
			file >> p.z;
			pointsPosEarth.push_back(p);
		}
	}
}

void Nuke::Tick(float _dt)
{
	time += _dt;
	deltaTime = _dt;
}

void Nuke::multiDrawPoints(int start, int end, voxie_frame_t* _vf, float ratio)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	float restitution = 0.8f;
	float zBound = vw->aspz;
	for (int i = start; i < end; i++)
	{
		if (hash11((float)i) < ratio)
		{
			// integration
			vec3 vel = pointsVelEarth[i];
			vel.z -= 9.8f * deltaTime;
			vec3 pos = pointsPosEarth[i] + vel * deltaTime;
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
			}
			else if (pos.z > zBound)
			{
				pos.z = zBound;
				vel *= vec3(restitution, restitution, -restitution);
			}
			// Store
			pointsVelEarth[i] = vel;
			pointsPosEarth[i] = pos;
			vt[curIndexInBatch].x = pos.x; vt[curIndexInBatch].y = pos.y; vt[curIndexInBatch].z = pos.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
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

void Nuke::DrawParticles(voxie_frame_t* _vf)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	for (int i = 0; i < particles.size(); i++)
	{
		float remainingLive = particles[i].t;
		remainingLive -= deltaTime;
		if (remainingLive < 0.0f)
		{
			std::swap(particles[i], particles.back());
			particles.pop_back();
			i--;
			continue;
		}
		particles[i].t = remainingLive;

		vec3 src = particles[i].p;

		float noiseScale = 6.0f;
		vec3 p1 = src * noiseScale;
		vec3 p2 = p1 + vec3(31.341f, -43.23f, 12.34f);		//random offset
		vec3 p3 = p2 + vec3(-231.341f, 124.23f, -54.34f);	//random offset

		p1 -= vec3(0.f, 0.f, time);
		p2 -= vec3(0.f, 0.f, time * 0.91f);
		p3 -= vec3(0.f, 0.f, time * 0.79f);

		vec3 d1 = SimplexPerlin3D_Deriv(p1).yzw;
		vec3 d2 = SimplexPerlin3D_Deriv(p2).yzw;
		vec3 d3 = SimplexPerlin3D_Deriv(p3).yzw;

		vec3 curlDir = vec3(d3.y - d2.z, d1.z - d3.x, d2.x - d1.y);

		vec3 dst = src + curlDir *particles[i].s * deltaTime;

		particles[i].p = dst;

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
}

vec3 casteljeau(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float ratio)
{
	vec3 s00 = mix(p0, p1, ratio);
	vec3 s01 = mix(p1, p2, ratio);
	vec3 s02 = mix(p2, p3, ratio);
	vec3 s10 = mix(s00, s01, ratio);
	vec3 s11 = mix(s01, s02, ratio);
	return mix(s10, s11, ratio);
}

vec3 Nuke::spawnTrail(voxie_frame_t* _vf, const vec3& P0, const vec3& V0, const vec3& P1, const vec3& V1, int step, float ratio)
{
	const float particlesPerSeconds = 500.f;
	float len = distance(P0, P1) / 3.f;
	vec3 M0 = P0 + V0 * len;
	vec3 M1 = P1 + V1 * len;
	vec3 cur = casteljeau(P0, M0, M1, P1, ratio);
	float life = pow(mexHat(ratio), 2.0f);
	for (int i = 0; i < int(particlesPerSeconds * deltaTime); ++i, ++partCounter)
	{
		particles.emplace_back(cur, mix(0.01f, 0.004f, hash11((float)partCounter)), life * mix(1.0f, 2.0f, hash11((float)partCounter * 3)));
	}
	return cur;
}

void multiDrawSimple(voxie_frame_t* _vf, const mat4x4 &tr, const std::vector<vec3> &points, float ratio)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	for (int i = 0; i < points.size(); i++)
	{
		if (hash11((float)i) < ratio)
		{
			vec3 dst = tr * vec4(points[i], 1);

			vt[curIndexInBatch].x = dst.x; vt[curIndexInBatch].y = dst.y; vt[curIndexInBatch].z = dst.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
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

void multiDrawSpaceWarp(voxie_frame_t* _vf, const mat4x4& tr, const std::vector<vec3>& points, const std::vector<vec3>& pointsSpaceWarp, float ratio)
{
	const int batchSize = 2048;
	poltex_t vt[batchSize];
	int curIndexInBatch = 0;
	float alpha = powf(ratio, 2.0f);
	for (int i = 0; i < points.size(); i++)
	{
		if (hash11((float)i) < ratio)
		{
			vec3 dst = tr * vec4(mix(pointsSpaceWarp[i], points[i], alpha), 1);

			vt[curIndexInBatch].x = dst.x; vt[curIndexInBatch].y = dst.y; vt[curIndexInBatch].z = dst.z; vt[curIndexInBatch].col = 0xffffff; vt[curIndexInBatch].u = 0;  vt[curIndexInBatch].v = 0;
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

void Nuke::Draw(voxie_frame_t* _vf)
{
	if (time > duration)
		finished = true;
	float col = smoothstep(0.0f, 0.5f, time) * smoothstep(duration, duration - 1.0f, time);

	float shipDisolve = smoothsteplin(2.0f, 3.0f, time) * smoothsteplin(explodeTime - 1.f, explodeTime - 2.0f, time);

	mat4x4 EarthTr = Translate(vec3(0.15f, 0.15f, 0.f)) * RotX(radians(15.0f)) * RotZ(time * speedRot + radians(120.0f)) * Scale(vec3(scale));

	mat4x4 ShipTr = Translate(vec3(sinf(time * 0.9f - 1.2f), sinf(time * 1.1f + 1.2f), sinf(time * 1.2f + 2.56f)) * 0.015f) * Translate(vec3(-0.4f, -0.4f, 0.f)) * RotZ(radians(-45.0f - 90.0f)) * fromEuler(vec3(0.f, sinf(time), sinf(time * 1.2f + 2.56f)) * 0.025f) * Scale(vec3(0.18f));

	const float timeToTarget = 4.0f;
	const float maxRadius = 0.08f;
	const float explosingDuration = 2.f;

	if (time < explodeTime)
	{
		vec3 canonPos[4] = { vec3(-0.893303, -0.333333, -0.871963), vec3(0.893303, -0.333333, -0.871963) , vec3(-0.559969, -0.333333, 0.871963) , vec3(0.559969, -0.333333, 0.871963) };
		for (int i = 0; i < missiles.size(); i++)
		{
			if (time < missiles[i].time || time > missiles[i].time + timeToTarget + explosingDuration)
				continue;
			vec3 cityPos = GpsTo3d(towns[missiles[i].end]);
			if (time < missiles[i].time + timeToTarget)
			{
				float ratio = min(1.0f, (time - missiles[i].time) / timeToTarget);
				vec3 p = spawnTrail(_vf, ShipTr * vec4(canonPos[i % 4], 1), ShipTr * vec4(vec3(0, -1, 0), 1.f), EarthTr * vec4(cityPos, 1), normalize(EarthTr * vec4(cityPos, 0)), 16, ratio);
				float radius = 0.01f;
				vec3 r(radius, 0, 0);
				vec3 d(0, radius, 0);
				vec3 f(0, 0, radius);
				voxie_drawspr(_vf, "data\\sphere.ply", (point3d*)&p, (point3d*)&r, (point3d*)&d, (point3d*)&f, 0xffffff);
			}
			// Draw Explosion
			float texp = clamp((time - (missiles[i].time + timeToTarget)) / explosingDuration, 0.0f, 1.0f);
			float radius = sin(texp * Pi) * maxRadius;
			if (radius > floatEps)
			{
				vec3 p = EarthTr * vec4(cityPos, 1.0f);
				vec3 r(radius, 0, 0);
				vec3 d(0, radius, 0);
				vec3 f(0, 0, radius);
				voxie_drawspr(_vf, "data\\sphere.ply", (point3d *) &p, (point3d*)&r, (point3d*)&d, (point3d*)&f, 0xffffff);
			}
		}
		multiDrawSimple(_vf, EarthTr, pointsPosEarth, col);
	}
	else
	{
		if(!explodeInit)
		{
			explodeInit = true;
			for (int i = 0; i < nbPointsEarth; i++)
			{
				vec3 p = EarthTr * vec4(pointsPosEarth[i], 1.0f);
				pointsPosEarth[i] = p;
				float speedPart = mix(1.0f, 2.0f, hash11(float(i)));
				p += hash31(float(i * 13)) * 0.1f;
				pointsVelEarth.push_back(normalize(p) * speedPart);
			}
		}
		multiDrawPoints(0, nbPointsEarth, _vf, col);
	}
	if (shipDisolve > 0.0f)
	{
		multiDrawSpaceWarp(_vf, ShipTr, pointsPosShip, pointsPosSpaceWarp, shipDisolve);
	}
	DrawParticles(_vf);
}
