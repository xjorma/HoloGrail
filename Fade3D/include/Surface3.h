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


#include <vector>
#include <map>
#include "Point3.h" // From Fade_3D
#include "geom_tools/SimplePool.h"


namespace FADE3D {
class Dt3; // FWD
class Fade_3D; // FWD
class Triangle3; // FWD
template <class T> class SimplePool; // FWD
typedef ::GEOM_TOOLS::SimplePool<Triangle3> TrianglePool;
typedef std::pair<Point3*,Point3*> VtxPair;


class Surface3
{
public:
	Surface3(Dt3* pDT_);
	~Surface3();
	Point3* createVertex(double x,double y,double z);
	Triangle3* createTriangle(Point3* p0,Point3* p1,Point3* p2);
	bool establishNeighborships();
	void getVertices(std::vector<Point3*>& vVtx) const;
	void getTriangles(std::vector<Triangle3*>& vTri) const;
	void getIncidentTriangles(Point3* pVtx,std::vector<Triangle3*>& vTri) const;
	void getIncidentVertices(Point3* pVtx,std::vector<Point3*>& vVtx) const;
	bool readSTL(const std::string& filename,bool bVerbose=false);
	void writeGeomview(const std::string& filename,const std::string& color);
	Fade_3D* getFade3D();


protected:
	std::map<Point3*,Triangle3*> mVtxTri;
	Surface3(const Surface3& ){}
	TrianglePool* pTrianglePool;
	std::vector<Point3*> vVertices;
	std::vector<Triangle3*> vTriangles;
	Dt3* pDT;

};




} // NAMESPACE FADE3D
