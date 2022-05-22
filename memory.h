#pragma once
#include "includes.h"

class Memory {
public:
    static void* PatternScan(HMODULE module, const char* pattern);
};