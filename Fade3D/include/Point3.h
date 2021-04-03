
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
#include <float.h>
#include "definitions.h"

namespace FADE3D {


class Tet3; // FWD
class Vector3; // FWD
/** \brief Vertex
*/

class CLASS_DECLSPEC Point3
{
	friend class Tet3;
	friend class HC3;
	friend struct Validator;
public:
/** \brief Constructor
*
* Coordinates are initialized to -DBL_MAX, the custom index is initialized to -1, the associated incident tetrahedron pointer is initialized to NULL.
*/
	Point3();
/**
* This constructor initializes the custom index to -1 and the associated
* incident tetrahedron to NULL.
*/
	Point3(const double x,const double y,const double z);
/**
* The copy constructor copies the coordinates and the custom index but
* not the associated incident tetrahedron
*/
	Point3(const Point3& p_);
	~Point3();

/** \brief Get the x coordinate
*/
	double x() const;

/** \brief Get the y coordinate
*/
	double y() const;

/** \brief Get the u coordinate
*/
	double z() const;
/** \brief Access all coordinates at once
*/
	void xyz(double& x_,double& y_,double& z_) const;

/** \brief Get one incident tetrahedron
*
* \return an incident tetrahedron if one exists
* \return NULL otherwise
*/
	Tet3* getOneTet() const;
/// @private
	void init(const Point3& p);
/// @private
	void setOneTet(Tet3* pc);


	void exchange(double x,double y,double z);
	void debug();

	//void setCustomIndex(int customIndex_);
	int getCustomIndex();

/** \brief Less than operator
*
* Compares the coordinates of the points
*/
	bool operator<(const Point3& p) const;

/** \brief Equality operator
*
* Compares the coordinates of the points
*/
	bool operator==(const Point3& p) const;

	Vector3 operator-(const Point3& other) const;
	Point3 operator+(const Vector3& vec) const;

	friend std::ostream &operator<<(std::ostream &stream, const Point3& pnt);
	friend std::istream &operator>>(std::istream &stream, Point3& pnt);
//#ifdef GEOM_DEV_DEBUG
///** Deprecated, will be removed
//*/
	//static int runningCtr;
	//int label;
///** Deprecated, will be removed
//*/
//#endif




protected:
	double coordX;
	double coordY;
	double coordZ;
	Tet3* pAssociatedTet;
	int customIndex;
};


// FREE FUNCTIONS

/** \brief Compute the squared distance
*/
inline
double sqDistance(const Point3& p0,const Point3& p1)
{
	double deltaX=p1.x()-p0.x();
	double deltaY=p1.y()-p0.y();
	double deltaZ=p1.z()-p0.z();
	return (deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
}

/** \brief Compute the squared distance
*/
inline
double sqDistance(const double x0,const double y0,const double z0,const Point3& p1)
{
	double deltaX=p1.x()-x0;
	double deltaY=p1.y()-y0;
	double deltaZ=p1.z()-z0;
	return (deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
}


} // NAMESPACE FADE3D


