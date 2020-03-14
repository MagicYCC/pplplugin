#include <windows.h>
#include <utils\yutils.h>
#include <string>
#include <vector>

namespace YUtils
{
void YUTILS_CALL
ProcessIdsByName(std::string name, std::vector<DWORD> &processIds);

void YUTILS_CALL
ProcessIdByName(std::string name, DWORD &processId);

void YUTILS_CALL
ProcessBaseNameByPid(DWORD processId, std::string &baseName);

void YUTILS_CALL
ProcessNameByPid(DWORD processId, std::string &name);

}
