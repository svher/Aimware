#include "hook.h"

void Patch(BYTE *dst, BYTE *src, unsigned int size) {
    DWORD oldProtect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldProtect, nullptr);
}

bool Hook(BYTE *src, const BYTE *dst, unsigned int len) {
    // space for long jmp instruction
    if (len < 5) {
        return false;
    }
    DWORD oldProtect;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
    // E9: JMP rel16
    memset(src, 0xFF, len);
    auto relAddy = (uintptr_t)(dst - src - 5);
    *src = (BYTE)0xE9;
    *(uintptr_t*)(src +  1) = (uintptr_t)relAddy;
    VirtualProtect(src, len, oldProtect, nullptr);
    return true;
}

// x86 Trampoline Hook
BYTE * TrampHook(BYTE *src, BYTE *dst, unsigned int len) {
    if (len < 5) {
        return nullptr;
    }
    auto gateway = (BYTE*)VirtualAlloc(nullptr, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(gateway, src, len);
    // jmp from gateway + len + 5 to src + len, thus bypass the replaced code
    auto relAddy = (uintptr_t)(src - gateway - 5);
    *(gateway + len) = (BYTE)0xE9;
    *(uintptr_t*)(gateway + len + 1) = relAddy;
    if (Hook(src, dst, len)) {
        return gateway;
    } else {
        return nullptr;
    }
}
