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
#include "definitions.h"

namespace FADE3D {

class Tet3;
class Point3;

/** \brief Edge of a tetrahedron
 *
 * An Edge3 is represented by a Tet3 pointer and two two IntraTetIndices
 * <EM>opp3</EM> and <EM>opp2</EM>. The Edge of the tetrahedron is
 * selected as follows: At first, opp3 selects the facet (triangle)
 * of the tetrahedron opposite to the corner addressed by opp3. Then
 * opp2 selects the edge in this triangle opposite to the vertex at
 * position opp2. See the image where opp3=3 and opp2=0. The edge
 * proceeds from corner 1 to corner 2.
 *
 * \image html Edge3.png "Edge (0,1) of a tetrahedron, selected by opp3=3 and opp2=0"
 * \image latex Edge3.eps "Edge (0,1) of a tetrahedron, selected by opp3=3 and opp2=0" width=6cm
*/

class CLASS_DECLSPEC Edge3
{
public:
/** \brief Constructor of Edge3
*
* \param pTet_ Tetrahedron
* \param opp3_ selects one of the four triangles of the tetrahedron
* \param opp2_ selects one of the edges of the triangle
*/
	Edge3(Tet3* pTet_ ,const int opp3_,const int opp2_);
/** \brief Get the tetrahedron
*/
	Tet3* getTet() const;
/** \brief Get the opp3ITI index
*/
	int getOpp3Index() const;
/** \brief Get the opp2ITI index
*/
	int getOpp2Index() const;
/** \brief Get the source index
*
* \returns the index of the tetrahedron which selects the source vertex of the edge
*/
	int getSourceIndex	() const;
/** \brief Get the target index
*
* \returns the index of the tetrahedron which selects the target vertex of the edge
*/
	int getTargetIndex() const;
/** \brief Get the source vertex of the edge
*/
	Point3* getSourceVtx() const;
/** \brief Get the target vertex of the edge
*/
	Point3* getTargetVtx() const;
/** \brief Check if two undirected edges coincide
*
* Two edges are equal when their vertices coincide (undirected edge)
* even if the two objects use different tetrahedra that meet on this
* edge.
*/
	bool operator==(const Edge3& rhs) const;
/** \brief Check if two undirected edges are different
*
* Two Edge3 objects are different when they do not refer to the same
* undirected edge.
*/
	bool operator!=(const Edge3& rhs) const;

// Internal use, compares the pointers (used to sort lists of edges)
/// @private
	bool operator<(const Edge3& other) const;
	friend std::ostream &operator<<(std::ostream &stream, Edge3& e);
private:
	Tet3* pTet;
	int opp3ICI;
	int opp2ICI;
	Point3 *pOrderedVtx0,*pOrderedVtx1;
};





} // NAMESPACE FADE3D


