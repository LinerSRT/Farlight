#include <windows.h>
#include <d3d11.h>
#include <d3d9.h>
#include <d3dcompiler.h>
#include <dwmapi.h>
#include <string>
#include <sstream>
#include <tchar.h>
#include <memory>
#include <thread>
#include <functional>
#include <array>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

typedef struct Farlight {
    HWND window = nullptr;
    int width{};
    int height{};
    int left{};
    int top{};
    int right{};
    int bottom{};
    int centerX{};
    int centerY{};
} Farlight;


Farlight farlight = Farlight();

namespace Overlay {
    WNDCLASSEX className;
    HWND window;
    LPCSTR name;
    int width = 600;
    int height = 500;
    bool show = true;
    ImFont *DroidSans, *DefaultFont;
}

namespace D3D {
    IDirect3D9Ex *d3d = nullptr;
    IDirect3DDevice9Ex *device = nullptr;
    D3DPRESENT_PARAMETERS parameters = {};
    MARGINS Margin = {-1};
    MSG Message = {nullptr};
}

void render();

void renderMenu();

void inputHandle() {
    for (bool &i: ImGui::GetIO().MouseDown) i = false;
    int button = -1;
    if (GetAsyncKeyState(VK_LBUTTON)) button = 0;
    if (button != -1) ImGui::GetIO().MouseDown[button] = true;
}


void renderFrame() {
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        Overlay::show = !Overlay::show;
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    render();
    ImGui::GetIO().MouseDrawCursor = Overlay::show;
    if (Overlay::show) {
        inputHandle();
        ImGui::SetNextWindowPos(ImVec2((float) farlight.width / 2 - (float) Overlay::width / 2, (float) farlight.height / 2 - (float) Overlay::height / 2));
        ImGui::SetNextWindowSize(ImVec2((float) farlight.width, (float) farlight.height));
        ImGui::PushFont(Overlay::DefaultFont);
        ImGui::Begin("Farlight84 Tool", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        renderMenu();
        ImGui::PopFont();
        ImGui::End();
    }
    ImGui::EndFrame();
    D3D::device->SetRenderState(D3DRS_ZENABLE, false);
    D3D::device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    D3D::device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
    D3D::device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    if (D3D::device->BeginScene() >= 0) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        D3D::device->EndScene();
    }
    HRESULT result = D3D::device->Present(nullptr, nullptr, nullptr, nullptr);
    if (result == D3DERR_DEVICELOST && D3D::device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        D3D::device->Reset(&D3D::parameters);
        ImGui_ImplDX9_CreateDeviceObjects();
    }
}


