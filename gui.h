#pragma once
#include <d3d9.h>

namespace gui {
    inline bool open = true;
    inline bool setup = false;
    inline HWND window = nullptr;
    inline WNDCLASSEX windowClass = {};
    inline WNDPROC oWindowProc = nullptr;
    inline LPDIRECT3DDEVICE9 device = nullptr;
    inline LPDIRECT3D9 d3d9 = nullptr;

    void Setup();
    void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
    void Destroy() noexcept;

    void Render() noexcept;
}
