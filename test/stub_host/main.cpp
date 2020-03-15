#include <windows.h>
#include <utils\yutils.h>

using namespace YUtils;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND        hWnd;
    MSG         msg;
    WNDCLASS    wndClass;

    wndClass.style          = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc    = WndProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName   = NULL;
    wndClass.lpszClassName  = TEXT("StubHost");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("StubHost"),       // window class name
        TEXT("Stub Host"),      // window caption
        WS_OVERLAPPEDWINDOW,    // window style
        CW_USEDEFAULT,          // initial x position
        CW_USEDEFAULT,          // initial y position
        CW_USEDEFAULT,          // initial x size
        CW_USEDEFAULT,          // initial y size
        NULL,                   // parent window handle
        NULL,                   // window menu handle
        hInstance,              // program instance handle
        NULL);                  // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        OutputDebugPrintf("StubHost WM_CREATE!\n");
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
