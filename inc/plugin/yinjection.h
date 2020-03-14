#pragma once

#include <windows.h>
#include <string>

namespace YPlugIn
{

class YRemoteThreadInjection
{
public:
    enum RetCode
    {
        FreeLibraryErr      = -9,
        LoadLibraryErr,
        CreateRemoteThdErr,
        FreeRemoteMemErr,
        WriteRemoteMemErr,
        AllocRemoteMemErr,
        OpenProcessErr,
        ModulePathErr,
        ProcessIdErr,

        Succ                = 0,

        Attached,
        Detached,
    };

public:
    YRemoteThreadInjection(DWORD processId = 0, std::string modulePath = "");
    ~YRemoteThreadInjection();

    int Set(DWORD processId, std::string modulePath);
    void Get(DWORD &processId, std::string &modulePath) const;
    int Attach();
    int Detach();
protected:
    DWORD       _processId;
    // HANDLE      _handle;
    std::string _modulePath;

    // void       *_pRemoteModulePath;
    HMODULE     _remoteModule;
};

}
