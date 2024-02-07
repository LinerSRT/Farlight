cmake_minimum_required(VERSION 3.27)
project(Farlight)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
message( "\${CMAKE_CXX_FLAGS_RELEASE} = ${CMAKE_CXX_FLAGS_RELEASE}")
message( "\${CMAKE_CXX_FLAGS_DEBUG} = ${CMAKE_CXX_FLAGS_DEBUG}")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
add_link_options(-static -static-libgcc -static-libstdc++)
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
        cheat/offsets.h
        solarland/solar_struct.h
        solarland/solar_enum.h
        utils/f_math.h
        utils/strings.h
        cheat/solarland.h
        cheat/images.h
        imgui/font_awesome.h
        cheat/solar_enum.h
        ue/utils.h
        ue/unreal.h
        utils/mouse_controller.cpp
        utils/mouse_controller.h
)
target_link_libraries(Farlight PRIVATE d3d11.lib d3d9.lib dwmapi.lib d3dcompiler.lib d3dx9.lib ntdll.lib)