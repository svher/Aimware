target_sources(${MODULE_NAME} PRIVATE
        imgui/imgui.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_draw.cpp)

target_sources(${MODULE_NAME} PRIVATE
        imgui/backends/imgui_impl_dx9.cpp
        imgui/backends/imgui_impl_win32.cpp)

target_sources(${MODULE_NAME} PRIVATE
        imgui/imgui_demo.cpp)
