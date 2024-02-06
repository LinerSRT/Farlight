#include "mouse_controller.h"

namespace input {
    BYTE ntUserSendInputBytes[30];
    BYTE ntUserGetKeyStateBytes[30];

    BOOLEAN WINAPI init() {
        auto ntUserSendInput = (LPVOID) GetProcAddress(GetModuleHandleA("win32u"), "NtUserSendInput");
        if (!ntUserSendInput) {
            ntUserSendInput = (LPVOID) GetProcAddress(GetModuleHandleA("user32"), "NtUserSendInput");
            if (!ntUserSendInput) {
                ntUserSendInput = (LPVOID) GetProcAddress(GetModuleHandleA("user32"), "SendInput");
                if (!ntUserSendInput)
                    return FALSE;
            }
        }
        auto ntUserGetKeyState = (LPVOID) GetProcAddress(GetModuleHandleA("win32u"), "NtUserGetAsyncKeyState");
        if (!ntUserGetKeyState) {
            ntUserGetKeyState = (LPVOID) GetProcAddress(GetModuleHandleA("user32"), "NtUserGetAsyncKeyState");
            if (!ntUserGetKeyState) {
                ntUserGetKeyState = (LPVOID) GetProcAddress(GetModuleHandleA("user32"), "GetAsyncKeyState");
                if (!ntUserGetKeyState)
                    return FALSE;
            }
        }
        memcpy(ntUserSendInputBytes, ntUserSendInput, 30);
        memcpy(ntUserGetKeyStateBytes, ntUserGetKeyState, 30);
        return TRUE;
    }

    BOOLEAN WINAPI unInit() {
        ZeroMemory(ntUserSendInputBytes, 30);
        ZeroMemory(ntUserGetKeyStateBytes, 30);
        return TRUE;
    }

    BOOLEAN WINAPI sendInput(UINT cInputs, LPINPUT pInputs, int cbSize) {
        LPVOID inputSpoof = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!inputSpoof)
            return FALSE;
        memcpy(inputSpoof, ntUserSendInputBytes, 30);
        NTSTATUS result = reinterpret_cast<NTSTATUS(NTAPI *)(UINT, LPINPUT, int)>(inputSpoof)(cInputs, pInputs, cbSize);
        ZeroMemory(inputSpoof, 0x1000);
        VirtualFree(inputSpoof, 0, MEM_RELEASE);
        return (result > 0);
    }

    UINT WINAPI getAsyncKeyState(UINT Key) {
        LPVOID spoof = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!spoof)
            return FALSE;
        memcpy(spoof, ntUserGetKeyStateBytes, 30);
        NTSTATUS result = reinterpret_cast<NTSTATUS(NTAPI *)(UINT)>(spoof)(Key);
        ZeroMemory(spoof, 0x1000);
        VirtualFree(spoof, 0, MEM_RELEASE);
        return result;
    }

    BOOLEAN WINAPI moveMouse(int x, int y) {
        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.mouseData = 0;
        input.mi.time = 0;
        input.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
        input.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;
        return sendInput(1, &input, sizeof(input));
    }

    BOOLEAN WINAPI moveMouseDxDy(float x, float y) {
        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.mouseData = 0;
        input.mi.time = 0;
        input.mi.dx = static_cast<int>(x);
        input.mi.dy = static_cast<int>(y);
        input.mi.dwFlags = MOUSEEVENTF_MOVE ;
        return sendInput(1, &input, sizeof(input));
    }

    UINT WINAPI getKeyState(UINT Key) {
        return getAsyncKeyState(Key);
    }
}