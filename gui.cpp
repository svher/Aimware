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

bool cbNetvars = false;
bool showDemo = false;
bool showSettings = false;

void gui::Render() noexcept {
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (showDemo) {
        ImGui::ShowDemoWindow(&showDemo);
    }
    if (showSettings) {
        if (ImGui::Begin("Settings")) {
            ImGui::Checkbox("Show Teammates (F1)", &hack->settings.showTeammates);
            ImGui::Checkbox("Aimbot (F2)", &hack->settings.aimBot);
            ImGui::Checkbox("2D Box (F3)", &hack->settings.box2D);
            ImGui::Checkbox("3D Box (F4)", &hack->settings.box3D);
            ImGui::Checkbox("Velocity ESP (F5)", &hack->settings.velocityEsp);
            ImGui::Checkbox("Headline ESP (F6)", &hack->settings.headline3D);
            ImGui::Checkbox("Recoil Crosshair (F7)", &hack->settings.rcsCrossHair);
            ImGui::Checkbox("Snaplines (F8)", &hack->settings.snapLines);
            ImGui::Separator();
            ImGui::Text("Misc");
            ImGui::Checkbox("FakeAim", &hack->settings.fakeAim);
            ImGui::Checkbox("Friendly Fire", &hack->settings.friendlyFire);
        }
        ImGui::End();
    }

    if (open) {
        if (ImGui::Begin("Offsets", &open)) {
            if (ImGui::Button("Settings")) {
                showSettings = !showSettings;
            }
            ImGui::SameLine();
            if (ImGui::Button("Demo window")) {
                showDemo = !showDemo;
            }
            ImGui::Separator();
            ImGui::Text("General");
            ImGui::BulletText("ClientGadget 0x%08x", (uint32_t) hack->clientGadget);
            ImGui::Separator();
            ImGui::Text("Hooks");
            ImGui::BulletText("EndScene 0x%08x", (uint32_t) EndScenePtr);
            ImGui::BulletText("Reset 0x%08x", (uint32_t) ResetPtr);
            ImGui::BulletText("CreateMove 0x%08x", (uint32_t) CreateMovePtr);
            ImGui::BulletText("LockCursor 0x%08x", (uint32_t) LockCursorPtr);
            ImGui::Separator();
            ImGui::Text("Netvars");
            ImGui::Checkbox("Show netvars offsets table", &cbNetvars);
            if (cbNetvars) {
                static ImGuiTextFilter filter;
                filter.Draw();
                if (ImGui::BeginTable("netvars", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders)) {
                    for (auto it = namedNetvars.begin(); it != namedNetvars.end(); ++it) {
                        if (filter.PassFilter(it->first.c_str())) {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%s", it->first.c_str());
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("0x%08x", it->second);
                        }
                    }
                }
                ImGui::EndTable();
            }
        }
        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        gui::open = !gui::open;
    }

    if (gui::open) {
        ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    }
    hack->objs.InputSystem->EnableInput(!gui::open);

    return CallWindowProc(gui::oWindowProc, hwnd, uMsg, wParam, lParam);
}
