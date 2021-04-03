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

//#include "common.h"
#include "Point3.h"
//#include "TrPoint.h"

namespace FADE3D {

/** \brief Line segment
*/
class CLASS_DECLSPEC Segment3
{
private:
	Point3 src,trg;
public:
	Segment3(const Point3& src,const Point3& trg);
/** \brief Get the source point
*/
	Point3 getSrc() const;
/** \brief Get the target point
*/
	Point3 getTrg() const;
/// @private
	void debug();
	friend std::ostream &operator<<(std::ostream &stream, Segment3 seg);
};



} // NAMESPACE FADE3D


