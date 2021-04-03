#pragma once

#include "./headers/HSpan.h"

template <class T> class Image
{
private:
	T* data;
	int sizeX;
	int sizeY;
public:
	Image(int _sizeX, int _sizeY) : sizeX(_sizeX), sizeY(_sizeY)
	{
		data = new T[sizeX * sizeY];
	}
	~Image()
	{
		delete[] data;
	}
	inline int getSizeX() const
	{
		return sizeX;
	}
	inline int getSizeY() const
	{
		return sizeY;
	}
	T Read(int x, int y) const
	{
		return data[x + y * sizeX];
	}
	void Write(int x, int y, T v)
	{
		data[x + y * sizeX] = v;
	}
	T ReadClip(int x, int y) const
	{
		x = clamp(x, 0, sizeX - 1);
		y = clamp(y, 0, sizeY - 1);
		return data[x + y * sizeX];
	}
	void WriteClip(int x, int y, T v)
	{
		if ((unsigned int)x >= (unsigned int)sizeX || (unsigned int)y >= (unsigned int)sizeY)
			return;
		data[x + y * sizeX] = v;
	}
	void Fill(T v)
	{
		for (int i = 0; i < sizeX * sizeY; i++)
		{
			data[i] = v;
		}
	}

	tiletype GetTileType()
	{
		tiletype tt;
		tt.f = INT_PTR(data);
		tt.p = sizeX * 4;
		tt.x = sizeX;
		tt.y = sizeY;
		return tt;
	}
	void Draw(const HSpan& hspan, std::function<T(T)> f)
	{
		for(const HSpan::Entry &entry : hspan.entries)
		{
			T* line = data + entry.y * sizeX;
			for(int x = entry.xIn; x < entry.xOut; x++)
			{
				line[x] = f(line[x]);
			}
		}
	}

	void line(const vec2& p0, const vec2& p1, T v)
	{
		// calculate dx , dy
		vec2 d = p1 - p0;

		// Depending upon absolute value of dx & dy
		// choose number of steps to put pixel as
		int steps = abs(d.x) > abs(d.y) ? int(abs(d.x)) : int(abs(d.y));
		 
		// calculate increment in x & y for each steps
		vec2 inc = d / (float)steps;

		// Put pixel for each step
		vec2 p = p0;
		for (int i = 0; i <= steps; i++)
		{
			Write(int(p.x), int(p.y), v);
			p += inc;
		}
	}

	void copy(const Image<T> &src)
	{
		for (int i = 0; i < sizeX * sizeY; i++)
		{
			data[i] = src.data[i];
		}
	}

};

