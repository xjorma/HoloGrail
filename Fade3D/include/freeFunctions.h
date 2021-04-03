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
#include "Point3.h"
#include <vector>

namespace FADE3D {


/** \defgroup freeFunctions Free Functions
 *
 *  @{
 */

/** \brief Get normal vector
 *
* Returns the normalized normal vector of a triangle made of the three
* input points
*/
CLASS_DECLSPEC
Vector3 getNormalVector(const Point3& p0,const Point3& p1,const Point3& p2);

CLASS_DECLSPEC
void printLicense();

/** \brief Fade version string
*
* This method returns a version string
*/
CLASS_DECLSPEC
std::string getFade3DVersion();
/** \brief Get the major version number
*/
CLASS_DECLSPEC
int getMajorVersionNumber();
/** \brief Get the minor version number
*/
CLASS_DECLSPEC
int getMinorVersionNumber();
/** \brief Get the revision version number
*/
CLASS_DECLSPEC
int getRevisionNumber();
/** \brief Check if a RELEASE or a DEBUG version is used.
*/
CLASS_DECLSPEC
bool isRelease();

/** \brief Write points to an ASCII file
 *
 * Writes points to an ASCII file, three coordinates (x y z) per line,
 * whitespace separated.
*/
CLASS_DECLSPEC
bool writePoints(const char* filename,const std::vector<Point3*>& vPointsOut);

/** \brief Write points to an ASCII file
 *
 * Writes points to an ASCII file, three coordinates (x y z) per line,
 * whitespace separated.
*/
CLASS_DECLSPEC
bool writePoints(const char* filename,const std::vector<Point3>& vPointsIn);



/** @}*/
} // NAMESPACE