void overlayLoop() {
    static RECT OldRect;
    ZeroMemory(&D3D::Message, sizeof(MSG));
    while (D3D::Message.message != WM_QUIT) {
        if (PeekMessage(&D3D::Message, Overlay::window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&D3D::Message);
            DispatchMessage(&D3D::Message);
        }
        HWND foregroundWindow = GetForegroundWindow();
        if (foregroundWindow == farlight.window) {
            HWND TempProcessHwnd = GetWindow(foregroundWindow, GW_HWNDPREV);
            SetWindowPos(Overlay::window, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
        RECT rect;
        POINT point;
        ZeroMemory(&rect, sizeof(RECT));
        ZeroMemory(&point, sizeof(POINT));
        GetClientRect(farlight.window, &rect);
        ClientToScreen(farlight.window, &point);
        rect.left = point.x;
        rect.top = point.y;
        ImGuiIO &io = ImGui::GetIO();
        io.ImeWindowHandle = farlight.window;
        POINT cursorPosition;
        GetCursorPos(&cursorPosition);
        io.MousePos.x = (float) cursorPosition.x - (float) point.x;
        io.MousePos.y = (float) cursorPosition.y - (float) point.y;
        if (GetAsyncKeyState(0x1)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].x = io.MousePos.y;
        } else {
            io.MouseDown[0] = false;
        }
        if (rect.left != OldRect.left || rect.right != OldRect.right || rect.top != OldRect.top ||
            rect.bottom != OldRect.bottom) {
            OldRect = rect;
            farlight.width = rect.right;
            farlight.height = rect.bottom;
            D3D::parameters.BackBufferWidth = farlight.width;
            D3D::parameters.BackBufferHeight = farlight.height;
            SetWindowPos(Overlay::window, (HWND) 0, point.x, point.y, farlight.width, farlight.height, SWP_NOREDRAW);
            D3D::device->Reset(&D3D::parameters);
        }
        renderFrame();
    }
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    if (D3D::device != nullptr) {
        D3D::device->EndScene();
        D3D::device->Release();
    }
    if (D3D::d3d != nullptr) {
        D3D::d3d->Release();
    }
    DestroyWindow(Overlay::window);
    UnregisterClass(Overlay::className.lpszClassName, Overlay::className.hInstance);
}


bool directInit() {
    if (Direct3DCreate9Ex(D3D_SDK_VERSION, &D3D::d3d) < 0) {
        return false;
    }
    D3DPRESENT_PARAMETERS Params = {0};
    Params.Windowed = TRUE;
    Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    Params.hDeviceWindow = Overlay::window;
    Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    Params.BackBufferFormat = D3DFMT_A8R8G8B8;
    Params.BackBufferWidth = farlight.width;
    Params.BackBufferHeight = farlight.height;
    Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    Params.EnableAutoDepthStencil = TRUE;
    Params.AutoDepthStencilFormat = D3DFMT_D16;
    Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    if (D3D::d3d->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Overlay::window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, nullptr, &D3D::device) < 0) {
        D3D::d3d->Release();
        return false;
    }
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.WantCaptureMouse || io.WantTextInput || io.WantCaptureKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplWin32_Init(Overlay::window);
    ImGui_ImplDX9_Init(D3D::device);
    D3D::d3d->Release();
    return true;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
        return true;
    switch (Message) {
        case WM_DESTROY:
            if (D3D::device != nullptr) {
                D3D::device->EndScene();
                D3D::device->Release();
            }
            if (D3D::d3d != nullptr) {
                D3D::d3d->Release();
            }
            PostQuitMessage(0);
            exit(4);
        case WM_SIZE:
            if (D3D::device != nullptr && wParam != SIZE_MINIMIZED) {
                ImGui_ImplDX9_InvalidateDeviceObjects();
                D3D::parameters.BackBufferWidth = LOWORD(lParam);
                D3D::parameters.BackBufferHeight = HIWORD(lParam);
                HRESULT hr = D3D::device->Reset(&D3D::parameters);
                if (hr == D3DERR_INVALIDCALL)
                    IM_ASSERT(0);
                ImGui_ImplDX9_CreateDeviceObjects();
            }
            break;
        default:
            return
                    DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}


void setupOverlay() {
    Overlay::name = "FTLDebug";
    Overlay::className = {
            sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION),
            LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, Overlay::name, LoadIcon(nullptr, IDI_APPLICATION)
    };
    RegisterClassEx(&Overlay::className);
    static RECT rect = {};
    static POINT point;
    GetClientRect(farlight.window, &rect);
    ClientToScreen(farlight.window, &point);
    Overlay::window = CreateWindowEx(NULL, Overlay::name, Overlay::name, WS_POPUP | WS_VISIBLE, point.x, point.y, rect.right, rect.bottom, nullptr, nullptr, nullptr, nullptr);
    DwmExtendFrameIntoClientArea(Overlay::window, &D3D::Margin);
    SetWindowLong(Overlay::window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
    ShowWindow(Overlay::window, SW_SHOW);
    UpdateWindow(Overlay::window);
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    Overlay::DefaultFont = io.Fonts->AddFontDefault();
    Overlay::DroidSans = io.Fonts->AddFontFromFileTTF(R"(DroidSans.ttf)", 16.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
    io.Fonts->Build();
}

bool allocateGame() {
    HWND gameWindow = FindWindowA("UnrealWindow", nullptr);
    if (gameWindow) {
        farlight.window = gameWindow;
        RECT rect;
        GetWindowRect(farlight.window, &rect);
        farlight.width = rect.right - rect.left;
        farlight.height = rect.bottom - rect.top;
        farlight.left = rect.left;
        farlight.top = rect.top;
        farlight.right = rect.right;
        farlight.bottom = rect.bottom;
        farlight.centerX = farlight.left + farlight.width / 2;
        farlight.centerY = farlight.top + farlight.height / 2;
        return true;
    } else {
        return false;
    }
}


auto drawLine(float x1, float y1, float x2, float y2, int rgb[], float width) -> VOID {
    auto vList = ImGui::GetForegroundDrawList();
    ImVec2 start = ImVec2(x1, y1);
    ImVec2 end = ImVec2(x2, y2);
    ImColor color = ImColor(rgb[0], rgb[1], rgb[2], rgb[3]);
    vList->AddLine(start, end, color, width);
}

auto drawBox(float x, float y, float w, float h, int rgb[]) -> VOID {
    drawLine(x, y, x + w, y, rgb, 1.3f); // top
    drawLine(x, y - 1.3f, x, y + h + 1.4f, rgb, 1.3f); // left
    drawLine(x + w, y - 1.3f, x + w, y + h + 1.4f, rgb, 1.3f);  // right
    drawLine(x, y + h, x + w, y + h, rgb, 1.3f);   // bottom
}

auto drawRectFilled(float x0, float y0, float x1, float y1, int rgb[], float rounding, int rounding_corners_flags) -> VOID {
    auto vList = ImGui::GetForegroundDrawList();
    ImColor color = ImColor(rgb[0], rgb[1], rgb[2], rgb[3]);
    vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

auto tabButton(const char *label, int *index, int val, bool sameLine) -> VOID {
    if (ImGui::Button(label, ImVec2(80, 25)))
        *index = val;
    if (sameLine)
        ImGui::SameLine();
}

auto button(const char *label, bool sameLine, void (*_when_clicked)()) -> VOID {
    if (ImGui::Button(label, ImVec2(80, 25))){
        _when_clicked();
    }
    if (sameLine)
        ImGui::SameLine();
}

auto drawText(const std::string &text, float xX, float yY, float size, ImU32 color, bool center) -> VOID {
    ImVec2 pos = ImVec2(xX, yY);
    ImGui::PushFont(Overlay::DefaultFont);
    std::stringstream stream(text);
    std::string line;
    float y = 0.0f;
    int index = 0;
    while (std::getline(stream, line)) {
        ImVec2 textSize = Overlay::DefaultFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
        if (center) {
            ImGui::GetForegroundDrawList()->AddText(Overlay::DefaultFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
            ImGui::GetForegroundDrawList()->AddText(Overlay::DefaultFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
            ImGui::GetForegroundDrawList()->AddText(Overlay::DefaultFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
        } else {
            ImGui::GetForegroundDrawList()->AddText(Overlay::DefaultFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
            ImGui::GetForegroundDrawList()->AddText(Overlay::DefaultFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
            ImGui::GetForegroundDrawList()->AddText(Overlay::DefaultFont, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
        }
        y = pos.y + textSize.y * ((float) index + 1.0f);
        index++;
    }
    ImGui::PopFont();
}

auto drawCircle( float xX, float yY, const float radius, int rgb[], const FLOAT lineWidth) -> VOID {
    auto vList = ImGui::GetForegroundDrawList();
    ImColor color = ImColor(rgb[0], rgb[1], rgb[2], rgb[3]);
    ImVec2 pos = ImVec2(xX, yY);
    vList->AddCircle(pos, radius, color, 120, lineWidth);
}

auto drawCircle(float x, float y, float radius, ImColor color, int segments) -> VOID {
    auto vList = ImGui::GetForegroundDrawList();
    vList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
}

auto drawCircleFilled(float x, float y, float radius, int rgb[], int segments) -> VOID {
    auto vList = ImGui::GetForegroundDrawList();
    ImColor color = ImColor(rgb[0], rgb[1], rgb[2], rgb[3]);
    vList->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
}

void drawRoundRect(float x, float y, float width, float height, float radius, int rgb[]) {
    ImColor color = ImColor(rgb[0], rgb[1], rgb[2], rgb[3]);
    const float corner_radius = std::clamp(radius, 0.0f, (width < height ? width : height) * 0.5f - 1.0f);
    const ImVec2 p0 = ImVec2(x, y);
    const ImVec2 p1 = ImVec2(x + width, y + height);
    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p0.x + corner_radius, p0.y + corner_radius), corner_radius, 6, 9);
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p1.x - corner_radius, p0.y + corner_radius), corner_radius, 9, 12);
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p1.x - corner_radius, p1.y - corner_radius), corner_radius, 0, 3);
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p0.x + corner_radius, p1.y - corner_radius), corner_radius, 3, 6);
    ImGui::GetForegroundDrawList()->PathFillConvex(color);
}

void drawRoundRect(float x, float y, float width, float height, float radiusTopLeft, float radiusTopRight, float radiusBottomRight, float radiusBottomLeft, int rgb[]) {
    ImColor color = ImColor(rgb[0], rgb[1], rgb[2], rgb[3]);
    const ImVec2 p0 = ImVec2(x, y);
    const ImVec2 p1 = ImVec2(x + width, y + height);
    const float topLeftRadius = std::clamp(radiusTopLeft, 0.0f, (width < height ? width : height) * 0.5f - 1.0f);
    const float topRightRadius = std::clamp(radiusTopRight, 0.0f, (width < height ? width : height) * 0.5f - 1.0f);
    const float bottomRightRadius = std::clamp(radiusBottomRight, 0.0f, (width < height ? width : height) * 0.5f - 1.0f);
    const float bottomLeftRadius = std::clamp(radiusBottomLeft, 0.0f, (width < height ? width : height) * 0.5f - 1.0f);
    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p0.x + topLeftRadius, p0.y + topLeftRadius), topLeftRadius, 6, 9);
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p1.x - topRightRadius, p0.y + topRightRadius), topRightRadius, 9, 12);
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p1.x - bottomRightRadius, p1.y - bottomRightRadius), bottomRightRadius, 0, 3);
    ImGui::GetForegroundDrawList()->PathArcToFast(ImVec2(p0.x + bottomLeftRadius, p1.y - bottomLeftRadius), bottomLeftRadius, 3, 6);
    ImGui::GetForegroundDrawList()->PathFillConvex(color);
}

void drawSeparator(){
    ImGui::Separator();
}