#include <utils\yprocess.h>
#include <Psapi.h>
#include <cstring>

namespace YUtils
{
using namespace std;

#define PROCESS_MAX_COUNT   (1024)

void YUTILS_CALL
ProcessIdsByBaseName(string name, vector<DWORD> &processIds)
{
    processIds.clear();

    DWORD aProcesses[PROCESS_MAX_COUNT], cbNeeded, cProcesses;
    if (!::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    for(size_t i = 0; i < cProcesses; i++)
        if(aProcesses[i])
        {
            string nameTmp;
            ProcessBaseNameByPid(aProcesses[i], nameTmp);
            if(name == nameTmp)
                processIds.push_back(aProcesses[i]);
        }
}

void YUTILS_CALL
ProcessIdByBaseName(string name, DWORD &processId)
{
    processId = 0;

    DWORD aProcesses[PROCESS_MAX_COUNT], cbNeeded, cProcesses;
    if (!::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    for(size_t i = 0; i < cProcesses; i++)
        if(aProcesses[i])
        {
            string nameTmp;
            ProcessBaseNameByPid(aProcesses[i], nameTmp);
            if(name == nameTmp)
            {
                processId = aProcesses[i];
                break;
            }
        }

}

void YUTILS_CALL
ProcessBaseNameByPid(DWORD processId, string &baseName)
{
    baseName.clear();
    char szProcessName[MAX_PATH] = { 0 };

    // Get a handle to the process.
    HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processId);

    // Get the process name.
    if(hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;
        if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod),
                                 &cbNeeded))
        {
            ::GetModuleBaseNameA(hProcess, hMod, szProcessName,
                                 sizeof(szProcessName)/sizeof(TCHAR));
        }
    }

    baseName = szProcessName;
    ::CloseHandle(hProcess);
}

void YUTILS_CALL
ProcessNameByPid(DWORD processId, string &name)
{
    name.clear();
    char szProcessName[MAX_PATH] = { 0 };

    // Get a handle to the process.
    HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processId);

    // Get the process name.
    if(hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;
        if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod),
                                 &cbNeeded))
        {
            ::GetModuleFileNameExA(hProcess, hMod, szProcessName,
                                   sizeof(szProcessName)/sizeof(TCHAR));
        }
    }

    name = szProcessName;
    ::CloseHandle(hProcess);
}

}
