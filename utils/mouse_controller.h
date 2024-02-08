#include <windows.h>

namespace input {

    BOOLEAN WINAPI init();

    BOOLEAN WINAPI unInit();

    BOOLEAN WINAPI sendInput(UINT cInputs, LPINPUT pInputs, int cbSize);

    UINT WINAPI getAsyncKeyState(UINT Key);

    BOOLEAN WINAPI moveMouse(int x, int y);

    BOOLEAN WINAPI holdKey(UINT Key);

    BOOLEAN WINAPI moveMouseDxDy(float x, float y);

    UINT WINAPI getKeyState(UINT Key);
}