#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"

void HyperGeometry::Init(voxie_wind_t& _vw)
{
	geometry3d.lines = geometry4d.lines;
	geometry3d.vertices.reserve(geometry4d.vertices.size());
}

void HyperGeometry::Tick(float _dt)
{
	float ang1 = time * speed;
	float ang2 = time * speed;
	mat4x4 mat(cos(ang1), -sin(ang1), 0.0, 0.0,
		sin(ang1), cos(ang1), 0.0, 0.0,
		0.0, 0.0, cos(ang2), -sin(ang2),
		0.0, 0.0, sin(ang2), cos(ang2)
	);
	geometry3d.vertices.clear();
	for (const vec4& vertex : geometry4d.vertices)
	{
		vec4 pos = mat * vec4(vertex.xyz, vertex.w * 0.25f);
		float lheight = 1.129f;
		float a = lheight / (pos.w - lheight);
		geometry3d.vertices.push_back(pos.xyz * a);
	}
	time += _dt;

}

void HyperGeometry::Draw(voxie_frame_t* _vf)
{
	mat4x4 transform = RotX(radians(90.f)) * mat4x4(scale);
	geometry3d.Draw(_vf, transform);
}
