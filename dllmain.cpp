// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "includes.h"

void* d3d9Device[119];
BYTE EndSceneByte[7]{ 0 };
tEndScene oEndScene = nullptr;
LPDIRECT3DDEVICE9 pDevice = nullptr;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
    if (!pDevice) {
        pDevice = o_pDevice;
    }

    DrawFilledRect(25, 25, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));

    DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2 - 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));

    oEndScene(pDevice);
}

DWORD WINAPI DllAttach(HMODULE hModule) {
#ifdef _DEBUG
    AllocConsole();
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    SetConsoleTitleA("MyAimbot - DEBUG");
#endif

    if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
        memcpy(EndSceneByte, d3d9Device[42], 7);
        oEndScene = (tEndScene)TrampHook((BYTE*)d3d9Device[42], (BYTE*)hkEndScene, 7);
    }

    while(!GetAsyncKeyState(VK_END)) {
        Sleep(1);
    }

    Patch((BYTE*)d3d9Device[42], EndSceneByte, 7);

    FreeLibraryAndExitThread(hModule, 0);
}

VOID WINAPI DllDetach() {
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    HWND hwConsole = GetConsoleWindow();
    FreeConsole();
    PostMessage(hwConsole, WM_CLOSE, 0, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)DllAttach, hModule, 0, nullptr));
        break;
    case DLL_PROCESS_DETACH:
        DllDetach();
        break;
    default:
        break;
    }
    return TRUE;
}

