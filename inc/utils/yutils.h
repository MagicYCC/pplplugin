#pragma once

#include <windows.h>
#include <stdio.h>

#define YUTILS_CALL __stdcall

namespace YUtils
{

#define Items(a)   (sizeof(a)/sizeof(a[0]))

#ifdef _DEBUG
#define OutputDebugPrintf(str)    OutputDebugPrintfA(str)
#else
#define OutputDebugPrintf(str)
#endif

void OutputDebugPrintfA(const char *strOutputString, ...);

}

