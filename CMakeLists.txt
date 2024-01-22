cmake_minimum_required(VERSION 3.27)
project(Farlight)

set(CMAKE_CXX_STANDARD 20)

add_executable(Farlight
        main.cpp
        utils/driver.h
        utils/driver.cpp
        utils/driverdefs.h
        imgui/imgui.h
        imgui/imconfig.h
        imgui/imgui_internal.h
        imgui/imgui_widgets.cpp
        imgui/imgui_tables.cpp
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_dx9.h
        imgui/imgui_impl_dx9.cpp
        imgui/imgui_impl_win32.h
        imgui/imgui_impl_win32.cpp
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        ue/engine.h
        cheat/overlay.h
)

target_link_libraries(Farlight PRIVATE d3d11.lib d3d9.lib dwmapi.lib d3dcompiler.lib)