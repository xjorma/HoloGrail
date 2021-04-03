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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Ball3.h"
#include "Point3.h"
#include "Edge3.h"
#include "Facet3.h"
#include "definitions.h"
#include "textTools.h"

namespace FADE3D {

/** \brief Tetrahedron
 *
 * The 4 corners of a Tetrahedron (Tet3) are addressed by the
 * Intra-Tetrahedron-Indices 0,1,2 and 3. For short we refer to them as
 * the IntraTetIndices. A Tet3 is oriented and thus its IntraTetIndices
 * appear in a specific order. Here is a memory hook (see the image):
 * When a triangle with counterclockwise indices 0,1,2 lies on the
 * floor then the remaining vertex 3 of the tetrahedron lies above
 * this triangle.
 *
 * \image html tetrahedron-indices.png "IntraTetIndices: A counterclockwise triangle 0,1,2 on the floor and corner 3 above"
 * \image latex tetrahedron-indices.eps "IntraTetIndices: A counterclockwise triangle 0,1,2 on the floor and corner 3 above" width=6cm
 *
*/
class CLASS_DECLSPEC Tet3
{
public:
	Tet3();
	~Tet3();

	// GET
/** \brief Get Circumcenter
 *
 * \return the center of a sphere that passes through the 4 corners
 * of the present Tet3.
 *
*/
	Point3 getCircumcenter();
/** \brief Get Corners
 *
 * Used to access all corners of the present Tet3 at once
 *
*/
	void getCorners(Point3*& p0,Point3*& p1,Point3*& p2,Point3*& p3) const;
/** \brief Get Corner
 *
 * \return the corner with the \p ith IntraTetIndex
 *
*/
	Point3* getCorner(const int ith) const;
/** \brief Has Vertex
 *
 * \return true if any of the four vertex pointers corresponds to \p p
 *
*/
	bool hasVertex(const Point3* p) const;

/** \brief Has Vertex
 *
 * \return true if the coordinates of any of the four corner vertices
 * correspond to the coordinates of \p p.
 *
*/
	bool hasVertex(const Point3& p) const;
/** \brief Get IntraTetIndex
 *
 * \return the IntraTetIndex of the vertex \p p.
 *
*/
	int getIntraTetIndex(const Point3* p) const;
/** \brief Get IntraTetIndex
 *
 * The present Tet3 and \p pNeigTet must be neighbors. The present
 * method returns the IntraTetIndex of the corner of the present
 * Tet3 opposite to the shared triangle.
 *
*/
	int getIntraTetIndex(const Tet3* pNeigTet) const;
/** \brief Get Opposite Tetrahedron
 *
 * return the opposite tetrahedron of the \p ith corner, i.e., the
 * tetrahedron adjacent at the facet opposite to the \p ith vertex.
 *
*/
	Tet3* getOppTet(const int ith) const;
/** \brief Get Opposite Tetrahedron
 *
 * return the opposite tetrahedron of \p pOppVtx, i.e., the tetrahedron
 * adjacent at the facet opposite to \p pOppVtx.
 *
*/
	Tet3* getOppTet(const Point3* pOppVtx) const;
/** \brief Get Opposite Tetrahedron
 *
 * return the opposite vertex in the \p ith opposite tetrahedron.
 *
*/
	Point3* getOppVtxInOppTet(const int ith,bool bNullAllowed) const;

/** \brief Get Edge Indices
 *
 * return a pair of IntraTetIndices that specify an edge of the
 * tetrahedron.
 *
 * The edge is selected through two IntraTetIndices: At first \p opp3
 * selects the opposite triangle. Then \p opp2 selects the opposite edge
 * in this triangle. The triangle has a counterclockwise orientation
 * (see the image) and thus the two returned indices are exactly
 * defined.
 */

	static std::pair<int,int> getEdgeIndices(int opp3,int opp2);
	// The slower version, which is only called from registerTets
/// @private
	int getIntraTetIndexInOppTetForReg(const Tet3* c,int opp3) const;
	// The efficient version which assumes an intact Tet
/// @private
	int getIntraTetIndexInOppTet(int ownIndex) const;
/// @private
	void getFacetIndices(int opp3,int& iciA,int& iciB,int& iciC) const;
/// @private
	static bool haveCommonFacet(Tet3* pC0,int opp0ICI,Tet3* pC1,int opp1ICI);
/** \brief Get Edge
 *
 * return an Edge3 addressed by the two IntraTetIndices \p opp3
 * (selects the opposite triangle) and \p opp2 (selects the edge
 * of this triangle)
 *
*/
	Edge3 getEdge(const int opp3,const int opp2);
/** \brief Get Facet
 *
 * return the Facet3 opposite from the vertex addressed by \p opp3.
 *
*/
	Facet3 getFacet(const int opp3);

	// SET
/// @private
	void setOppTet(const int ith, Tet3* pC);
/// @private
	void setProperties(Point3* pI,Point3* pJ,Point3* pK,Point3* pL);
/// @private
	void setCorner(const int ith,Point3* pp);
/// @private
	void setCustomIndex(int customIndex);
/// @private
	int getCustomIndex();


	// DEBUG
/// @private
	int debug_getTetIndex() const;
	CLASS_DECLSPEC
	friend std::ostream& operator<<(std::ostream &stream, const Tet3& pC);
/// @private
	int debug_getOppTetIndex(const int ith) const;
/// @private
	bool hasBeenVisited;
/// @private
	int label;
/// @private
	static int runningLabel;
private:
	Tet3(const Tet3 &c);
	Point3* aVertexPointer[4];
	Tet3* aOppTets[4];
/// @private
	int customIndex;
};





} // NAMESPACE FADE3D







