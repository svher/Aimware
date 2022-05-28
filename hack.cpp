#include "includes.h"
#include "x86RetSpoof.h"

void Hack::Init() {
    client = (uintptr_t)GetModuleHandle("client.dll");
    engine = (uintptr_t)GetModuleHandle("engine.dll");
    entityList = (EntityList*)(client + dwEntityList);
    // Interfaces
    objs.EngineTrace = GetInterface<IEngineTrace>(INTERFACEVERSION_ENGINETRACE_CLIENT, (HMODULE)engine);
    objs.ClientEntityList = GetInterface<IClientEntityList>("VClientEntityList003", (HMODULE)client);
    objs.ChlClient = GetInterface<CHLClient>("VClient018", (HMODULE)client);
    objs.EngineClient = GetInterface<IVEngineClient>("VEngineClient014", (HMODULE)engine);
    objs.MatSystemSurface = GetInterface<void>("VGUI_Surface031", GetModuleHandle("vguimatsurface.dll"));
    objs.InputSystem = GetInterface<IInputSystem>("InputSystemVersion001", GetModuleHandle("inputsystem.dll"));
    // 5 bytes from the fifth vtable function entrance is global var g_pClientMode[MAX_SPLITSCREEN_PLAYERS]
    // CHLClient - << HudProcessInput >>
    void **g_pClientMode = *reinterpret_cast<void***>((*reinterpret_cast<uintptr_t**>(objs.ChlClient))[10] + 5);
    objs.ClientModeShared = g_pClientMode[0];
    SetupNetvars();
    // search FF 23 in execution page
    // aka: jmp dword ptr [ebx]
    clientGadget = Memory::PatternScan((HMODULE)client, "FF 23 F8 F6 87 B1 03 00 00 02");
    // start the loop
    stop = false;
}

void Hack::Update() {
    memcpy(&viewMatrix, (PBYTE*)(client + dwViewMatrix), sizeof(viewMatrix));
    localEntity = *(Entity**)(client + dwLocalPlayer);
    if (localEntity != nullptr) {
        crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * localEntity->aimPunchAngle.y);
        crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * localEntity->aimPunchAngle.x);
    } else {
        crosshair2D.x = windowWidth / 2;
        crosshair2D.y = windowHeight / 2;
    }
}

bool Hack::CheckValidEntity(Entity *entity) const {
    if (entity == nullptr) {
        return false;
    }
    if (entity == localEntity) {
        return false;
    }
    if (entity->iHealth < 1) {
        return false;
    }
    if (entity->isDormant) {
        return false;
    }
    return true;
}

bool Hack::WorldToScreen(Vec3 position, Vec2 &screen) {
    Vec4 clipcoords{};
    clipcoords.x = position.x * viewMatrix[0] + position.y * viewMatrix[1] + position.z * viewMatrix[2] + viewMatrix[3];
    clipcoords.y = position.x * viewMatrix[4] + position.y * viewMatrix[5] + position.z * viewMatrix[6] + viewMatrix[7];
    clipcoords.z = position.x * viewMatrix[8] + position.y * viewMatrix[9] + position.z * viewMatrix[10] + viewMatrix[11];
    clipcoords.w = position.x * viewMatrix[12] + position.y * viewMatrix[13] + position.z * viewMatrix[14] + viewMatrix[15];

    // enemy behinds us
    if (clipcoords.w < 0.1f) {
        return false;
    }

    Vec3 NDC{};
    NDC.x = clipcoords.x / clipcoords.w;
    NDC.y = clipcoords.y / clipcoords.w;
    NDC.z = clipcoords.z / clipcoords.w;

    screen.x = (windowWidth / 2.f * NDC.x) + (windowWidth / 2.f + NDC.x);
    screen.y = -(windowHeight / 2.f * NDC.y) + (windowHeight / 2.f + NDC.y);
    return true;
}

