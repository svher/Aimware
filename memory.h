#pragma once
#include <Windows.h>

class Memory {
public:
    static void* PatternScan(HMODULE module, const char* pattern);
};