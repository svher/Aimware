﻿#include "includes.h"
#include "thirdparty/x86RetSpoof.h"
#include <iostream>

using CreateMoveFn = bool(__thiscall*)(void*, float, CUserCmd*);

void* d3d9Device[119];
Hack* hack;

EndSceneFn oEndScene = nullptr;
CreateMoveFn oCreateMove = nullptr;
BYTE EndSceneByte[7]{ 0 };
BYTE CreateMoveByte[9] {0};

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice);
bool __fastcall hkCreateMove(void* ecx, void* edx, float frameTime, CUserCmd* cmd) {
    bool result = x86RetSpoof::invokeThiscall<bool>((std::uintptr_t)ecx, (std::uintptr_t)oCreateMove, (std::uintptr_t)hack->gadget, frameTime, cmd);
    if (!cmd || !cmd->command_number) {
        return result;
    }
    if (hack->settings.aimBot) {
        hack->Run(cmd);
    }
    if (hack->settings.bHop && hack->localEntity) {
        /* BE AWARE OF Pointer Arithmetic
         *
         * If the player is not on ground, don't press the jump key */
        if (!(hack->localEntity->fFlags & FL_ONGROUND)) {
            cmd->buttons &= ~IN_JUMP;
        }
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


    BYTE* createMovePtr = (*static_cast<BYTE***>(hack->clientMode))[24];
    memcpy(CreateMoveByte, createMovePtr, 9);
    /* cannot be len of 7
    * +06 | FF 8B 0E E8 72 DB dec dword [ebx-0x248D17F2]
    *
    * We need to bypass Valve return address check, otherwise you won't be able to join a team
    */
    oCreateMove = (CreateMoveFn) TrampHook(createMovePtr, (BYTE*)hkCreateMove, 9);

    while(!GetAsyncKeyState(VK_END)) {
        hack->Update();
        hack->CheckButtons();
    }

    Patch((BYTE*)d3d9Device[42], EndSceneByte, 7);
    Patch(createMovePtr, CreateMoveByte, 9);

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
        DisableThreadLibraryCalls(hModule);
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

