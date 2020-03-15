#include <plugin\yinjection.h>

namespace YPlugIn
{

#define VIRTUALALLOC_PAGESIZE   (4096)

YRemoteThreadInjection::YRemoteThreadInjection
    (DWORD processId /*= 0*/, std::string modulePath /*= NULL*/)
    : _processId(processId), _modulePath(modulePath), _remoteModule(NULL)
{

}

YRemoteThreadInjection::~YRemoteThreadInjection()
{
    Detach();
}

int
YRemoteThreadInjection::Set(DWORD processId, std::string modulePath)
{
    if(_remoteModule)
        return Attached;

    _processId = processId;
    _modulePath = modulePath;

    return Succ;
}

void
YRemoteThreadInjection::Get(DWORD &processId, std::string &modulePath) const
{
    processId = _processId;
    modulePath = _modulePath;
}

int YRemoteThreadInjection::Attach()
{
    if(_remoteModule)
        return Attached;

    if(!_processId)
        return ProcessIdErr;
    if(_modulePath.empty())
        return ModulePathErr;

    // EnablePrivilege(TRUE);

    HANDLE handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, _processId);
    if(!handle)
        return OpenProcessErr;

    void *pRemoteModulePath = ::VirtualAllocEx(handle, NULL,
                                               VIRTUALALLOC_PAGESIZE,
                                               MEM_COMMIT, PAGE_READWRITE);
    if(!pRemoteModulePath)
    {
        ::CloseHandle(handle);
        return AllocRemoteMemErr;
    }

    if(!::WriteProcessMemory(handle, pRemoteModulePath, _modulePath.c_str(),
                             _modulePath.size() + 1, NULL))
    {
        ::VirtualFreeEx(handle, pRemoteModulePath, VIRTUALALLOC_PAGESIZE,
                        MEM_COMMIT);
        ::CloseHandle(handle);
        return WriteRemoteMemErr;
    }

    HANDLE hRemoteThread = ::CreateRemoteThread(
                                handle,
                                NULL,
                                0,
                                (LPTHREAD_START_ROUTINE)::LoadLibraryA,
                                pRemoteModulePath,
                                NULL,
                                NULL);
    if(!hRemoteThread)
    {
        ::VirtualFreeEx(handle, pRemoteModulePath, VIRTUALALLOC_PAGESIZE,
                        MEM_COMMIT);
        ::CloseHandle(handle);
        return CreateRemoteThdErr;
    }

    ::WaitForSingleObject(hRemoteThread, -1);

    DWORD exitCode;
    ::GetExitCodeThread(hRemoteThread, &exitCode);
    if(!exitCode)
    {
        ::CloseHandle(handle);
        return LoadLibraryErr;
    }

    _remoteModule = (HMODULE)exitCode;

    if(!::VirtualFreeEx(handle, pRemoteModulePath, VIRTUALALLOC_PAGESIZE,
                        MEM_DECOMMIT))
    {
        ::CloseHandle(handle);
        return FreeRemoteMemErr;
    }

    return Succ;
}

int YRemoteThreadInjection::Detach()
{
    if(!_remoteModule)
        return Detached;

    if(!_processId)
        return ProcessIdErr;

    // EnablePrivilege(TRUE);

    HANDLE handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, _processId);
    if(!handle)
        return OpenProcessErr;

    HANDLE hRemoteThread = ::CreateRemoteThread(
                                handle,
                                NULL,
                                0,
                                (LPTHREAD_START_ROUTINE)::FreeLibrary,
                                _remoteModule,
                                NULL,
                                NULL);
    if(!hRemoteThread)
    {
        ::CloseHandle(handle);
        return CreateRemoteThdErr;
    }

    ::WaitForSingleObject(hRemoteThread, -1);

    DWORD exitCode;
    ::GetExitCodeThread(hRemoteThread, &exitCode);
    if(!exitCode)
    {
        ::CloseHandle(handle);
        return FreeLibraryErr;
    }

    _remoteModule = NULL;
    ::CloseHandle(handle);

    return Succ;
}

}
