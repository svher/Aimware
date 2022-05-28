#include "includes.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx9.h"

#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern Hack *hack;

void gui::SetupMenu(LPDIRECT3DDEVICE9 pDevice) noexcept {
    auto params = D3DDEVICE_CREATION_PARAMETERS {};
    pDevice->GetCreationParameters(&params);

    window = params.hFocusWindow;
    oWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc)));

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(pDevice);
    setup = true;
}

void gui::Destroy() noexcept {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (setup) {
        SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWindowProc));
        setup = false;
    }
}

void gui::Render() noexcept {
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (open) {
        ImGui::ShowDemoWindow(&open);
    }

    if (ImGui::Begin("TEST")) {
        ImGui::Text("ABOUT THIS DEMO:");
        ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
        ImGui::BulletText("The \"Examples\" menu above leads to more demo contents.");
        ImGui::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n");
        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (GetAsyncKeyState(VK_HOME) & 1) {
        gui::open = !gui::open;
    }

    if (gui::open) {
        ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    }
    hack->objs.InputSystem->EnableInput(!gui::open);

    return CallWindowProc(gui::oWindowProc, hwnd, uMsg, wParam, lParam);
}
