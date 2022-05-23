#pragma once
#include <d3d9.h>
#include <Windows.h>

using EndSceneFn = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice);

static HWND window;

extern int windowHeight, windowWidth;

HWND GetProcessWindow();

bool GetD3D9Device(void** pTable, size_t size);