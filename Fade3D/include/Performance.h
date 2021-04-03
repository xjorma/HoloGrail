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
#include <string>

namespace FADE3D {


/**  \brief Timer
*
* Call the timer function with a certain string to start time
* measurement. Call it a second time with the same string to
* finish time measurement.
*
* @return -1 when the timer is started or the elapsed time in
* seconds when the timer is stopped.
*/
CLASS_DECLSPEC
double timer(const std::string& str);

} // (namespace)