void Hack::AdjustViewAngle(CUserCmd *cmd) const {
    // if neither feature is enabled, skip function
    if (!settings.aimBot && !settings.recoilControl) {
        return;
    }
    // skip if local player not exists
    if (localEntity == nullptr) {
        return;
    }
    // don't set viewangle when fakeAim is enabled
    if (settings.fakeAim && !!!(cmd->buttons & IN_ATTACK) && !settings.autoFire) {
        return;
    }
    float closetFOV = 10000;
    // DON'T USE GetViewAngles() HERE
    // if CreateMove returns true, then viewangles will be overridden by cmd->viewangles
    Vec3 closetDeltaAngle{};
    Vec3 *viewAngle = &cmd->viewangles;
    bool hasEnemy = false;
    if (settings.aimBot) {
        for (int i = 1; i < 32; i++) {
            Entity *entity = entityList->entities[i].entity;
            // Entity* entity = clientEntityList->GetClientEntity(i+1);
            if (!CheckValidEntity(entity)) {
                continue;
            }
            if (!settings.friendlyFire && entity->iTeamNum == localEntity->iTeamNum) {
                continue;
            }
            // don't shot if player immune
            if (entity->bGunGameImmunity) {
                continue;
            }
            entity->bSpotted = true;
            // slow but work in non-game thread
            // if (!(entity->bSpottedByMask & (1 << localEntityId))) {
            if (!TraceRay(entity)) {
                continue;
            }
            Vec3 localViewPos = localEntity->vecViewOffset + localEntity->vecOrigin;
            Vec3 deltaAngle = localViewPos.CalcAngles(*entity->GetBonePos(8)) - *viewAngle;
            if (deltaAngle.Norm2D() < closetFOV) {
                hasEnemy = true;
                closetDeltaAngle = deltaAngle;
            }
        }
    }
    Vec3 newAngle = *viewAngle;
    bool shouldAim = hasEnemy && ABS(closetDeltaAngle.y) <= settings.aimFovY && ABS(closetDeltaAngle.x) <= settings.aimFovX;
    if (settings.autoFire) {
        static bool firing = false;
        if (shouldAim) {
            // TODO: Need a rate limiter
            if (firing) {
                cmd->buttons &= ~IN_ATTACK;
            } else if (!firing) {
                cmd->buttons |= IN_ATTACK;
            }
            firing = !firing;
        } else if (firing) {
            firing = false;
            cmd->buttons &= ~IN_ATTACK;
        }
    }
    if (settings.recoilControl) {
        static Vec3 oPunchAngle;
        Vec3 punchAngle = localEntity->aimPunchAngle * 2.f;
        if (cmd->buttons & IN_ATTACK) {
            // REFERENCE: https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Recoil_Compensation
            // refer to weapon_recoil_scale in console
            // full or incremental calculation?
            if (settings.fakeAim || shouldAim) {
                newAngle = newAngle - punchAngle;
            } else {
                newAngle = newAngle + oPunchAngle - punchAngle;
            }
        }
        oPunchAngle = punchAngle;
    }
    if (shouldAim) {
        newAngle = newAngle + closetDeltaAngle;
    }
    if (newAngle.x >= -89 && newAngle.x <= 89 && newAngle.y >= -180 && newAngle.y <= 180) {
        // why set dwClientState_ViewAngles not work in CreateMove?
        viewAngle->x = newAngle.x;
        viewAngle->y = newAngle.y;
    }
}

bool Hack::TraceRay(Entity* target) const {
    CGameTrace trace{};
    Ray_t ray{};
    CTraceFilter traceFilter;
    traceFilter.pSkip = localEntity;
    ray.Init(localEntity->vecOrigin + localEntity->vecViewOffset, *target->GetBonePos(8));
    objs.EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &traceFilter, &trace);
    if (target == trace.m_pEnt) {
        return true;
    }
    return false;
}

Vec3 * Entity::GetBonePos(int boneId) const {
    static Vec3 bonePos;
    bonePos.x = *(float*)(dwBoneMatrix + 0x30 * boneId + 0x0C);
    bonePos.y = *(float*)(dwBoneMatrix + 0x30 * boneId + 0x1C);
    bonePos.z = *(float*)(dwBoneMatrix + 0x30 * boneId + 0x2C);
    return &bonePos;
}

Vec3 *Hack::GetViewAngles() const {
    return (Vec3*)(*(uintptr_t*)(engine + dwClientState) + dwClientState_ViewAngles);
}

int Hack::GetLocalEntityId() const {
    return *(int*)(*(uintptr_t*)(engine + dwClientState) + dwClientState_GetLocalPlayer);
}

void Hack::CheckButtons() {
#define CHECK_BTN_STATUS(name) if (GetAsyncKeyState(buttons.name##Btn) & 1) { settings.name = !settings.name; }
    CHECK_BTN_STATUS(showMenu);
    CHECK_BTN_STATUS(showTeammates);
    CHECK_BTN_STATUS(snapLines);
    CHECK_BTN_STATUS(box2D);
    CHECK_BTN_STATUS(box3D);
    CHECK_BTN_STATUS(velocityEsp);
    CHECK_BTN_STATUS(headline3D);
    CHECK_BTN_STATUS(rcsCrossHair);
    CHECK_BTN_STATUS(aimBot);
}
