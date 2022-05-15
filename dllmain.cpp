// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "includes.h"

void* d3d9Device[119];
BYTE EndSceneByte[7]{ 0 };
tEndScene oEndScene = nullptr;
LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
    if (!pDevice) {
        pDevice = o_pDevice;
    }

    for (int i = 1; i < 32; i++) {
        Entity* currentEntity = hack->entityList->entities[i].entity;
        if (!hack->CheckValidEntity(currentEntity)) {
            continue;
        }
        D3DCOLOR color;
        if (currentEntity->iTeamNum == hack->localEntity->iTeamNum) {
            color = D3DCOLOR_ARGB(255, 0, 255, 0);
        } else {
            color = D3DCOLOR_ARGB(255, 255, 0, 0);
        }
        Vec2 pos2D{};
        if (hack->WorldToScreen(currentEntity->vecOrigin, pos2D)) {
            DrawLine(pos2D.x, pos2D.y, windowWidth / 2, windowHeight, 2, color);
        }
    }

    DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));

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

    hack = new Hack();
    hack->Init();

    while(!GetAsyncKeyState(VK_END)) {
        hack->Update();
        hack->Run();
    }

    Patch((BYTE*)d3d9Device[42], EndSceneByte, 7);

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

