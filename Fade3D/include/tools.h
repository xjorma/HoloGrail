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
#include <algorithm>
#include <assert.h>
#include <map>
#include "Point3.h"
#include "textTools.h"
namespace FADE3D {


std::string repstr(std::string str, const std::string& from, const std::string& to);

void goToHell(const std::string& s);
#define ASSEX(CONDITION) \
if(!(CONDITION)) \
{\
	error("Fade, " ,"Condition [ "+std::string(#CONDITION)+" failed ]"); \
	std::cout<<"      "<<__FILE__<<":"<<__LINE__<<std::endl; \
	FadeException fadeEx; \
	throw fadeEx; \
}\

#define GEOM_CCHECKS
#ifdef GEOM_CCHECKS
#define ASSEXC(CONDITION) ASSEX(CONDITION)
#else
#define ASSEXC(CONDITION)
#endif


//#define GEOM_ECHECKS
#ifdef GEOM_ECHECKS
#define ASSEXE(CONDITION) ASSEX(CONDITION)
#else
#define ASSEXE(CONDITION)
#endif



template <typename T> inline void unused(const T&){}

template<class T_PTR> struct func_ltDerefPtr
{
	bool operator()(T_PTR p0,T_PTR p1) const {return *p0<*p1;}
};

//#define GEOM_BENCHCOUNTING
#ifdef GEOM_BENCHCOUNTING
		#define BC(x) benchCounter(x);
#else
		#define BC(x)
#endif


void showTets(const std::string& name,std::vector<Tet3*>& vTets);
void showTets(const std::string& name,std::vector<Tet3*>& vTets,const Point3& p);
void listTets(const std::string& name,std::vector<Tet3*>& vTets,bool bShort);
inline double sqDistance(const Point3& p0, const Point3& p1);
inline double sqDistance(const double x,const double y, const Point3& p1);
void stopWatch(const std::string& str);

template<class T> void makeUnique(T& v)
{
	sort(v.begin(),v.end());
	v.resize(unique(v.begin(),v.end())-v.begin());
}

void benchCounter(const std::string& str);


//inline std::string redString(std::string s)
//{
//#ifndef _WIN32
        //return ("\033[0;31m"+s+"\033[m");
//#else
        //return s;
//#endif
//}

//inline std::string greenString(std::string s)
//{
//#ifndef _WIN32
        //return ("\033[0;32m"+s+"\033[m");
//#else
        //return s;
//#endif
//}

//inline std::string magentaString(std::string s)
//{
//#ifndef _WIN32
        //return ("\033[0;35m"+s+"\033[m");
//#else
        //return s;
//#endif
//}

//inline std::string yellowString(std::string s)
//{
//#ifndef _WIN32
        //return ("\033[0;33m"+s+"\033[m");
//#else
        //return s;
//#endif
//}

//inline std::string red()
//{
//#ifndef _WIN32
        //return ("\033[0;31m");
//#else
        //return std::string("");
//#endif
//}

// Switch to magenta (and not back again)
//inline std::string magenta()
//{
//#ifndef _WIN32
        //return ("\033[0;35m");
//#else
        //return std::string("");
//#endif
//}

//// Switch to white (and not back again)
//inline std::string white()
//{
//#ifndef _WIN32
        //return ("\033[m");
//#else
        //return std::string("");
//#endif
//}

//// Switch to yellow (and not back again)
//inline std::string yellow()
//{
//#ifndef _WIN32
        //return ("\033[0;33m");
//#else
        //return std::string("");
//#endif
//}

// Baricenter of 4 points
inline Point3 barycenter(const Point3& p0,const Point3& p1,const Point3& p2,const Point3& p3)
{
	return Point3((p0.x()+p1.x()+p2.x()+p3.x())/4.0,(p0.y()+p1.y()+p2.y()+p3.y())/4.0,(p0.z()+p1.z()+p2.z()+p3.z())/4.0);
}

// Baricenter of 3 points
inline Point3 barycenter(const Point3& p0,const Point3& p1,const Point3& p2)
{
	return Point3(	(p0.x()+p1.x()+p2.x())/3.0,
					(p0.y()+p1.y()+p2.y())/3.0,
					(p0.z()+p1.z()+p2.z())/3.0
					);
}

bool readXYZ(const char* filename,std::vector<Point3>& vPointsOut);


// Center of two points
inline Point3 barycenter(const Point3& p0,const Point3& p1)
{
	return Point3(	(p0.x()+p1.x())/2.0,
					(p0.y()+p1.y())/2.0,
					(p0.z()+p1.z())/2.0
					);
}

inline
bool range03(int num)
{
	if(num>=0 && num<=3) return true;
	return false;
}


inline
int incIndexBy1(const int num)
{
	assert(range03(num));
	int incArray[]={1,2,3,0};
	return incArray[num];
}

inline
int incIndexBy2(const int num)
{
	assert(range03(num));
	int decArray[]={2,3,0,1};
	return decArray[num];
}

inline
int incIndexBy3(const int num)
{
	assert(range03(num));
	int decArray[]={3,0,1,2};
	return decArray[num];
}

inline
int decRangeIndexBy1(const int num)
{
	assert(range03(num));
	int decArray[]={3,0,1,2};
	return decArray[num];
}


struct Bounds
{
	double minX,minY,maxX,maxY;
	Bounds();
	Bounds(Bounds &bnd);
	Bounds(double minX_,double minY_,double maxX_,double maxY_);
	void debug();
};

struct Func_objBounds
{
	Bounds* pBnd;
	explicit Func_objBounds(Bounds& bnd_);
	void operator()(const Point3& p);
};


template<class T>
void oneInstance(T& v)
{
	sort(v.begin(),v.end());
	v.resize(unique(v.begin(),v.end())-v.begin());
}

//printf( " Warning: IEEE floating points used: Define -DEXACT in makefile \n");

inline
Point3 tetCircumcenter(double a[3], double b[3], double c[3], double d[3])
{
  double xba, yba, zba, xca, yca, zca, xda, yda, zda;
  double balength, calength, dalength;
  double xcrosscd, ycrosscd, zcrosscd;
  double xcrossdb, ycrossdb, zcrossdb;
  double xcrossbc, ycrossbc, zcrossbc;
  double denominator;
  double xcirca, ycirca, zcirca;

  /* Use coordinates relative to point `a' of the tetrahedron. */
  xba = b[0] - a[0];
  yba = b[1] - a[1];
  zba = b[2] - a[2];
  xca = c[0] - a[0];
  yca = c[1] - a[1];
  zca = c[2] - a[2];
  xda = d[0] - a[0];
  yda = d[1] - a[1];
  zda = d[2] - a[2];
  /* Squares of lengths of the edges incident to `a'. */
  balength = xba * xba + yba * yba + zba * zba;
  calength = xca * xca + yca * yca + zca * zca;
  dalength = xda * xda + yda * yda + zda * zda;
  /* Cross products of these edges. */
  xcrosscd = yca * zda - yda * zca;
  ycrosscd = zca * xda - zda * xca;
  zcrosscd = xca * yda - xda * yca;
  xcrossdb = yda * zba - yba * zda;
  ycrossdb = zda * xba - zba * xda;
  zcrossdb = xda * yba - xba * yda;
  xcrossbc = yba * zca - yca * zba;
  ycrossbc = zba * xca - zca * xba;
  zcrossbc = xba * yca - xca * yba;

  /* Calculate the denominator of the formulae. */
#ifdef EXACT
  /* Use orient3d() from http://www.cs.cmu.edu/~quake/robust.html     */
  /*   to ensure a correctly signed (and reasonably accurate) result, */
  /*   avoiding any possibility of division by zero.                  */
  denominator = 0.5 / orient3d(b, c, d, a);
#else
  /* Take your chances with floating-point roundoff. */

  denominator = 0.5 / (xba * xcrosscd + yba * ycrosscd + zba * zcrosscd);
#endif

  /* Calculate offset (from `a') of circumcenter. */
  xcirca = (balength * xcrosscd + calength * xcrossdb + dalength * xcrossbc) *
           denominator;
  ycirca = (balength * ycrosscd + calength * ycrossdb + dalength * ycrossbc) *
           denominator;
  zcirca = (balength * zcrosscd + calength * zcrossdb + dalength * zcrossbc) *
           denominator;
	return Point3(xcirca+a[0],ycirca+a[1],zcirca+a[2]);
}

inline
Point3 tetCircumcenter(const Point3& pa,const Point3& pb,const Point3& pc,const Point3& pd)
{
	double a[3],b[3],c[3],d[3];
	pa.xyz(a[0],a[1],a[2]);
	pb.xyz(b[0],b[1],b[2]);
	pc.xyz(c[0],c[1],c[2]);
	pd.xyz(d[0],d[1],d[2]);


	return tetCircumcenter(a,b,c,d);
}



} // NAMESPACE FADE3D


