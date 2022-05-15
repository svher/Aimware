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
        Entity* entity = hack->entityList->entities[i].entity;
        if (!hack->CheckValidEntity(entity)) {
            continue;
        }
        D3DCOLOR color;
        if (entity->iTeamNum == hack->localEntity->iTeamNum) {
            color = D3DCOLOR_ARGB(255, 0, 255, 0);
        } else {
            color = D3DCOLOR_ARGB(255, 255, 0, 0);
        }
        Vec2 pos2D{};
        if (hack->WorldToScreen(entity->vecOrigin, pos2D)) {
            // snapLine
            // DrawLine(pos2D.x, pos2D.y, windowWidth / 2, windowHeight, 2, color);

            Vec2 head2D{};
            Vec3 entPos = *entity->GetBonePos(8);
            entPos.z += 8;
            if (hack->WorldToScreen(entPos, head2D)) {
                DrawEspBox2D(head2D, pos2D, 2, color);
                float height = ABS(head2D.y - pos2D.y);
                double dx = head2D.x - pos2D.x;
                double healthPerc = entity->iHealth / 100.f;
                double armorPerc = entity->armorValue / 100.f;

                Vec2 botHealth{}, topHealth{}, botArmor{}, topArmor{};
                float healthHeight = height * healthPerc;
                float armorHeight = height * armorPerc;

                botHealth.y = botArmor.y = pos2D.y;
                botHealth.x = pos2D.x + height / 4 + 2;
                botArmor.x = pos2D.x - height / 4 - 2;
                topHealth.y = pos2D.y - healthHeight;
                topArmor.y = pos2D.y - armorHeight;
                topHealth.x = botHealth.x + dx * healthPerc;
                topArmor.x = botArmor.x + dx * armorPerc;

                DrawLine(topHealth, botHealth, 2, D3DCOLOR_ARGB(255, 46, 139, 87));
                DrawLine(topArmor, botArmor, 2, D3DCOLOR_ARGB(255, 30, 144, 255));
            }
        }
    }

    // DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));
    // crossHair

    Vec2 l{}, r{}, t{}, b{};
    l = r = t = b = hack->crosshair2D;
    l.x -= hack->crosshairSize;
    r.x += hack->crosshairSize;
    b.y += hack->crosshairSize;
    t.y -= hack->crosshairSize;

    DrawLine(l, r, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
    DrawLine(t, b, 2, D3DCOLOR_ARGB(255, 255, 255, 255));

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

