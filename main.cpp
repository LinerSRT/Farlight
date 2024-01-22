#include <iostream>
#include <windows.h>
#include <string>
#include "utils/driver.h"
#include "cheat/overlay.h"
#include "ue/engine.h"

int selectedTab = 0;

void print(const std::string &value) {
    std::cout << value << std::endl;
}


[[noreturn]] void update() {
    while (true) {

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}


void render() {


}

void renderMenu() {
    drawSeparator();
    tabButton("ESP", &selectedTab, 0, true);
    tabButton("Debug", &selectedTab, 1, true);
    drawSeparator();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    print("[~] Loading vulnerable driver...");
    system("kdmapper.exe driver.sys >nul 2>nul");
    print("[~] Locating driver...");
    if (driver::find_driver()) {
        processId = driver::find_process("SolarlandClient-Win64-Shipping.exe");
        baseId = driver::find_image();
        print("[+] Process id: " + std::to_string(processId));
        print("[+] Base id: " + std::to_string(baseId));
        print("[~] Searching game window...");
        if (allocateGame()) {
            print("[~] Creating overlay...");
            setupOverlay();
            print("[~] Direct init...");
            directInit();
            print("[+] Attach success!");
            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(update), nullptr, 0, nullptr);
            while (driver::find_process("SolarlandClient-Win64-Shipping.exe") != -1) {
                overlayLoop();
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
            print("[+] Game was closed, stopping!");
        } else {
            print("[-] Game not found!");
        }
    } else {
        print("[-] Driver not found!");
    }
    return 0;
}
