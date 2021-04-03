#pragma once
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <set>
#include <math.h>
#include <string.h>
#include <sstream>
#include <float.h>


struct FadeException: public std::exception
{
  virtual const char* what() const throw()
  {
	return "\n  FADE EXCEPTION due to a bug _or_ wrong usage of Fade. If you think it is a \n  bug then please report this incident and it will be fixed quickly: bkorn@geom.at";
  }
};
struct FadeLicenseException: public std::exception
{
  virtual const char* what() const throw()
  {
	return "Fade: Your program used an unlicensed feature, sorry.";
  }
};
