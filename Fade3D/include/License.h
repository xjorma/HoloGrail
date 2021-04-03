
// (c) 2010 Geom e.U. Bernhard Kornberger, Graz/Austria. All rights reserved.
//
// This file is part of the Fade3D library. You can use it for your personal
// non-commercial, non-military research. Don't redistribute without explicit
// permission. 

// This software is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Please contact the author if any conditions of this licensing are not clear 
// to you.
// 
// Author: Bernhard Kornberger, bkorn (at) geom.at 
//         C++ Freelancer
// http://www.geom.at/products/fade3d/

//
// * Please remember to put a link to Fade on your research website *
//


#pragma once 
#include "Fade_3D.h"

namespace{

struct License
{
	License()
	{
		FADE3D::setLic(
		"[NonCommercialResearch]",
		"[LicType,student],[3D,1e6]",
		"[LF:F/C]",
		"5303992d",
		"6505fb34");
	}
};
License lic;
}
