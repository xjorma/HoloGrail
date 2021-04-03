#pragma once

#include "./headers/geometries4d.h"
#include "./headers/HSpan.h"

template <class T> class Image;

class Effect
{
protected:
	bool	finished;
public:
	Effect() : finished(false)
	{
	}
	virtual ~Effect()
	{
	}
	virtual void Init(voxie_wind_t &_vw) = 0;
	virtual void Tick(float _dt) = 0;
	virtual void Draw(voxie_frame_t* _vf) = 0;
	inline bool isFinished() const 
	{
		return finished;
	}
};


class BouncingDots: public Effect
{
private:
	std::vector<vec4> dotInfo;
	int frame;
	float fframe;
	float time;
	float fadeIn;
	float duration;
	float fadeOut;
	float speedScale;
	const Geometry &geometry;
	voxie_wind_t *vw;
	Image<int> *imageRT;
	Image<int>* imageFallOff;
	void Integrate(float _dt, int _frame);
public:
	BouncingDots(float _fadeIn, float _duration, float _fadeOut, float _speedScale,const Geometry &_geometry) : fadeIn(_fadeIn), duration(_duration), fadeOut(_fadeOut), speedScale(_speedScale),geometry(_geometry), frame(0), fframe(0.f), time(0.f), imageRT(nullptr), imageFallOff(nullptr), vw(nullptr)
	{
	}
	~BouncingDots();
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class CollapsingTiles : public Effect
{
private:
	float	time;
	float	duration;
	float	fadeTime;
	float	speed;
	int		nbTiles;
public:
	CollapsingTiles(int _nbTiles, float _speed, float _duration, float _fadeTime) : time(0.f), nbTiles(_nbTiles), speed(_speed), duration(_duration), fadeTime(_fadeTime)
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class HyperGeometry : public Effect
{
private:
	float	time;
	float	speed;
	float	scale;
	Geometry& geometry3d;
	const	Geometry4d& geometry4d;
public:
	HyperGeometry(float _speed, float _scale, const Geometry4d& _geometry4d) : time(0.f), speed(_speed), scale(_scale), geometry4d(_geometry4d), geometry3d(*(new Geometry()))
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class Nuke : public Effect
{
private:
	enum Town;
	struct Particle
	{
		vec3 p;
		float s;
		float t;
		Particle(const vec3& _p, float _s, float _t) : p(_p), s(_s), t(_t)
		{
		}
	};
	struct Missile
	{
		float	time;
		Town	end;
	};
	float	time;
	float	deltaTime;
	float	speedRot;
	float	scale;
	int		nbPointsEarth;
	int		nbPointsShip;
	int		partCounter;
	float	explodeTime;
	float   duration;
	bool	explodeInit;
	const	Geometry& geoWorld;
	const	Geometry& geoShip;
	voxie_wind_t* vw;
	std::vector<vec3> pointsPosShip;
	std::vector<vec3> pointsPosSpaceWarp;
	std::vector<vec3> pointsPosEarth;
	std::vector<vec3> pointsVelEarth;
	std::vector<vec2> towns;
	std::vector<Missile> missiles;
	std::vector<Particle> particles;
	std::mutex drawMutex;
	vec3	spawnTrail(voxie_frame_t* _vf, const vec3& P0, const vec3& V0, const vec3& P1, const vec3& V1, int step, float ratio);
	void	multiDrawPoints(int start, int end, voxie_frame_t* _vf, float ratio);
	void	DrawParticles(voxie_frame_t* _vf);
public:
	Nuke(float _speedRot, float _scale, float _explodeTime, float _duration) : time(0.f), speedRot(_speedRot), scale(_scale), geoWorld(World), geoShip(XShip), nbPointsEarth(64000), nbPointsShip(32000), explodeTime(_explodeTime), duration(_duration), explodeInit(false), partCounter(0)
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class Dissolve : public Effect
{
private:
	int		nbPoints;
	float	scale;
	float	angle;
	float	time;
	float	dTime;
	float	fadeInTime;
	float	logoTime;
	float	dissolveTime;
	float	fadeOutTime;
	const	Geometry &geo;
	std::vector<vec3> points;
	int		nbThreads;
	std::mutex drawMutex;
	void multiDraw(int start, int end, voxie_frame_t* _vf);
	void multiDrawSimple(int start, int end, voxie_frame_t* _vf, float ratio);
public:
	Dissolve(float _fadeInTime, float _logoTime, float _dissolveTime, float _fadeOutTime,int _nbPoint, float _scale, float _angle, const Geometry &_geo) : fadeInTime(_fadeInTime), logoTime(_logoTime), dissolveTime(_dissolveTime), fadeOutTime(_fadeOutTime), nbPoints(_nbPoint), scale(_scale), angle(_angle), geo(_geo), time(0.f), dTime(0.f), nbThreads(1)
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class Water : public Effect
{
private:
	float time;
	float timePerSpan;
	float timeGrowSpan;
	float timeFade;
	float globalIntensity;
	int size;
	Image<float> *simulBuffer[2];
	Image<int>* heightMap;
	voxie_wind_t *vw;
	int	nbThreads;
	const std::vector<HSpan> spans;
	void Simulate(int start, int end);
	void DrawSpot(vec2 c, int r, float i);
	void Temper();
public:
	Water(int _size, const std::vector<HSpan> _spans, float _timePerSpan, float _timeGrowSpan, float _timeFade);
	~Water();
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};


class EndCredit : public Effect
{
private:
	struct textItem
	{
		char c;
		vec2 p;
	};
	struct Part
	{
		vec3 pos;
		vec3 vel;
		int  bounceLeft;
		Part(const vec3& _pos, const vec3& _vel) : pos(_pos), vel(_vel), bounceLeft(10) {}
	};
	float time;
	float dt;
	float pageDuration;
	voxie_wind_t* vw;
	std::vector<std::vector<vec3>> bakedFont;
	mat4 transform;
	int oldPage;
	std::vector<vec3>	textPoint;
	std::vector<Part>	particles;
	std::vector<std::vector<textItem>> pagesTi;
	std::vector<std::string> pages;
	int multiDrawChar(voxie_frame_t* _vf, char c, const vec3& pos, int curId, std::function<vec3(const vec3&, const vec3&, int)> f);
	void DrawText(voxie_frame_t* _vf);
	void spawnParticules();
	void drawParticles(voxie_frame_t* _vf);
public:
	EndCredit(std::vector<std::string> _pages, float _pageDuration) : pages(_pages), pageDuration(_pageDuration), time(0.f), oldPage(0)
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class Greeting : public Effect
{
public:
	struct GeoScale
	{
		const Geometry *geo;
		float scale;
	};
private:
	float time;
	float freq;
	float scale;
	int	nbPoints;
	std::vector<GeoScale> geos;
	std::vector<std::vector<vec3>> pointsVec;
	void multiDraw(int start, int end, voxie_frame_t* _vf, float ratio);
public:
	Greeting(const std::vector<GeoScale> _geos, int _nbPoints, float _scale, float _freq) : geos(_geos), nbPoints(_nbPoints), scale(_scale), freq(_freq), time(0.f)
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class Wave : public Effect
{
private:
	float time;
	float duration;
	float fadeTime;
	float phase;
	float height;
	int	gridSize;
	float angleZ;
	float maxv;
	float exp;
	float angle;
	vec3 deformPos;
	Image<vec3>* buffer;
	vec3 Read(int x, int y) const;
	vec3 Compute(vec2 v, const mat4& deform, const mat4& ideform) const;
public:
	Wave(int _gridSize, float _height, float _duration, float _fadeTime, float _phase);
	~Wave();
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class Plexus : public Effect
{
	struct Part
	{
		vec3 pos;
		vec3 vel;
		Part(const vec3 &_pos, const vec3 &_vel) : pos(_pos), vel(_vel) {}
	};
private:
	float time;
	float fadeTime;
	float duration;
	float pointRadius;
	float maxLineDist;
	float maxTriangleDist;
	int	nbPointAnim;
	int	nbFrame;
	std::vector<vec3> anim;
	std::vector<vec3> points;
	std::vector<Part> particles;
	voxie_wind_t* vw;
public:
	Plexus(float _duration, float _fadeTime) : duration(_duration), fadeTime(_fadeTime), time(0.f), vw(nullptr)
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};

class Lorentz : public Effect
{
private:
	float time;
	float duration;
public:
	Lorentz(float _duration) : duration(_duration), time(0.f)
	{
	}
	void Init(voxie_wind_t& _vw) override;
	virtual void Tick(float _dt) override;
	virtual void Draw(voxie_frame_t* _vf) override;
};