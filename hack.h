#pragma once

#include "includes.h"
#include "csgo.h"
#include <cstdint>

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;};

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

class Entity {
public:
    union {
        // is player far away?
        DEFINE_MEMBER_N(bool, isDormant, m_bDormant);
        DEFINE_MEMBER_N(int, iHealth, m_iHealth);
        DEFINE_MEMBER_N(Vec3, vecOrigin, m_vecOrigin);
        DEFINE_MEMBER_N(int, iTeamNum, m_iTeamNum);
        // where the head is?
        DEFINE_MEMBER_N(uint32_t, dwBoneMatrix, m_dwBoneMatrix);
        // player view from origin
        DEFINE_MEMBER_N(Vec3, vecViewOffset, m_vecViewOffset)
        DEFINE_MEMBER_N(int, armorValue, m_ArmorValue);
        // recoil
        DEFINE_MEMBER_N(Vec3, aimPunchAngle, m_aimPunchAngle);
        DEFINE_MEMBER_N(float, angEyeAnglesX, m_angEyeAnglesX);
        DEFINE_MEMBER_N(float, angEyeAnglesY, m_angEyeAnglesY);
        DEFINE_MEMBER_N(Vec3, vecVelocity, m_vecVelocity);
        DEFINE_MEMBER_N(bool, bHasHelmet, m_bHasHelmet);
        DEFINE_MEMBER_N(uint32_t, bSpottedByMask, m_bSpottedByMask);
    };
    Vec3* GetBonePos(int boneId) const;
};

class EntityListObject {
public:
    struct Entity* entity;
    BYTE padding[12];
};

class EntityList {
public:
    EntityListObject entities[32];
};

class Hack {
public:
    uintptr_t engine;
    uintptr_t client;

    Entity* localEntity;
    EntityList* entityList;
    float viewMatrix[16];

    Vec2 crosshair2D;
    int crosshairSize = 4;

    ID3DXLine* LineL;
    ID3DXFont* FontF;

    bool stop = false;

    CreateInterfaceFn createInterface;
    IEngineTrace *engineTrace;

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
    void AimAt(Vec3 *target);
    Vec3* GetViewAngles() const;
    Vec3 TransformVec(Vec3 src, Vec3 dst, float distance);
    // aimbot
    void Run();
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

    void TraceRay();
};