#include <iostream>
#include <windows.h>
#include <string>
#include "utils/driver.h"
#include "cheat/overlay.h"
#include "ue/solarland.h"

int selectedTab = 0;
DWORD_PTR gWorld;
UWorld* world{nullptr};

void print(const std::string &value) {
    std::cout << value << std::endl;
}


[[noreturn]] void update() {
    while (true) {
        gWorld = read<DWORD_PTR>(baseId + 0x6FD96F8);
        world = new UWorld(gWorld);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}


void render() {
    //auto level = world->getWorldLevel();
    //auto items = level.getItems();
    //auto players = level.getCharacters();
    //auto localPlayer = UPlayer(world->owningGameInstance.localPlayerPointer);
    //auto cameraManager = localPlayer.playerController.cameraManager;
    //for (DWORD_PTR pointer : players) {
    //    auto player = UPlayer(pointer);
    //    auto position = player.playerController.acknowledgedPawn.rootComponent.relativeLocation;
    //    auto screenPosition = worldToScreen(FVector(position.X, position.Y, position.Z), cameraManager.cameraCache.POV.Location, cameraManager.cameraCache.POV.Rotation, cameraManager.cameraCache.POV.FOV);
    //    drawCircle(screenPosition.X, screenPosition.Y, 10, {255, 0, 0}, 30);
    //}
//
    //drawText("players: " + std::to_string(players.size()),  32, (1 * 24), 16, ImColor(255, 25, 25, 255), false);
    //drawText("cameraManager: " + cameraManager.cameraCachePrivate.POV.Rotation.debug(),  32, (2 * 24), 16, ImColor(255, 25, 25, 255), false);


    //drawText("isCrouched: " + std::to_string(isCrouched), 32, (1 * 24), 16, ImColor(255, 25, 25, 255), false);
    //drawText("isFalling: " + std::to_string(isFalling), 32, (2 * 24), 16, ImColor(255, 25, 25, 255), false);
    //drawText("Class: " + className.toString(baseId), 32, (3 * 24), 16, ImColor(255, 25, 25, 255), false);
    //drawText("Levels: " + std::to_string(levels.length()), 32, (4 * 24), 16, ImColor(255, 25, 25, 255), false);
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
