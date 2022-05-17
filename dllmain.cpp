// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "includes.h"
#include <sstream>

void* d3d9Device[119];
BYTE EndSceneByte[7]{ 0 };
tEndScene oEndScene = nullptr;
LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
    if (!pDevice) {
        pDevice = o_pDevice;
    }

    DrawTextWrapper("ESP HACK SUPER LEGIT", windowWidth / 2, windowHeight - 20, D3DCOLOR_ARGB(255, 255, 255, 255));
    int menuOffX = windowWidth / 2;
    int menuOffY = 50;
    D3DCOLOR enabled = D3DCOLOR_ARGB(255, 0, 255, 0);
    D3DCOLOR disabled = D3DCOLOR_ARGB(255, 255, 0, 0);

    if (hack->settings.showMenu) {

#define DRAW_SETTINGS(name, index, setting) DrawTextWrapper(name, menuOffX, menuOffY + (index) * 12, (setting) ? enabled : disabled)
        DRAW_SETTINGS("Show Teammates (F1)", 0, hack->settings.showTeammates);
        DRAW_SETTINGS("Aimbot (F2)", 1, hack->settings.aimBot);
        DRAW_SETTINGS("2D Box (F3)", 2, hack->settings.box2D);
        DRAW_SETTINGS("3D Box (F4)", 3, hack->settings.box3D);
        DRAW_SETTINGS("Velocity ESP (F5)", 4, hack->settings.velocityEsp);
        DRAW_SETTINGS("Headline ESP (F6)", 5, hack->settings.headline3D);
        DRAW_SETTINGS("Recoil Crosshair (F7)", 6, hack->settings.rcsCrossHair);
        DRAW_SETTINGS("Snaplines (F8)", 7, hack->settings.snapLines);
        DrawTextWrapper("Hide Menu (INS)", menuOffX, menuOffY + 8 * 12, D3DCOLOR_ARGB(255, 255, 255, 255));
    } else {
        DrawTextWrapper("Show Menu (INS)", menuOffX, menuOffY, D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    if (hack->localEntity != nullptr) {
        for (int i = 1; i < 32; i++) {
            Entity *entity = hack->entityList->entities[i].entity;
            if (!hack->CheckValidEntity(entity)) {
                continue;
            }
            D3DCOLOR espColor, snaplineColor, velocityColor, headlineColor;
            if (entity->iTeamNum == hack->localEntity->iTeamNum) {
                if (!hack->settings.showTeammates) {
                    continue;
                }
                espColor = hack->colors.team.esp;
                snaplineColor = hack->colors.team.snapline;
                velocityColor = hack->colors.team.velocity;
                headlineColor = hack->colors.team.headline;
            } else {
                espColor = hack->colors.enemy.esp;
                snaplineColor = hack->colors.enemy.snapline;
                velocityColor = hack->colors.enemy.velocity;
                headlineColor = hack->colors.enemy.headline;
            }
            Vec2 pos2D{};
            if (W2S(entity->vecOrigin, pos2D)) {
                if (hack->settings.snapLines) {
                    DrawLine(pos2D.x, pos2D.y, windowWidth / 2, windowHeight, 2, snaplineColor);
                }

                if (hack->settings.velocityEsp) {
                    Vec3 velOff = entity->vecOrigin + entity->vecVelocity * 0.25f;
                    Vec2 velOff2D;
                    if (W2S(velOff, velOff2D)) {
                        DrawLine(pos2D, velOff2D, 2, velocityColor);
                        DrawFilledRect(velOff2D.x - 2, velOff2D.y - 2, 4, 4, velocityColor);
                    }
                }

                Vec2 head2D{};
                Vec3 head3D = *entity->GetBonePos(8);
                head3D.z += 8;
                if (W2S(head3D, head2D)) {
                    if (hack->settings.box3D) {
                        DrawEspBox3D(head3D, entity->vecOrigin, entity->angEyeAnglesY, 25, 2, espColor);

                        if (hack->settings.statusText) {
                            std::stringstream s1, s2;
                            s1 << "hp: " << entity->iHealth;
                            s2 << "ap: " << entity->armorValue;
                            std::string t1 = s1.str();
                            std::string t2 = s2.str();
                            DrawTextWrapper(t1.c_str(), pos2D.x, pos2D.y, D3DCOLOR_ARGB(255, 255, 255, 255));
                            DrawTextWrapper(t2.c_str(), pos2D.x, pos2D.y + 12, D3DCOLOR_ARGB(255, 255, 255, 255));
                            if (entity->bHasHelmet) {
                                DrawTextWrapper("has helmet", pos2D.x, pos2D.y + 24, D3DCOLOR_ARGB(255, 255, 255, 255));
                            } else {
                                DrawTextWrapper("no helmet", pos2D.x, pos2D.y + 24, D3DCOLOR_ARGB(255, 255, 255, 255));
                            }
                        }
                    }

                    if (hack->settings.box2D) {
                        DrawEspBox2D(head2D, pos2D, 2, espColor);
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

                        DrawLine(topHealth, botHealth, 2, hack->colors.health);
                        DrawLine(topArmor, botArmor, 2, hack->colors.armor);
                    }

                    if (hack->settings.headline3D) {
                        head3D.z -= 8;
                        Vec3 angAngles{entity->angEyeAnglesX, entity->angEyeAnglesY, 0};
                        Vec3 endPoint = hack->TransformVec(head3D, angAngles, 60);
                        Vec2 endPoint2D;
                        if (W2S(endPoint, endPoint2D) && W2S(head3D, head2D)) {
                            DrawLine(head2D, endPoint2D, 2, headlineColor);
                        }
                        head3D.z += 8;
                    }
                }
            }
        }

        // crossHair
        if (hack->settings.rcsCrossHair) {
            Vec2 l{}, r{}, t{}, b{};
            l = r = t = b = hack->crosshair2D;
            l.x -= hack->crosshairSize;
            r.x += hack->crosshairSize;
            b.y += hack->crosshairSize;
            t.y -= hack->crosshairSize;

            DrawLine(l, r, 2, hack->colors.crosshair);
            DrawLine(t, b, 2, hack->colors.crosshair);
        }
    }

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
        hack->CheckButtons();
        if (hack->settings.aimBot) {
            hack->Run();
        }
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

