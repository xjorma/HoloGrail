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
#include "Tet3.h"
#include "Facet3.h"
#include "Edge3.h"

namespace FADE3D {


class CLASS_DECLSPEC TetAroundEdgeIterator
{
private:
	Edge3 edge;
	Tet3* storedPayload;
	int storedPayloadICI;

public:
	explicit TetAroundEdgeIterator(const Edge3& edge_):edge(edge_),storedPayload(NULL),storedPayloadICI(-1)
	{
	}

	TetAroundEdgeIterator(const TetAroundEdgeIterator& it_):edge(it_.edge),storedPayload(it_.storedPayload),storedPayloadICI(-1)
	{
	}

	TetAroundEdgeIterator& operator++()
	{
		// Get oldTet and the opposite vertex in oldTet
		Tet3* oldTet=edge.getTet();
		int oldOpp2ICI=edge.getOpp2Index();

		// Get the new Tet and the opposite index in it
		Tet3* newTet=oldTet->getOppTet(oldOpp2ICI);
		int newOpp3ICI=newTet->getIntraTetIndex(oldTet);
		int newOpp2ICI=0;
		for(;newOpp2ICI<4;++newOpp2ICI)
		{
			if(newOpp2ICI==newOpp3ICI) continue;
			Point3* tmpVtx=newTet->getCorner(newOpp2ICI);

			if(tmpVtx==edge.getSourceVtx() || tmpVtx==edge.getTargetVtx()) continue;
			break;
		}
		edge=Edge3(newTet,newOpp3ICI,newOpp2ICI);
		return *this;
	}
	bool nextIsNull()
	{
		// Get oldTet and the opposite vertex in oldTet
		Tet3* oldTet=edge.getTet();
		int oldOpp2ICI=edge.getOpp2Index();

		// Get the new Tet and the opposite index in it
		Tet3* newTet=oldTet->getOppTet(oldOpp2ICI);
		if(newTet==NULL) return true;
			else return false;
	}

	bool operator==(const TetAroundEdgeIterator& rhs)
	{
		return (edge==rhs.edge && edge.getTet()==rhs.edge.getTet());
	}

	bool operator!=(const TetAroundEdgeIterator& rhs)
	{
		return !operator==(rhs);
	}

	Tet3* operator*()
	{
		return edge.getTet();
	}

	Tet3* previewNextTet()
	{
		return edge.getTet()->getOppTet(edge.getOpp2Index());
	}

	Edge3 getEdge()
	{
		return edge;
	}

	int getOpp3Index()
	{
		return edge.getOpp3Index();
	}

	int getOpp2Index()
	{
		return edge.getOpp2Index();
	}

	void setPayload(Tet3* c)
	{
		assert(storedPayload==NULL);
		storedPayload=c;
	}

	Tet3* getPayload() const
	{
		assert(storedPayload!=NULL);
		return storedPayload;
	}
	void setPayloadICI(int i)
	{
		assert(storedPayloadICI==-1);
		storedPayloadICI=i;
	}

	int getPayloadICI() const
	{
		assert(storedPayloadICI!=-1);
		return storedPayloadICI;
	}

	void debug()
	{
		std::cout<<"-----------------------------------"<<std::endl;
		std::cout<<"# This is a TetAroundEdgeIterator:"<<std::endl;
		std::cout<<edge;
		std::cout<<"-----------------------------------"<<std::endl<<std::endl;
	}
};




} // NAMESPACE FADE3D


