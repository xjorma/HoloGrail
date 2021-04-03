#pragma once
#include <assert.h>


// DLL IMPORT/EXPORT MACRO
#if defined (_WIN32)
	#if defined(FADE3D_EXPORT)
		#define  CLASS_DECLSPEC __declspec(dllexport)
	#else
		#define  CLASS_DECLSPEC __declspec(dllimport)
	#endif
#else
	#define CLASS_DECLSPEC
#endif

const int INVALID_IDX(-999);

/** brief Check if an index is in the range [0...3] (debug function)
*/
inline bool range03(int i)
{
	if(i>-1 && i<4) return true;
		else return false;
}

/** brief Check if an index is in the range [0...2] (debug function)
*/
inline bool range02(int i)
{
	if(i>-1 && i<3) return true;
		else return false;
}
