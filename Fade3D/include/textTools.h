
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
#include <sstream>
#include <string>
#include <iostream>

/// @private
void warning(const std::string& who,const std::string& what);
/// @private
void error(const std::string& who,const std::string& what);
/// @private
void info(const std::string& who,const std::string& what);
/// @private
void fatBar(const std::string& name,bool em=false,bool newLines=false);
/// @private
std::string redString(const std::string& s,int em=0);
/// @private
std::string greenString(const std::string& s,int em=0);
/// @private
std::string magentaString(const std::string& s,int em=0);
/// @private
std::string yellowString(const std::string& s,int em=0);
/// @private
std::string globalPrefix();


template<class T_InType>
std::string toString(const T_InType& in)
{
	std::ostringstream oss;
	oss << in;
	return oss.str();
}

// Switch to red (and not back again)
inline std::string red()
{
#ifndef _WIN32
        return ("\033[0;31m");
#else
        return std::string("");
#endif
}

inline std::string green()
{
#ifndef _WIN32
        return ("\033[0;32m");
#else
        return std::string("");
#endif
}

inline std::string yellow()
{
#ifndef _WIN32
        return ("\033[0;33m");
#else
        return std::string("");
#endif
}

inline std::string magenta()
{
#ifndef _WIN32
        return ("\033[0;35m");
#else
        return std::string("");
#endif
}
// Switch to white (and not back again)
inline std::string white()
{
#ifndef _WIN32
        return ("\033[m");
#else
        return std::string("");
#endif
}


