#ifndef _Utilities_H_
#define _Utilities_H_

#include <time.h>
//for wait
#include <Windows.h>
//for _mkdir
#include <direct.h>
#include <Shlwapi.h>	//PathIsDirectory‚É•K—v

#include "Random.hpp"

#include <gl/GL.h>

int fieldGenerator(int* field);
std::string logPathGenerator();
std::string intToString5(int num);
std::wstring s2ws(const std::string& s);

#endif	//_Utilities_H_