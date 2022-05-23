#include "includes.h"

int windowHeight, windowWidth;

BOOL CALLBACK enumWindow(HWND hwnd, LPARAM lp) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    if (GetCurrentProcessId() != processId) {
        return TRUE;
    }
    window = hwnd;
    return FALSE;
}

HWND GetProcessWindow() {
    window = FindWindow("Valve001", nullptr);

    RECT size;
    GetWindowRect(window, &size);
    windowWidth = size.right - size.left;
    windowHeight = size.bottom - size.top;

    windowHeight -= 29;
    windowWidth -= 6;

    return window;
}

bool GetD3D9Device(void **pTable, size_t size) {
    if (!pTable) {
        return false;
    }

    IDirect3D9 *pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    IDirect3DDevice9* pDummyDevice = nullptr;

    D3DPRESENT_PARAMETERS d3dParams = {};
    d3dParams.Windowed = false;
    d3dParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dParams.hDeviceWindow = GetProcessWindow();

    HRESULT created = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dParams.hDeviceWindow,
                                         D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dParams, &pDummyDevice);

    if (created != S_OK) {
        d3dParams.Windowed = true;
        created = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dParams.hDeviceWindow,
                                     D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dParams, &pDummyDevice);

        if (created != S_OK) {
            pD3D->Release();
            return false;
        }
    }
    memcpy(pTable, *(void***)(pDummyDevice), size);
    pDummyDevice->Release();
    pD3D->Release();
    return TRUE;
}