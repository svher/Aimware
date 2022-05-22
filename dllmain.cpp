// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "includes.h"

using CreateMoveFn = bool(__thiscall*)(void*, float, void*);

void* d3d9Device[119];
Hack* hack;

EndSceneFn oEndScene = nullptr;
CreateMoveFn oCreateMove = nullptr;
BYTE EndSceneByte[7]{ 0 };

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice);
bool __fastcall hkCreateMove(void* ecx, void* edx, float frameTime, void* cmd) {
    bool result = oCreateMove(ecx, frameTime, cmd);
    if (GetAsyncKeyState(VK_HOME) & 1) {
        hack->TraceRay();
    }
    return result;
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
        oEndScene = (EndSceneFn)TrampHook((BYTE*)d3d9Device[42], (BYTE*)hkEndScene, 7);
    }

    hack = new Hack();
    hack->Init();

    MH_Initialize();
    MH_CreateHook((*static_cast<void***>(hack->clientMode))[24], (LPVOID)hkCreateMove, (LPVOID*)&oCreateMove);
    MH_EnableHook(MH_ALL_HOOKS);

    while(!GetAsyncKeyState(VK_END)) {
        hack->Update();
        hack->CheckButtons();
        if (hack->settings.aimBot) {
            hack->Run();
        }
    }

    Patch((BYTE*)d3d9Device[42], EndSceneByte, 7);
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    FreeLibraryAndExitThread(hModule, 0);
}

VOID WINAPI DllDetach() {
    delete(hack);
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
    HANDLE handle;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)DllAttach, hModule, 0, nullptr);
        if (handle != nullptr) {
            CloseHandle(handle);
        }
        break;
    case DLL_PROCESS_DETACH:
        DllDetach();
        break;
    default:
        break;
    }
    return TRUE;
}

