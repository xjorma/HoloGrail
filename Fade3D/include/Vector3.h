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
#include "common.h"

namespace FADE3D {

/** \brief 3D Vector
*/
class Vector3
{
public:
/** \brief Constructor
*/

	Vector3(const double x_,const double y_,const double z_);

/** \brief Constructor
* The vector is initialized to (0,0,0)
*/
	Vector3();


	/** \brief Get the x-value
	*/
	double x() const;

	/** \brief Get the y-value
	*/
	double y() const;

	/** \brief Get the z-value.
	*/
	double z() const;

	/** \brief Set the values
	*/
	void set(const double x_,const double y_,const double z_);

/** \brief Get the length of the vector
*/
	double length() const;

/** \brief Scalar product
*/
	double operator*(const Vector3& other) const;


/** \brief Multiply by a scalar value
*/
	Vector3 operator*(double val) const;


/** \brief Divide by a scalar value
*/
	Vector3 operator/(double val) const;

protected:
	double valX;
	double valY;
	double valZ;
};




// Free functions


inline std::ostream &operator<<(std::ostream &stream, const Vector3& vec)
{
	stream << "Vector3: "<<vec.x()<<", "<<vec.y()<<", "<<vec.z();
	return stream;
}

/** \brief Cross product
*/

inline Vector3 crossProduct(const Vector3& vec0,const Vector3& vec1)
{
	double x=vec0.y() * vec1.z() - vec0.z() * vec1.y();
	double y=vec0.z() * vec1.x() - vec0.x() * vec1.z();
	double z=vec0.x() * vec1.y() - vec0.y() * vec1.x();
	return Vector3(x,y,z);
}


/** \brief Normalize the vector
*/

inline Vector3 normalize(const Vector3& other)
{
	double len(other.length());
	if(len>0)
	{
		return Vector3(other.x()/len,other.y()/len,other.z()/len);
	}
	else
	{
		std::cout<<"warning: normalize(const Vector3& other), Null length vector!"<<std::endl;// COUTOK
		return Vector3(0,0,0);
	}
}


inline Vector3 operator-(const Vector3& in)
{
	return Vector3(-in.x(),-in.y(),-in.z());
}


inline Vector3 operator*(double d,const Vector3& vec)
{
	return Vector3(d*vec.x(),d*vec.y(),d*vec.z());
}


} // (namespace)
