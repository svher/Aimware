#pragma once

#include "csgo.h"
#include "vector.h"
#include <string>

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
        DEFINE_MEMBER_N(int, fFlags, m_fFlags);
        DEFINE_MEMBER_N(bool, bSpotted, m_bSpotted);
        DEFINE_MEMBER_N(bool, bGunGameImmunity, m_bGunGameImmunity)
    };
    NETVAR(_fImmuneToGunGameDamageTime, "CCSPlayer->m_fImmuneToGunGameDamageTime", float)
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
