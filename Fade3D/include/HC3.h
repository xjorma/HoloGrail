
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
#include "common.h"
#include "Point3.h"
#include "Bbox3.h"
#include "geom_tools/SimplePool.h"
#include "IdxVtx.h"

namespace FADE3D {


//struct Validator // is actually LtPointsByLabel
//{
	//bool operator()(const Point3* p0,const Point3* p1) const
	//{
		//if(p0->internalLabel < p1->internalLabel) return true;
		//if(p0->internalLabel > p1->internalLabel) return false;
		//return *p0<*p1;
	//}
//};
enum HState{S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11};

class HC3
{
public:
	explicit HC3(unsigned numBits_);
	HC3(double minx_,double miny_,double minz_,double maxx_,double maxy_,double maxz_,unsigned numBits_);
	unsigned iReset(const Point3& p);

#if 0
	Point3* createPendingVertex(double x,double y,double z);
	void getPendingVertices(std::vector<Point3*>& vPendingPoints_);
	void clearPendingVertices();
#endif
	void addPoints(
			std::vector<Point3>& vPoints,
			std::vector<Point3*>& vUserHandles,
			std::vector<Point3*>& vHilHandles
			);
	std::pair<Point3*,Tet3*> addPoint(Point3& p);
	void getVertices(std::vector<Point3*>& vVtxPtr) const;
	//Point3* getCloseVtx(unsigned internalLabel);
	Tet3* getCloseTet(Point3* pVtx);
	Tet3* getCloseTet(Point3& p);
	size_t getNumStoredVtx() const;
	void checkQ();
protected:
	void reconfig();
	Bbox3 bbox;
	double range;
	size_t offset;
	unsigned numBits;
	double& minX;
	double& minY;
	double& minZ;
	double& maxX;
	double& maxY;
	double& maxZ;
	double rangeX;
	double rangeY;
	double rangeZ;


	static unsigned addVls[8][12];
	static unsigned nextS[8][12];

	bool bValid;
	GEOM_TOOLS::SimplePool<Point3> vertexSimplePool;
	GEOM_TOOLS::SimplePool<IdxVtx> idxVtxSimplePool;
	//std::set<Point3*,Validator> sVtx;
	std::vector<Point3*> vPendingPoints;

	typedef std::set<IdxVtx*,Func_ltIdxVtxPtr> SIdxVtx;
	typedef std::set<IdxVtx*,Func_ltIdxVtxPtr>::iterator SIdxVtxIt;
	SIdxVtx sIdxVtxPtr;
};






} // NAMESPACE FADE3D


