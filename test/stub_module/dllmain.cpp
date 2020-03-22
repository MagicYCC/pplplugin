#include <windows.h>
#include <utils\yutils.h>

using namespace YUtils;

HANDLE HandleThread;

static DWORD WINAPI
ThreadProc(LPVOID p)
{
    for(; ; )
    {
        OutputDebugPrintf("Stub Module Thread Execute!\n");
        Sleep(2000);
    }
}

static void
Init()
{
    DWORD threadId;
    HandleThread = ::CreateThread(NULL, 0, ThreadProc, 0, 0, &threadId);
}

static void
Destroy()
{
    ::TerminateThread(HandleThread, 2);
}

BOOL WINAPI
DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugPrintf("Stub Module Process Attached!\n");
        Init();
        break;
    case DLL_THREAD_ATTACH: break;
    case DLL_THREAD_DETACH: break;
    case DLL_PROCESS_DETACH:
        OutputDebugPrintf("Stub Module Process Detached!\n");
        Destroy();
        break;
    default:
        break;
    }

    return TRUE;
}
