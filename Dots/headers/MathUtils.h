#pragma once

const float floatEps = 0.0001f;

inline float sqr(float v)
{
	return v * v;
}

template< class T > inline T smoothsteplin(const T& edge0, const T& edge1, float x)
{
	return clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
}

inline float sqDistance(const vec3& p0, const vec3& p1)
{
	vec3 v = p1 - p0;
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline float mexHat(float v)
{
	return (1.0f - sqr((v - 0.5f) * 2.0f)) * expf(sqr((v - 0.5f) * 2.0f));
}

// https://en.wikipedia.org/wiki/Slerp
inline vec3 slerp(const vec3& p0, const vec3& p1, float t)
{
	float omega = acosf(dot(p0, p1));
	return (sinf((1.0f - t) * omega) / sinf(omega)) * p0 + (sinf(t * omega) / sinf(omega)) * p1;
}

template<class T> T LowPassFilterInterpolation(float _DeltaTime, float _InterpTime, const T& _CurrentValue, const T& _TargetValue)
{
	float alpha = _DeltaTime / (_InterpTime / 6.0f + _DeltaTime);
	return mix(_CurrentValue, _TargetValue, alpha);
}

inline float fit(float value, float omin, float omax, float nmin, float nmax)
{
	float p = (value - omin) / (omax - omin);
	return (p * (nmax - nmin) + nmin);
}

inline point3d* vPtr(const vec3& v)
{
	return (point3d*)&v;
}

inline mat4x4 RotX(float ang)
{
	return mat4x4(
		1.f, 0.f, 0.f, 0.f,
		0.f, cosf(ang), -sinf(ang), 0.f,
		0.f, sinf(ang), cosf(ang), 0.f,
		0.f, 0.f, 0.f, 1.f);
}

inline mat4x4 RotY(float ang)
{
	return mat4x4(
		cosf(ang), 0.f, sinf(ang), 0.f,
		0.f, 1.f, 0.f, 0.f,
		-sinf(ang), 0.f, cosf(ang), 0.f,
		0.f, 0.f, 0.f, 1.f);
}

inline mat4x4 RotZ(float ang)
{
	return mat4x4(
		cosf(ang), -sinf(ang), 0.f, 0.f,
		sinf(ang), cosf(ang), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

inline mat4x4 Scale(vec3 s)
{
	return mat4x4(
		s.x, 0.f, 0.f, 0.f,
		0.f, s.y, 0.f, 0.f,
		0.f, 0.f, s.z, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

inline mat4x4 Translate(vec3 t)
{
	return mat4x4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		t.x, t.y, t.z, 1.f);
}

inline mat4x4 fromEuler(vec3 ang)
{
	mat4x4 mx = RotX(ang.x);
	mat4x4 my = RotY(ang.y);
	mat4x4 mz = RotZ(ang.z);
	return mx * my * mz;
}

// https://github.com/sole/tween.js/
inline float ElasticTweenInOut(float k)
{
	k *= 2.0f;

	if (k < 1.0f)
		return -0.5f * powf(2.0f, 10.0f * (k - 1.0f)) * sinf((k - 1.1f) * 5.0f * Pi);

	return 0.5f * powf(2.0f, -10.0f * (k - 1.0f)) * sinf((k - 1.1f) * 5.0f * Pi) + 1.0f;
}

inline float BackTweenInOut(float k)
{
	k *= 2;
	float s = 1.70158f * 1.525f;

	if (k < 1.0f)
		return 0.5f * (k * k * ((s + 1) * k - s));

	k -= 2.0f;
	return 0.5f * (k * k * ((s + 1) * k + s) + 2.0f);
}