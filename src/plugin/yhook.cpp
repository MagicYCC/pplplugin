#include <Windows.h>
#include <string.h>
#include <plugin\yhook.h>

#pragma pack(push, 1)

struct BaseProcCallScene
{
    BYTE    _pushad;    // 60
    BYTE    _pushfd;    // 9C
    BYTE    _call;      // E8
    DWORD   _proc;
    BYTE    _popfd;     // 9D
    BYTE    _popad;     // 61
};

struct BaseProcRetJump
{
    BYTE    _jmp;       // E9
    DWORD   _proc;
};

#pragma pack(pop)

#define SizeOfBasePorc(hookSize) \
(sizeof(BaseProcCallScene) + hookSize + sizeof(BaseProcRetJump))

#define BaseProcCallAddr(baseAddr) \
((BaseProcCallScene *)baseAddr)

#define BaseProcPatchAddr(baseAddr) \
((BYTE *)baseAddr + sizeof(BaseProcCallScene))

#define BaseProcRetJumpAddr(baseAddr, hookSize) \
((BaseProcRetJump *)((BYTE *)baseAddr + sizeof(BaseProcCallScene) + hookSize))

YInlineHook::YInlineHook(void *hookAddr/* = NULL*/, void *procAddr/* = NULL*/,
                        size_t hookSize/* = 0*/)
    : _hookAddr(hookAddr), _procAddr(procAddr), _hookSize(hookSize),
    _baseProc(NULL) { }

YInlineHook::~YInlineHook()
{
    Unload();
}

void YInlineHook::Set(void *hookAddr, void *procAddr, size_t hookSize/* = 5*/)
{
    _hookAddr   = hookAddr;
    _procAddr   = procAddr;
    _hookSize   = hookSize;
}

void YInlineHook::Get(void *&hookAddr, void *&procAddr, size_t &hookSize)
{
    hookAddr    = _hookAddr;
    procAddr    = _procAddr;
    hookSize    = _hookSize;
}

int YInlineHook::Load()
{
    if(_baseProc)
        return Loaded;

    if(!_hookAddr)
        return HookAddrErr;
    if(!_procAddr)
        return ProcAddrErr;
    if(_hookSize < 5)
        return HookSizeErr;

    DWORD oldProtect;
    if(!::VirtualProtect(_hookAddr,
                        _hookSize,
                        PAGE_EXECUTE_READWRITE,
                        &oldProtect))
        return HookMemProErr;

    // _basePorc函数内存分配及内存保护类型设置
    size_t baseProcSize = SizeOfBasePorc(_hookSize);
    _baseProc = malloc(baseProcSize);
    if(!::VirtualProtect(_baseProc,
                        baseProcSize,
                        PAGE_EXECUTE_READWRITE,
                        &oldProtect))
    {
        if(_baseProc)
        {
            free(_baseProc);
            _baseProc = NULL;
        }
        return BaseProcMemProErr;
    }

    // _baseProc加载(不包含patch)
    LoadBaseProc();

    // _hookAddr原始内容保存
    void *patch = BaseProcPatchAddr(_baseProc);
    memcpy(patch, _hookAddr, _hookSize);

    // _hookAddr赋值
    DWORD jmpAddr = (DWORD)_baseProc - (DWORD)_hookAddr - 5;
    memset(_hookAddr, 0x90, _hookSize);
    *(BYTE *)_hookAddr                  = 0xE9;
    *(DWORD *)((BYTE*)_hookAddr + 1)    = jmpAddr;

    return Succ;
}

int YInlineHook::Unload()
{
    if(!_baseProc)
        return Unloaded;

    if(!_hookAddr)
        return HookAddrErr;
    if(!_procAddr)
        return ProcAddrErr;
    if(_hookSize < 5)
        return HookSizeErr;

    DWORD oldProtect;
    if(!::VirtualProtectEx(::GetCurrentProcess(),
                            _hookAddr,
                            _hookSize,
                            PAGE_EXECUTE_WRITECOPY,
                            &oldProtect))
        return HookMemProErr;

    void *patch = BaseProcPatchAddr(_baseProc);
    memcpy(_hookAddr, patch, _hookSize);

    free(_baseProc);
    _baseProc = NULL;

    return Succ;
}

int YInlineHook::LoadBaseProc()
{
    if(!_baseProc)
        return BaseProcErr;

    BaseProcCallScene  *pCallScene  = BaseProcCallAddr(_baseProc);
    BaseProcRetJump    *pRetJmp     = BaseProcRetJumpAddr(_baseProc, _hookSize);

    // 计算call和jmp的地址
    DWORD callAddr  = (DWORD)_procAddr - (DWORD)(&pCallScene->_call) - 5;
    DWORD jmpAddr   = (DWORD)_hookAddr + _hookSize - (DWORD)(&pRetJmp->_jmp) - 5;

    pCallScene->_pushad = 0x60;
    pCallScene->_pushfd = 0x9C;
    pCallScene->_call   = 0xE8;
    pCallScene->_proc   = callAddr;
    pCallScene->_popfd  = 0x9D;
    pCallScene->_popad  = 0x61;
    pRetJmp->_jmp       = 0xE9;
    pRetJmp->_proc      = jmpAddr;

    return Succ;
}
