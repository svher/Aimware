#pragma once

#include <cstdint>
#include <d3dx9.h>
#include "entity.h"

class Hack {
public:
    uintptr_t engine;
    uintptr_t client;

    Entity* localEntity;
    EntityList* entityList;
    float viewMatrix[16];

    Vec2 crosshair2D;
    int crosshairSize = 4;

    // Direct2D variables
    ID3DXLine* LineL;
    ID3DXFont* FontF;

    // Gadget for return address spoof
    void* clientGadget = NULL;

    bool stop = false;

    struct {
        IEngineTrace *EngineTrace = nullptr;
        IClientEntityList* ClientEntityList = nullptr;
        CHLClient* ChlClient = nullptr;
        IVEngineClient* EngineClient = nullptr;
        IInputSystem* InputSystem = nullptr;
        void* MatSystemSurface = nullptr;
        void* ClientModeShared = nullptr;
    } objs;

    ~Hack() {
        stop = true;
        if (LineL != nullptr) {
            LineL->Release();
            LineL = nullptr;
        }
        if (FontF != nullptr) {
            FontF->Release();
            FontF = nullptr;
        }
    }

    void Init();
    // update view matrix
    void Update();
    bool CheckValidEntity(Entity* entity) const;
    int GetLocalEntityId() const;
    bool WorldToScreen(Vec3 position, Vec2& screen);
    Vec3* GetViewAngles() const;
    // aimbot
    void Run(CUserCmd *cmd) const;
    void CheckButtons();

    struct Settings {
        bool showMenu = true;
        bool showTeammates = false;
        bool snapLines = false;
        bool box2D = false;
        bool statusText = true;
        bool box3D = false;
        bool rcsCrossHair = false;
        bool headline3D = false;
        bool velocityEsp = false;
        bool aimBot = false;
        bool bHop = true;
    } settings;

    struct Buttons {
        DWORD showMenuBtn = VK_INSERT;
        DWORD showTeammatesBtn = VK_F1;
        DWORD aimBotBtn = VK_F2;
        DWORD box2DBtn = VK_F3;
        DWORD box3DBtn = VK_F4;
        DWORD velocityEspBtn = VK_F5;
        DWORD headline3DBtn = VK_F6;
        DWORD rcsCrossHairBtn = VK_F7;
        DWORD snapLinesBtn = VK_F8;
    } buttons;

    struct Colors {
        D3DCOLOR health = D3DCOLOR_ARGB(255, 46, 139, 87);
        D3DCOLOR armor = D3DCOLOR_ARGB(255, 30, 144, 255);
        D3DCOLOR crosshair = D3DCOLOR_ARGB(255, 255, 255, 255);
        struct Team {
            D3DCOLOR esp = D3DCOLOR_ARGB(255, 255, 0, 0);
            D3DCOLOR snapline = D3DCOLOR_ARGB(255, 255, 0, 0);
            D3DCOLOR velocity = D3DCOLOR_ARGB(255, 0, 0, 255);
            D3DCOLOR headline = D3DCOLOR_ARGB(255, 0, 0, 255);
        } team;
        struct Enemy {
            D3DCOLOR esp = D3DCOLOR_ARGB(255, 0, 255, 0);
            D3DCOLOR snapline = D3DCOLOR_ARGB(255, 0, 255, 0);
            D3DCOLOR velocity = D3DCOLOR_ARGB(255, 0, 255, 255);
            D3DCOLOR headline = D3DCOLOR_ARGB(255, 0, 255, 255);
        } enemy;
    } colors;

    bool TraceRay(Entity *target) const;
};