#pragma once
#include <Windows.h>

void Patch(BYTE* dst, BYTE* src, unsigned int size);
bool Hook(void* src, void* dst, int len);
BYTE * TrampHook(BYTE *src, BYTE *dst, unsigned int len);