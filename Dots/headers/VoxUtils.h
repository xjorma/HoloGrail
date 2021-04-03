#pragma once


inline int GetThreadsNumber(voxie_wind_t& _vw)
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return max((int)sysinfo.dwNumberOfProcessors - !_vw.useemu, 1);
}

inline int intCol(float v)
{
	int iv = (int)(v * 255.f);
	return (iv << 16) | (iv << 8) | iv;
}