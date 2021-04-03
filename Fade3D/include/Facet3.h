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
#include "Edge3.h"

namespace FADE3D {
/** \brief Side of a tetrahedron
 *
 * A Facet3 is one of the four sides (triangles) of a tetrahedron and
 * it is represented by a Tet3 pointer and the opposite IntraTetIndex.
 *
 * \image html Facet3.png "Facet (0,1,2) of a tetrahedron, selected by opp3=3"
 * \image latex Facet3.eps "Edge (0,1,2) of a tetrahedron, selected by opp3=3" width=6cm

*/

class CLASS_DECLSPEC Facet3
{
public:
/**
 *
* \param pTet_ is a pointer to a Tet3
* \param opp3_ selects the side of \p pTet
*/
	Facet3(Tet3* pTet_,const int opp3_);

/** Get the Tet3
*/
	Tet3* getTet();

/** Get the IntraTetIndex
*
* \return the IntraTetIndex of the tetrahedron which selects the side
*/
	int getOpp3Index() const;

/** Get the index of the \p ith vertex of the facet
*/
	int getIntraTetIndex(int ith) const;

/** Get the \p ith edge of the facet
*
* \param ith {0,1,2} selects the edge to be returned
*/
	Edge3 getEdge(int ith) const;

/** Check if two Facet3 objects refer to the same triangle
*
* \return true if the present Facet3 and \p other refer to the same
* facet and false otherwise
*
* \note Inner facets of a triangulation are shared by two tetrahedra,
* thus the same facet can be expressed with two different Tet3 objects
* and opposite indices.
*
*/
	bool operator==(const Facet3& other) const;
/** Check if two Facet3 objects refer to a different triangle
*/
	bool operator!=(const Facet3& other) const;
private:
	Tet3* pTet;
	int opp3ICI;
};




} // NAMESPACE FADE3D


