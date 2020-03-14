#include <windows.h>
#include <utils\yutils.h>

using namespace YUtils;

HANDLE HandleThread;

static DWORD WINAPI
ThreadProc(LPVOID p)
{
    for(; ; )
    {
        OutputDebugPrintf("Stub Module Process Attached!\n");
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
        Init();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        Destroy();
        break;
    default:
        break;
    }

    return TRUE;
}
