#pragma once

#include "client_class.h"
#include "hash.h"
#include <unordered_map>
#include <map>

void SetupNetvars();

void Dump(const char *baseClass, RecvTable *table, uint32_t offset = 0);

inline std::unordered_map<uint32_t, uint32_t> netvars;

#define NETVAR(funcName, netvar, type) type& funcName() \
{                                                       \
    static auto offset = netvars[fnv::HashConst(netvar)];\
    return *reinterpret_cast<type*>(uintptr_t(this) + offset);\
}
