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

#include "definitions.h"
#include "Visualizer3.h"
#include "Point3.h"
#include "Tet3.h"
#include "Performance.h"
//#include "Surface3.h"
#include "Vector3.h"
#include "testDataGenerators.h"
#include "tools.h"
#include "freeFunctions.h"

namespace FADE3D {

/// Orientations
enum Orientation3{
	ORIENTATION3_POSITIVE,
	ORIENTATION3_NEGATIVE,
	ORIENTATION3_COPLANAR
	};


class Dt3; // Forward

/** \brief 3D Delaunay triangulation - the main class
*
* Fade_3D represents a 3D Delaunay triangulation (tetrahedralization)
*/
class Fade_3D
{
public:
	CLASS_DECLSPEC
	Fade_3D()
	{
		initFade();
	}

	CLASS_DECLSPEC
	~Fade_3D();

/** \brief Get all tetrahedra around a vertex
*
* \param [in] pVtx is the query vertex
* \param [out] vTetOut is used to return the incident tetrahedra
*/
	CLASS_DECLSPEC
	void getTetsAroundVertex(Point3* pVtx,std::vector<Tet3*>& vTetOut);


/// @private
	CLASS_DECLSPEC
	void setVerbosity(bool b);

// \brief Orientation test
//*
//* This method returns the orientation of q with respect to the plane of the triangle (p0,p1,p2).
//* The result is either ORIENTATION3_POSITIVE, ORIENTATION3_NEGATIVE or ORIENTATION3_COPLANAR. Which is the ORIENTATION3_POSITIVE side?
//* If you look at a triangle and you see the vertices (p0,p1,p2) in counterclockwise order
//* then you are on the positive side.
//*/
	//CLASS_DECLSPEC
	//Orientation3 getOrientation(const Point3& p0,
								//const Point3& p1,
								//const Point3& p2,
								//const Point3& q);

// \brief Returns an empty Surface object (currently not in the API)
//*
//* This method creates an empty Surface object. Surface is currently not part of
//* the API. Surface objects will later be used to define surfaces or to read
//* triangles from STL files. Points created by Surface are owned by the Fade_3D
//* object (thus they do not need to be recreated later) but are not triangulated
//* until the triangulation is explicitly launched.
//*/
	//CLASS_DECLSPEC
	//Surface3* createEmptySurface();

// \brief Inserts pending points (currently for internal use)
//*
//* This method inserts points that have been created but not triangulated yet.
//*/
	//CLASS_DECLSPEC
	//void insertPendingPoints();


/** \brief Check validity of the tetrahedral mesh
*
* This is a debug method, primary ment for internal use to check if the
* internal data strucutre is valid. It may be time-consuming. Don't use
* this method unless you assume that something is wrong.
*
* \param msg is a debug string that will be shown when the check fails
* \param bCheckSphereInc specifies if the empty sphere property shall be checked also
*
*
*/
	CLASS_DECLSPEC
	bool checkValidity(const std::string& msg,bool bCheckSphereInc);


/** \brief Draw a 3D scene
*
* This method draws all tetrahedra. The output is a *.list file for Geomview
*
*/
	CLASS_DECLSPEC
 	void show(const std::string& filename,Point3* pVtx=NULL);

/// @private
	CLASS_DECLSPEC
 	void print(const std::string& name);


/** \brief Insert a single 3D point
*
* @param p is the point to be inserted.
*
* The triangulation keeps a copy of \e p and returns a pointer to this
* copy. If duplicate points are inserted the returned pointer is always
* the same (the one of the very first insertion).
*
* @return a pointer to the point in the triangulation
*
* @note This method is fast but it is even faster to pass all points
* at once if possible. See void insert(const std::vector<Point3>& vInputPoints)
*/
	CLASS_DECLSPEC
	Point3* insert(Point3& p);

/** \brief Insert a vector of 3D points.
*
* @param vInputPoints contains the points to be inserted.
*
*/
	CLASS_DECLSPEC
	void insert(std::vector<Point3>& vInputPoints);

/** \brief Insert 3D points from \e vInputPoints and store pointers in \e vHandles
*
* @param [in] vInputPoints contains the points to be inserted.
* @param [out] vHandlesOut (empty) is used to return Point3 pointers
*
* Internally, the triangulation keeps copies of the inserted points which
* are returned in \e vHandles (in the same order). If duplicate points are
* contained in vInputPoints then only one copy will be made and a pointer
* to this unique copy will be stored in vHandles for every occurance.
*/

	CLASS_DECLSPEC
	void insert(std::vector<Point3>& vInputPoints,std::vector<Point3*>& vHandlesOut);

/** \brief Locate a tetrahedron which contains \e p
* The Fade_3D class can be used as a data structure for point location.
* This method returns a pointer to a tetrahedron which contains \e p.
*
* @param p is the query point
* @return a pointer to a Tet3 object (or NULL if is3D()==false or if \e p is outside the triangulation)
*/
	CLASS_DECLSPEC
	Tet3* locate(const Point3& p);

/** \brief Get all Tet3 (tetrahedra)
*
* \param [out] vTetrahedra is used to return Tet3 pointers.
*/
	CLASS_DECLSPEC
	void getTetrahedra(std::vector<Tet3*>& vTetrahedra) const;

/** \brief Get all vertices
*
* \param [out] vVertices is used to return Point3 pointers. The
* order does not necessrily coincide with the insertion order.
*
* \note When duplicate points are inserted then only one copy is made
* and consequently only one vertex pointer is returned for them. Thus
* the number of points returned by the present method can be smaller
* than the number of inserted points.
*/
	CLASS_DECLSPEC
	void getVertices(std::vector<Point3*>& vVertices) const;

/** \brief Check if the triangulation is 3D
*
* Check if the triangulation containts tetrahedra. This is the case
* if at least 4 non-coplanar vertices exist.
*/
	CLASS_DECLSPEC
	bool is3D() const;
/** \brief Print your license type
*/
	CLASS_DECLSPEC
	static void printLicense();

protected:
	///@private
	void initFade();
	///@private
	Fade_3D(const Fade_3D&); // No copy constructor
	///@private
	Fade_3D& operator=(const Fade_3D&); // No assignment allowed
	///@private
	Dt3* pImpl;

};

CLASS_DECLSPEC
std::string getFade3DVersion();
} // (namespace)


// Helper
namespace FADE3D
{
	CLASS_DECLSPEC
	void setLic(
		const std::string& l1,
		const std::string& l2,
		const std::string& dt,
		const std::string& s1,
		const std::string& s2_
		);
	class Lic; // FWD
	Lic* getLic();
}

#ifndef GEOM_NOLIC
	#include "License.h"
#endif

/// @private
CLASS_DECLSPEC
void benchCounter(std::string& s);

