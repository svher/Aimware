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

class Vec2 {
public:
    float x, y;
};

class Vec3 {
public:
    float x, y, z;

    Vec3() = default;
    Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float Norm2D() const {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }
};

class Vec4 {
public:
    float x, y, z, w;
};

class Entity {
public:
    union {
        DEFINE_MEMBER_N(bool, isDormant, m_bDormant);
        DEFINE_MEMBER_N(int, iHealth, m_iHealth);
        DEFINE_MEMBER_N(Vec3, vecOrigin, m_vecOrigin);
        DEFINE_MEMBER_N(int, iTeamNum, m_iTeamNum);
        DEFINE_MEMBER_N(uint32_t, dwBoneMatrix, m_dwBoneMatrix);
        DEFINE_MEMBER_N(Vec3, vecViewOffset, m_vecViewOffset)
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

    void Init();
    // update view matrix
    void Update();
    bool CheckValidEntity(Entity* entity) const;
    bool WorldToScreen(Vec3 position, Vec2& screen);
    Entity* GetClosetEnemy();
    void AimAt(Vec3 *target);
    Vec3* GetViewAngles() const;
    // aimbot
    void Run();
};