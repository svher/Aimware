#pragma once
#include "includes.h"

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

static HWND window;

extern int windowHeight, windowWidth;

extern LPDIRECT3DDEVICE9 pDevice;

HWND GetProcessWindow();

bool GetD3D9Device(void** pTable, size_t size);