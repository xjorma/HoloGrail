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
#include <iostream>
#include <fstream>
#include "Ball3.h"
#include "Triangle3.h"
#include "Bbox3.h"
namespace FADE3D {

/** \brief Geomview visualizations
*/
class Visualizer3
{
public:
	explicit Visualizer3(const std::string& filename);
	~Visualizer3();
	void closeFile();
	void openFile(std::string filename);
	//void writePoints(std::vector<Point3>& vPoints,unsigned linewidth,const std::string& color) ;
	//void writePoint(Point3& p,unsigned linewidth,const std::string& color);
	void writeBall(Ball3* ball);
	void writeBall(const Point3& center,double weight,bool bTransparent=false);
	void writeBalls(const std::vector<Ball3>& vBalls,bool bTransparent=false);
	void writeSegment(const Point3& src,const Point3& trg,const std::string& c);
	void writePolygon(std::vector<Point3>& vPoints,const std::string& c);
	void writeTetrahedron(Tet3* pTet,const std::string& c);
	void writeTriangle(Triangle3* pT,const std::string& c);
	void writeTriangles(std::vector<Triangle3*> vTriangles,const std::string& c);
	void writeBbox(const Bbox3& bbx,const std::string& c);

	void writePoint(const Point3& p,unsigned lineWidth,const std::string& color);
	void writePoints(std::vector<Point3>& vPoints,unsigned lineWidth,const std::string& color);
	void writePoints(std::vector<Point3*>& vPoints,unsigned lineWidth,const std::string& color);

	static std::string getNextColor();
	static std::string getColor(unsigned ith);
	static std::string getColorName(unsigned ith);

	static std::string CLIGHTBLUE;
	static std::string CDARKBLUE;
	static std::string CYELLOW;
	static std::string CPINK;
	static std::string CBLACK;
	static std::string CLIGHTBROWN;
	static std::string CDARKBROWN;
	static std::string CORANGE;
	static std::string CPURPLE;
	static std::string CGRAY;
	static std::string CLIGHTGRAY;
	static std::string CRED;
	static std::string CGREEN;
	static std::string CWHITE;
	static std::string CRIMSON;
	static std::string CDARKORANGE;
	static std::string CGOLDENROD;
	static std::string COLIVE;
	static std::string CLAWNGREEN;
	static std::string CGREENYELLOW;
	static std::string CPALEGREEN;
	static std::string CMEDSPRINGGREEN;
	static std::string CLIGHTSEAGREAN;
	static std::string CCYAN;
	static std::string CSTEELBLUE;
	static std::string MIDNIGHTBLUE;
	static std::string CWHEAT;


protected:
	std::ofstream outFile;
	void startList(const size_t numPoints,const size_t numTriangles,bool bWithEdges=true);
	void endList();
	static const std::string colorNames[27];
	static const std::string colorArray[27];
	static int nextColorIdx;

};



} // NAMESPACE FADE3D


