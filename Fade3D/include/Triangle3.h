// Copyright (C) Geom Software e.U, Bernhard Kornberger, Graz/Austria
//
// This file is part of the Fade3D library. The student license is free
// of charge and covers personal non-commercial research. Licensees
// holding a commercial license may use this file in accordance with
// the Commercial License Agreement.
//
// This software is provided AS IS with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE.
//
// Please contact the author if any conditions of this licensing are
// not clear to you.
//
// Author: Bernhard Kornberger, bkorn (at) geom.at
// http://www.geom.at


#pragma once
#include <iostream>
#include <assert.h>
#include "Point3.h"
#include "definitions.h"

namespace FADE3D {



inline
int incTriIndexBy1(int i)
{
	return (i+1)%3;
}

inline
int incTriIndexBy2(int i)
{
	return (i+2)%3;
}

class CLASS_DECLSPEC Triangle3
{
public:
	Triangle3(Point3* p0_,Point3* p1_,Point3* p2_)
	{
		aVtx[0]=p0_;
		aVtx[1]=p1_;
		aVtx[2]=p2_;
		aNeigs[0]=NULL;
		aNeigs[1]=NULL;
		aNeigs[2]=NULL;

	}

	Point3* getCorner(int i) const
	{
		assert(i>-1 && i<3);
		return aVtx[i];
	}

	Triangle3* getNeig(int i) const
	{
		assert(i>-1 && i<3);
		return aNeigs[i];
	}

	void setNeig(int i,Triangle3* pNeig)
	{
		assert(i>-1 && i<3);
		aNeigs[i]=pNeig;
	}

	int getIntraTriangleIndex(Point3* p) const
	{
		for(int i=0;i<3;++i)
		{
			if(	aVtx[i]==p ) return i;
		}
		std::cerr<<"Triangle3::getIntraTriangleIndex(): invalid (p)"<<std::endl;
		return INVALID_IDX; // Never reached
	}

	int getIntraTriangleIndex(Point3* p1,Point3* p2) const
	{
		for(int i=0;i<3;++i)
		{
			if(
				(aVtx[incTriIndexBy1(i)]==p1 && aVtx[incTriIndexBy2(i)]==p2) ||
				(aVtx[incTriIndexBy1(i)]==p2 && aVtx[incTriIndexBy2(i)]==p1)
				)
			{
				return i;
			}
		}
		std::cerr<<"Triangle3::getIntraTriangleIndex(): invalid (p1,p2)"<<std::endl;
		std::cout<<"Have searched for "<<p1<<" and "<<p2<<" in "<<*this<<std::endl;
		return INVALID_IDX; // Never reached
	}

	Point3 getBarycenter() const
	{
		return Point3(	(aVtx[0]->x()+aVtx[1]->x()+aVtx[2]->x())/3,
						(aVtx[0]->y()+aVtx[1]->y()+aVtx[2]->y())/3,
						(aVtx[0]->z()+aVtx[1]->z()+aVtx[2]->z())/3);
	}

	friend std::ostream &operator<<(std::ostream &stream, const Triangle3& pC);

private:
	Triangle3(); // No standard constructor
	Triangle3(const Triangle3&); // No copy constructor


	Point3* aVtx[3];
	Triangle3* aNeigs[3];
};





} // NAMESPACE FADE3D
