#pragma once

#include <stddef.h>

class YInlineHook
{
public:
    enum RetCode
    {
        BaseProcErr         = -6,
        BaseProcMemProErr,
        HookMemProErr,
        HookSizeErr,
        ProcAddrErr,
        HookAddrErr,

        Succ                = 0,

        Loaded,
        Unloaded,
    };

public:
    YInlineHook(void   *hookAddr    = NULL,
                void   *procAddr    = NULL,
                size_t  hookSize    = 0);

    ~YInlineHook();

    void Set(void *hookAddr, void *procAddr, size_t hookSize = 5);
    void Get(void *&hookAddr, void *&procAddr, size_t &hookSize);
    int Load();
    int Unload();

protected:
    int LoadBaseProc();

protected:
    void   *_hookAddr;
    void   *_procAddr;
    size_t  _hookSize;

    void   *_baseProc;
};
