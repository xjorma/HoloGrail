//
// C++ Interface: Ball
//
// Description:
//
//
// Author: Bernhard Kornberger <bkorn@geom.at>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#pragma once
#include "Point3.h"


namespace FADE3D {

/** \brief Sphere
*/
class CLASS_DECLSPEC Ball3
{
public:
/**
 *
 * @param x,y,z coordinates of the center
 * @param sqRadius_ is the squared radius
 */
	Ball3(double x,double y,double z,double sqRadius_);
	~Ball3();
/** Get the radius
*/
	double getRadius() const;
/** Get the squared radius
*/
	double getSqRadius() const;
/** Get the center
*/
	Point3 getCenter() const;

/// @private
	void debug() const;
	friend std::ostream &operator<<(std::ostream &stream, const Ball3& b);

protected:
	Point3 center;
	double sqRadius;


};


} // NAMESPACE FADE3D

