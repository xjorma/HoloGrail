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

/** @brief Generate random numbers
 *
 * @param num Number of random numbers to be generated
 * @param min Lower bound
 * @param max Upper bound
 * @param [out] vRandomNumbersOut is the output vector
 * @param seed initializes the random number generator RNG
 * (default: 0...mapped to a random seed, other values: constant initialization)
 *
 * \note Reproducable random numbers are often desirable when software
 * is tested with random geometric constructions. Thus each seed value
 * different from \e 0 leads to its own, reproducible, output sequence.
 * In contrast the \p seed value \e 0 is mapped to random initialization
 * of the RNG. In this case the RNG will produce a different output
 * sequence each time it is called.
 */
CLASS_DECLSPEC
void generateRandomNumbers(	size_t num,
							double min,
							double max,
							std::vector<double>& vRandomNumbersOut,
							unsigned int seed=0
							);

/** @brief Generate random points
 *
 * @param numRandomPoints Number of points to be generated
 * @param min Lower bound (x,y,z)
 * @param max Upper bound (x,y,z)
 * @param [out] vRandomPointsOut is the output vector
 * @param seed initializes the random number generator RNG
 * (default: 0...mapped to a random seed, other values: constant initialization)
 *
*/
CLASS_DECLSPEC
void generateRandomPoints(	size_t numRandomPoints,
							double min,
							double max,
							std::vector<Point3>& vRandomPointsOut,
							unsigned int seed=0
						);



} // Namespace
