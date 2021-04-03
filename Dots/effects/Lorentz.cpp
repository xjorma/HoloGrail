#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"
#include "./headers/VoxUtils.h"

void Lorentz::Init(voxie_wind_t& _vw)
{
}

void Lorentz::Tick(float _dt)
{
	time += _dt;
}

vec3 LorentzAttractor(const vec3& p)
{
	float sigma = 10.0f;
	float rho = 28.0f;
	float beta = 8.f / 3.f;
	float dt = 0.01f;
	return vec3(	sigma * (p.y - p.x),
					p.x * (rho - p.z),
					p.x * p.y - beta * p.z
	) * dt;
}

vec3 LorentzAttractorMod1(const vec3& p)
{
	float alpha = 0.1f;
	float beta = 4.f;
	float xi = 14.f;
	float delta = 0.08f;
	float dt = 0.005f;
	return vec3(	alpha * p.x + sqr(p.y) - sqr(p.z) + alpha * xi,
					p.x * (p.y - beta * p.z) + delta,
					p.z + p.x * (beta * p.y + p.z)
	) * dt;
}

vec3 ThomasAttractor(const vec3& p)
{
	float beta = 0.19f;
	return vec3(	beta * p.x + sinf(p.y),
					-beta * p.y + sinf(p.z),
					-beta * p.z + sinf(p.x)
	);

}

void Lorentz::Draw(voxie_frame_t* _vf)
{
	if (time > duration)
		finished = true;
	int col = intCol(smoothsteplin(0.0f, 0.5f, time) * smoothsteplin(duration, duration - 0.5f, time));

	vec3 p(1.f);
	for (int i = 0; i < 16000; i++)
	{
		vec3 p0 = p / 100.f;
		//p += LorentzAttractor(p);
		p += LorentzAttractorMod1(p);
		//p += ThomasAttractor(p) * dt;
		vec3 p1 = p / 100.f;
		voxie_drawlin(_vf, p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, col);
	}
}
