#include <winuser.h>

void move_to(float screenWidth, float screenHeight, float smooth, float x, float y) {
    float center_x = screenWidth / 2;
    float center_y = screenHeight / 2;
    float target_x = 0.f;
    float target_y = 0.f;
    if (x != 0.f) {
        if (x > center_x) {
            target_x = -(center_x - x);
            target_x /= smooth;
            if (target_x + center_x > center_x * 2.f) target_x = 0.f;
        }
        if (x < center_x) {
            target_x = x - center_x;
            target_x /= smooth;
            if (target_x + center_x < 0.f) target_x = 0.f;
        }
    }
    if (y != 0.f) {
        if (y > center_y) {
            target_y = -(center_y - y);
            target_y /= smooth;
            if (target_y + center_y > center_y * 2.f) target_y = 0.f;
        }

        if (y < center_y) {
            target_y = y - center_y;
            target_y /= smooth;
            if (target_y + center_y < 0.f) target_y = 0.f;
        }
    }
    mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(target_x), static_cast<DWORD>(target_y), 0, 0);
}