#include "includes.h"
#include "hack.h"

extern Hack* hack;

void Hack::Init() {
    client = (uintptr_t)GetModuleHandle("client.dll");
    engine = (uintptr_t)GetModuleHandle("engine.dll");
    entityList = (EntityList*)(client + dwEntityList);
    localEntity = *(Entity**)(client + dwLocalPlayer);
}

void Hack::Update() {
    memcpy(&viewMatrix, (PBYTE*)(client + dwViewMatrix), sizeof(viewMatrix));
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

void Hack::Run() {
    Entity* closetEnemy = GetClosetEnemy();
    if (closetEnemy) {
        hack->AimAt(closetEnemy->GetBonePos(8));
    }
}

Vec3 * Entity::GetBonePos(int boneId) const {
    static Vec3 bonePos;
    bonePos.x = *(float*)(dwBoneMatrix + 0x30 * boneId + 0x0C);
    bonePos.y = *(float*)(dwBoneMatrix + 0x30 * boneId + 0x1C);
    bonePos.z = *(float*)(dwBoneMatrix + 0x30 * boneId + 0x2C);
    return &bonePos;
}

void Hack::AimAt(Vec3 *target) {
    Vec3 myPos = Vec3(localEntity->vecOrigin.x + localEntity->vecViewOffset.x,
                         localEntity->vecOrigin.y + localEntity->vecViewOffset.y,
                         localEntity->vecOrigin.z + localEntity->vecViewOffset.z);
    Vec3 deltaVec = Vec3(target->x - myPos.x, target->y - myPos.y, target->z - myPos.z);
    float deltaVecLength = deltaVec.Norm2D();
    auto pitch = float(-asin(deltaVec.z / deltaVecLength) * (180 / PI));
    auto yaw = float(atan2(deltaVec.y, deltaVec.x) * (180 / PI));

    Vec3* viewAngles = GetViewAngles();

    if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) {
        viewAngles->x = pitch;
        viewAngles->y = yaw;
    }
}

Vec3 *Hack::GetViewAngles() const {
    return (Vec3*)(*(uintptr_t*)(hack->engine + dwClientState) + dwClientState_ViewAngles);
}

Entity *Hack::GetClosetEnemy() {
    float closetDistance = 10000;
    int closetDistanceIndex = -1;
    for (int i = 1; i < 32; i++) {
        Entity* currentEntity = entityList->entities[i].entity;
        if (!CheckValidEntity(currentEntity)) {
            continue;
        }
        if (currentEntity->iTeamNum == localEntity->iTeamNum) {
            continue;
        }
        Vec3 myPos = localEntity->vecOrigin;
        Vec3 enemyPos = currentEntity->vecOrigin;
        Vec3 delta = Vec3(enemyPos.x - myPos.x, enemyPos.y - myPos.y, enemyPos.z - myPos.z);
        float currentDistance = delta.Norm2D();
        if (currentDistance < closetDistance) {
            closetDistance = currentDistance;
            closetDistanceIndex = i;
        }
    }
    if (closetDistanceIndex == -1) {
        return nullptr;
    }
    return entityList->entities[closetDistanceIndex].entity;
}
