#pragma once
//#include "common.h"
#include "Point3.h"
//#include "GeomTest3.h"

namespace FADE3D {


class GeomTest3; // FWD

/** \brief Axis-aligned minimal 3D bounding box
*/
class CLASS_DECLSPEC Bbox3
{
	friend class HC3;
public:

/** \brief Constructor
*
* Constructor. The bounds of the bounding box are initialized to -DBL_MAX and +DBL_MAX values.
* The bounding box becomes valid as soon as points are added.
*/
	Bbox3()
		:
		minX(DBL_MAX),minY(DBL_MAX),minZ(DBL_MAX),
		maxX(-DBL_MAX),maxY(-DBL_MAX),maxZ(-DBL_MAX)
	{
	}
/** \brief Constructor
*
* This constructor computes the axis aligned minimal bounding box of the points in vPoints
*/
	explicit Bbox3(	const std::vector<Point3>& vPoints)
			:
			minX(DBL_MAX),minY(DBL_MAX),minZ(DBL_MAX),
			maxX(-DBL_MAX),maxY(-DBL_MAX),maxZ(-DBL_MAX)
	{
		add(vPoints);
	}
/** \brief Check if the bounding box is valid
*
* Check if the bounding box has valid bounds. After construction the
* bounds are initialized to DBL_MAX and DBL_MIN. As soon as the first
* point is added Bbox3 becomes valid.
* */
	bool isValid()
	{
		return minX<DBL_MAX;
	}
/** \brief Add points to the bounding box
*
* Add \p vPoints to the bounding box.
* \return true if the bounds of the present Bbox3 have changed
* \return false otherwise
*/
	bool add(const std::vector<Point3>& vPoints)
	{
		if(vPoints.empty()) return false;

		// Initialize if we had an invalid box, return true
		if(!isValid())
		{
			vPoints[0].xyz(minX,minY,minZ);
			vPoints[0].xyz(maxX,maxY,maxZ);
			for(size_t i=1;i<vPoints.size();++i)
			{
				treatPointForValidBox(vPoints[i]);
			}
			return true;
		}

		// Otherwise just add and check if the box changes
		double oldMinX(minX),oldMinY(minY),oldMinZ(minZ),oldMaxX(maxX),oldMaxY(maxY),oldMaxZ(maxZ);
		for(size_t i=0;i<vPoints.size();++i)
		{
			treatPointForValidBox(vPoints[i]);
		}

		if(	oldMinX!=minX ||
			oldMinY!=minY ||
			oldMinZ!=minZ ||
			oldMaxX!=maxX ||
			oldMaxY!=maxY ||
			oldMaxZ!=maxZ)
		{
			return true;
		}
		return false;
	}

/** \brief Add a point to the bounding box
*
* Add \p p to the bounding box
* \return true if the bounds of the present Bbox3 have changed
* \return false otherwise
*/
	bool add(const Point3& p)
	{
		if(isValid())
		{
			double oldMinX(minX),oldMinY(minY),oldMinZ(minZ),oldMaxX(maxX),oldMaxY(maxY),oldMaxZ(maxZ);
			treatPointForValidBox(p);
			if(	oldMinX!=minX ||
				oldMinY!=minY ||
				oldMinZ!=minZ ||
				oldMaxX!=maxX ||
				oldMaxY!=maxY ||
				oldMaxZ!=maxZ)
			{
				return true;
			}
			return false;
		}
		else
		{
			p.xyz(minX,minY,minZ);
			p.xyz(maxX,maxY,maxZ);
			return true;
		}
	}

/// @private
	// factor=1 means that half the range is subtracted from the lower
	// bound and half the range is added to the upper bound
	void enlargeBounds(double factor)
	{
		//std::cout<<"doubleTheBox has been called"<<std::endl;
		//std::cout<<"doubleTheBox, oldVals: minX="<<minX<<", minY="<<minY<<", minZ="<<minZ<<std::endl;
		//std::cout<<"                       maxX="<<maxX<<", maxY="<<maxY<<", maxZ="<<maxZ<<std::endl;
		double addValX=factor*getRangeX()/2.0;
		double addValY=factor*getRangeY()/2.0;
		double addValZ=factor*getRangeZ()/2.0;
		minX-=addValX;
		maxX+=addValX;
		minY-=addValY;
		maxY+=addValY;
		minZ-=addValZ;
		maxZ+=addValZ;
		//std::cout<<"              newVals: minX="<<minX<<", minY="<<minY<<", minZ="<<minZ<<std::endl;
		//std::cout<<"                       maxX="<<maxX<<", maxY="<<maxY<<", maxZ="<<maxZ<<std::endl;
	}

/** \brief Add another bounding box
*
* Add another Bbox3 to the present one.
* \return the axis aligned minimal bounding box of the union of the two boxes.
*/
	Bbox3 operator+(Bbox3& b)
	{
		if(b.minX<minX) minX=b.minX;
		if(b.maxX>maxX) maxX=b.maxX;
		if(b.minY<minY) minY=b.minY;
		if(b.maxY>maxY) maxY=b.maxY;
		if(b.minZ<minZ) minZ=b.minZ;
		if(b.maxZ>maxZ) maxZ=b.maxZ;
		return *this;
	}

/** \brief Get the minimum corner
*
* \return the point with the smallest coordinates of the bounding box
*/
	Point3 getMinPoint()
	{
		return Point3(minX,minY,minZ);
	}
/** \brief Get the maximum corner
*
* \return the point with the largest coordinates of the bounding box
*/
	Point3 getMaxPoint()
	{
		return Point3(maxX,maxY,maxZ);
	}
/** \brief Get the smallest coordinate
*
* \return the minimum of (minX,minY,minZ)
*/
	double getMinCoord()
	{
		if(minX<minY)
		{
			if(minX<minZ) return minX;
				else return minZ;
		}
		else
		{
			if(minY<minZ) return minY;
				else return minZ;
		}
	}
/** \brief Get the largest coordinate
*
* \return the maximum of (maxX,maxY,maxZ)
*/
	double getMaxCoord()
	{
		if(maxX>maxY)
		{
			if(maxX>maxZ) return maxX;
				else return maxZ;
		}
		else
		{
			if(maxY>maxZ) return maxY;
				else return maxZ;
		}
	}
/** \brief Get the x-range
*
* \return the x-range maxX-minX
*/
	double getRangeX() const
	{
		return maxX-minX;
	}
/** \brief Get the y-range
*
* \return the y-range maxY-minY
*/
	double getRangeY() const
	{
		return maxY-minY;
	}
/** \brief Get the z-range
*
* \return the z-range maxZ-minZ
*/
	double getRangeZ() const
	{
		return maxZ-minZ;
	}
/** \brief Get the maximum range
*
* \return the maximum of rangeX,rangeY and rangeZ
*/
	double getMaxRange() const
	{
		double range0=getRangeX();
		double range1=getRangeY();
		double range2=getRangeZ();
		if(range0>range1) std::swap(range0,range1);
		if(range1>range2) std::swap(range1,range2);
		return range2;
	}
/** \brief Get the minimal x coordinate
*
*/
	double getMinX() const{return minX;}
/** \brief Get the minimal y coordinate
*
*/
	double getMinY() const{return minY;}
/** \brief Get the minimal z coordinate
*
*/
	double getMinZ() const{return minZ;}
/** \brief Get the maximal x coordinate
*
*/
	double getMaxX() const{return maxX;}
/** \brief Get the maximal y coordinate
*
*/
	double getMaxY() const{return maxY;}
/** \brief Get the maximal z coordinate
*
*/
	double getMaxZ() const{return maxZ;}

/// @private
	void debug()
	{
		std::cout<<"Bbox3: ("<<minX<<","<<minY<<","<<minZ<<") -> ("<<maxX<<","<<maxY<<","<<maxZ<<")"<<std::endl;
	}

protected:
/// @private
	inline void treatPointForValidBox(const Point3& p)
	{
		double x,y,z;
		p.xyz(x,y,z);
		if(x<minX) minX=x;
			else if(x>maxX) maxX=x;
		if(y<minY) minY=y;
			else if(y>maxY) maxY=y;
		if(z<minZ) minZ=z;
			else if(z>maxZ) maxZ=z;
	}
	double minX,minY,minZ;
	double maxX,maxY,maxZ;


	friend std::ostream &operator<<(std::ostream &stream, Bbox3& pC);
};


inline std::ostream &operator<<(std::ostream &stream, Bbox3& pC)
{
	stream<<"Bbox3: ("<<pC.minX<<","<<pC.minY<<","<<pC.minZ<<") -> ("<<pC.maxX<<","<<pC.maxY<<","<<pC.maxZ<<"), rangeX="<<pC.getRangeX()<<", rangeY="<<pC.getRangeY()<<", rangeZ="<<pC.getRangeZ();
	return stream;
}


} // NAMESPACE FADE3D
