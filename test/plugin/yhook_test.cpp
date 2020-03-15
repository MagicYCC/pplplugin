#include <windows.h>
#include <stdio.h>
#include <plugin\yhook.h>
#include "yhook_test.h"

using namespace YPlugIn;

static int Plus(int x, int y);

// 从Plus + 3处开始hook
extern "C" __declspec(naked) void YInlineHookPorc()
{
    printf("Enter YInlineHookPorc!\n");
    _asm    ret
}

// 从Plus + 0x0C处开始hook
extern "C" __declspec(naked) void YInlineHookPorc2()
{
    printf("Enter YInlineHookPorc2!\n");
    _asm    ret
}

// Plus = ::GetModuleHandle(NULL) + 0x000536E0
static int Plus(int x, int y)
{
    printf("Enter Plus!\n");
    return x + y;
}

void YInlineHookTest()
{
    printf("Inline Hook Test!\n");

    YInlineHook yInlineHook = YInlineHook();
    yInlineHook.Set((BYTE *)::GetModuleHandle(NULL) + 0x000536E3/*Plus + 3*/,
                    YInlineHookPorc, 6);

    YInlineHook *yInlineHook2 = new YInlineHook();
    yInlineHook2->Set((BYTE *)::GetModuleHandle(NULL) + 0x000536EC/*Plus + 0x0C*/,
                      YInlineHookPorc2, 6);
    Plus(1, 2);

    // Hook Puls() + 3
    yInlineHook.Load();
    Plus(1, 2);

    // Hook2 Puls() + 0x0C
    yInlineHook2->Load();
    Plus(1, 2);

    // 卸载Hook
    yInlineHook.Unload();
    Plus(1, 2);

    // 卸载Hook2
    yInlineHook2->Unload();
    Plus(1, 2);

    delete yInlineHook2;

    printf("Inline Hook Test End!\n");
}
