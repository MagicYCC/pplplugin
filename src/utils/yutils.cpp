#include <utils\yutils.h>

namespace YUtils
{

void OutputDebugPrintfA(const char *strOutputString, ...)
{
#define PUT_DEBUG_BUF_LEN   4096
    char strBuffer[PUT_DEBUG_BUF_LEN] = { 0 };
    va_list vlArgs;
    va_start(vlArgs, strOutputString);
    _vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
    va_end(vlArgs);
    OutputDebugStringA(strBuffer);
}

}
