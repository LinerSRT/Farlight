#include <iostream>
#include <windows.h>
#include <string>
#include "utils/driver.h"
#include "cheat/overlay.h"
#include "cheat/solarland.h"
#include "utils/f_math.h"

#define VERSION 1
#define SUB_VERSION 1

WorldSettings worldSettings;
GameEntity localPlayer;


std::vector<GameEntity> entityList;
std::vector<std::string> unknownEntity;
int selectedItem = 0;
float aimBotFov = 80;
float aimBotSmooth = 0;

bool aiming = false;
GameEntity targetCharacter;

uint64_t readThreadDelay;
uint64_t drawThreadDelay;

struct Pointers {
    DWORD_PTR uWorld;
    DWORD_PTR gameState;
    DWORD_PTR gameMode;
    DWORD_PTR owningGameInstance;
    DWORD_PTR persistentLevel;
    TArray<DWORD_PTR> entityList;;
};


Pointers pointers;
DWORD offset = 0;

void print(const std::string &value) {
    std::cout << value << std::endl;
}


[[noreturn]] void update() {
    while (true) {
        auto start = local_time::milliseconds();
        std::vector<GameEntity> tmp_entityList;
        std::vector<std::string> tmp_unknownEntity;
        pointers.uWorld = read<DWORD_PTR>(baseId + offsets::gWorld);
        pointers.gameState = read<DWORD_PTR>(pointers.uWorld + offsets::wGameState);
        pointers.gameMode = read<DWORD_PTR>(pointers.uWorld + offsets::wAuthorityGameMode);
        pointers.owningGameInstance = read<DWORD_PTR>(pointers.uWorld + offsets::wOwningGameInstance);
        pointers.persistentLevel = read<DWORD_PTR>(pointers.uWorld + offsets::wPersistentLevel);
        localPlayer = GameEntity(read<DWORD_PTR>(read<DWORD_PTR>(pointers.owningGameInstance + offsets::giLocalPlayers)), LOCAL_CHARACTER);
        pointers.entityList = read<TArray<DWORD_PTR>>(pointers.persistentLevel + offsets::ncOwningActor);
        for (int i = 0; i < pointers.entityList.length(); ++i) {
            auto pointer = pointers.entityList.getValuePointer(i);
            if (pointer == 0x0)
                continue;
            auto name = UObject::getClassName(pointer);
            if (EntityType::isPickup(name)) {
                tmp_entityList.emplace_back(pointer, ITEM);
            } else if (EntityType::isPlayer(name)) {
                tmp_entityList.emplace_back(pointer, CHARACTER);
            } else if (EntityType::isTreasureBox(name)) {
                tmp_entityList.emplace_back(pointer, ITEM_BOX);
            } else if (EntityType::isDeadBox(name)) {
                tmp_entityList.emplace_back(pointer, DEATH_BOX);
            } else if (EntityType::isVehicle(name)) {
                tmp_entityList.emplace_back(pointer, VEHICLE);
            } else if (EntityType::isWorldSettings(name) || !worldSettings.isValid()) {
                worldSettings = WorldSettings(pointer);
            } else {
                tmp_unknownEntity.emplace_back(name);
            }
        }
        entityList = tmp_entityList;
        unknownEntity = tmp_unknownEntity;

        readThreadDelay = local_time::milliseconds() - start;

    }
}


void aimFunction(float fov, float smooth) {
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    float lastDistanceFromCrossHair = FLT_MAX;
    if (!aiming || !targetCharacter.isValid())
        for (const auto &e: entityList) {
            if (e.entityType == CHARACTER) {
                if (Character::isLocalPlayer(localPlayer, e))
                    continue;
                if (Character::isTeamWith(localPlayer, e))
                    continue;
                auto characterHeadPosition = Character::getWorldHeadPosition(e, 5);
                auto characterHeadPositionOnScreen = Character::projectToScreen(localPlayer, characterHeadPosition, FVector((float) farlight.width, (float) farlight.height, 0));
                auto distanceToCrosshair = screenCenter.distance2D(characterHeadPositionOnScreen);
                if (distanceToCrosshair < lastDistanceFromCrossHair && distanceToCrosshair < fov) {
                    targetCharacter = e;
                    lastDistanceFromCrossHair = distanceToCrosshair;
                }
            }
        }
    drawCircle(screenCenter.x, screenCenter.y, fov, ImColor(255, 255, 255), 30);
    if (!targetCharacter.isValid()) {
        aiming = false;
        return;
    }
    if (Character::isLocalPlayer(localPlayer, targetCharacter))
        return;
    if (Character::isDead(targetCharacter))
        return;
    auto aimPosition = Character::predictAimPosition(localPlayer, targetCharacter, worldSettings.globalGravityZ, worldSettings.worldToMeters);
    auto aipPositionOnScreen = Character::projectToScreen(localPlayer, aimPosition, FVector((float) farlight.width, (float) farlight.height, 0));
    if (GetAsyncKeyState(VK_SHIFT)) {
        aiming = true;
        float center_x = (float) farlight.width / 2;
        float center_y = (float) farlight.height / 2;
        float target_x = 0.f;
        float target_y = 0.f;
        if (aipPositionOnScreen.x != 0.f) {
            if (aipPositionOnScreen.x > center_x) {
                target_x = -(center_x - aipPositionOnScreen.x);
                if (smooth != 0)
                    target_x /= smooth;
                if (target_x + center_x > center_x * 2.f) target_x = 0.f;
            }
            if (aipPositionOnScreen.x < center_x) {
                target_x = aipPositionOnScreen.x - center_x;
                if (smooth != 0)
                    target_x /= smooth;
                if (target_x + center_x < 0.f) target_x = 0.f;
            }
        }
        if (aipPositionOnScreen.y != 0.f) {
            if (aipPositionOnScreen.y > center_y) {
                target_y = -(center_y - aipPositionOnScreen.y);
                if (smooth != 0)
                    target_y /= smooth;
                if (target_y + center_y > center_y * 2.f) target_y = 0.f;
            }

            if (aipPositionOnScreen.y < center_y) {
                target_y = aipPositionOnScreen.y - center_y;
                if (smooth != 0)
                    target_y /= smooth;
                if (target_y + center_y < 0.f) target_y = 0.f;
            }
        }
        input::moveMouseDxDy(target_x, target_y);
    } else {
        aiming = false;
    }
}

void toClipboard(HWND hwnd, const std::string &s) {
    OpenClipboard(hwnd);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

void render() {


    auto start = local_time::milliseconds();
    FVector screenSize = FVector((float) farlight.width, (float) farlight.height, 0);
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    aimFunction(aimBotFov, aimBotSmooth);
    for (const auto &e: entityList) {
        if (e.entityType == CHARACTER) {
            if (Character::isLocalPlayer(localPlayer, e)) {


                continue;
            }
            if (Character::isTeamWith(localPlayer, e))
                continue;
            if (Character::isDead(e))
                continue;


            auto characterPosition = Character::getWorldPosition(e);
            auto characterHeadPosition = Character::getWorldHeadPosition(e, 20);
            auto characterOnScreenBottom = Character::projectToScreen(localPlayer, characterPosition, screenSize);
            auto characterOnScreenTop = Character::projectToScreen(localPlayer, characterHeadPosition, screenSize);
            auto characterDistance = Character::getWorldPosition(localPlayer).distance(characterHeadPosition) / worldSettings.worldToMeters;
            auto characterHeight = characterOnScreenBottom.y - characterOnScreenTop.y;
            auto characterWidth = characterHeight * 0.5f;
            auto aimPosition = Character::predictAimPosition(localPlayer, e, worldSettings.globalGravityZ, worldSettings.worldToMeters);
            auto aipPositionOnScreen = Character::projectToScreen(localPlayer, aimPosition, screenSize);
            drawBox(
                    characterOnScreenBottom.x - characterWidth / 2,
                    characterOnScreenBottom.y - characterHeight,
                    characterWidth,
                    characterHeight,
                    Character::isBot(e) ? ImColor(255, 0, 0) : ImColor(20, 255, 20),
                    0.3f
            );
            float distanceScale = fmath::map(characterDistance, 0, 300, 1, 0);
            float healthBarWidth = 6 * distanceScale;
            float healthBarHeight = characterHeight;
            float healthScale = Character::getHealth(e) / Character::getMaxHealth(e);
            drawVerticalProgressBar(characterOnScreenBottom.x + characterWidth / 2 + 1, characterOnScreenBottom.y, healthBarWidth, healthBarHeight, healthScale, ImColor(255, static_cast<int>(255 * healthScale), static_cast<int>(255 * healthScale)));

            auto characterName = Character::isBot(e) ? "BOT" : Character::getNickname(e);
            characterName = characterName.substr(0, characterName.find('#'));
            characterName = strings::isStartWith(characterName, "[") ? characterName.substr(characterName.find(']') + 1, characterName.size()) : characterName;

            auto characterNameDraw = drawText(
                    characterName,
                    ImVec2(characterOnScreenBottom.x, characterOnScreenBottom.y - (characterHeight * 1.2f)),
                    {
                            ImColor(255, 255, 255),
                            CENTER,
                            fmax(10.0f, 16.0f * distanceScale),
                            true,
                            Character::isBot(e) ? ImColor(80, 23, 23, 120) : ImColor(23, 23, 23, 120),
                            8 * distanceScale,
                            8 * distanceScale
                    }
            );
            drawText(
                    std::to_string(static_cast<int>(characterDistance)) + " m",
                    ImVec2(characterNameDraw.x, characterNameDraw.y),
                    {
                            ImColor(255, 255, 255),
                            TOP_END,
                            fmax(10.0f, 12.0f * distanceScale),
                            false,
                            ImColor(0, 0, 0, 0),
                            0,
                            0
                    }
            );
            std::string weaponState;
            switch (Character::getActiveWeaponState(e)) {
                case Idle:
                    weaponState = "Idle";
                    break;
                case PreFire:
                    weaponState = "PreFire";
                    break;
                case RealFire:
                    weaponState = "RealFire";
                    break;
                case Rechamber:
                    weaponState = "Rechamber";
                    break;
                case Reloading:
                    weaponState = "Reloading";
                    break;
                case Bolting:
                    weaponState = "Bolting";
                    break;
                case Charging:
                    weaponState = "Charging";
                    break;
                case Overloading:
                    weaponState = "Overloading";
                    break;
                case KeyUPHolding:
                    weaponState = "KeyUPHolding";
                    break;
            }
            if (Character::holdingWeapon(e)) {
                drawText(
                        weaponIdToString(Character::getWeaponId(e)) + " - " + weaponState,
                        ImVec2(characterOnScreenBottom.x, characterOnScreenBottom.y + (characterHeight * 0.2f)),
                        {
                                ImColor(255, 255, 255),
                                CENTER,
                                fmax(10.0f, 16.0f * distanceScale),
                                true,
                                Character::holdingWeapon(e) ? ImColor(80, 23, 23, 120) : ImColor(80, 80, 23, 120),
                                8 * distanceScale,
                                8 * distanceScale
                        }
                );
            }
            drawLine(screenCenter.x, screenSize.y, characterOnScreenBottom.x, characterOnScreenBottom.y, Character::isBot(e) ? ImColor(255, 0, 0) : ImColor(255, 255, 255), 1);
            drawCircle(aipPositionOnScreen.x, aipPositionOnScreen.y, 3, ImColor(255, 0, 0), 30);
        } else if (e.entityType == ITEM) {
            auto itemData = Item::getItemData(e);
            auto itemPosition = Item::getPosition(e);
            auto itemPositionOnScreen = Character::projectToScreen(localPlayer, itemPosition, screenSize);
            auto itemDistance = Item::getDistance(localPlayer, e, worldSettings.worldToMeters);
            auto itemColor = Item::getItemColor(e);
            float distanceScale = fmath::map(itemDistance, 0, 300, 1, 0);
            drawText(
                    itemData.Name.toString(),
                    ImVec2(itemPositionOnScreen.x, itemPositionOnScreen.y),
                    {
                            ImColor(255, 255, 255),
                            CENTER,
                            fmax(10.0f, 16.0f * distanceScale),
                            true,
                            ImColor(itemColor.Value.x, itemColor.Value.y, itemColor.Value.z, 0.4f),
                            6 * distanceScale,
                            6 * distanceScale
                    }
            );
        } else if (e.entityType == ITEM_BOX) {
            auto boxPosition = Item::getPosition(e);
            auto boxPositionOnScreen = Character::projectToScreen(localPlayer, boxPosition, screenSize);
//            auto itemList = ItemBox::getItems(e);
//            auto boxColor = Item::getItemColor(ItemBox::getHighestItemQuality(e));
            auto items = ItemBox::getSortedItems(e);
            auto items2 = ItemBox::getItems(e);
            auto items3 = ItemBox::getBurstItems(e);
            string itemText = "Item box | Q:" + std::to_string(ItemBox::getHighestItemQuality(e)) + " - C1:" + std::to_string(items.length()) + " - C2:" + std::to_string(items2.length()) + " - C3:" + std::to_string(items3.size());
            switch (ItemBox::getState(e)) {
                case TreasureBoxState_None:
                    itemText += "None";
                    break;
                case TreasureBoxState_Close:
                    itemText += "Close";
                    break;
                case TreasureBoxState_Open:
                    itemText += "Open";
                    break;
            }
//            for (int i = 0; i < itemList.length(); ++i) {
//                auto itemData = itemList.getValue(i);
//                auto itemName = itemData.Name.toString();
//                auto itemColor = Item::getItemColor(itemData);
//                itemText += itemName + "\n";
//            }
            drawText(
                    itemText,
                    ImVec2(boxPositionOnScreen.x, boxPositionOnScreen.y),
                    {
                            ImColor(255, 255, 255),
                            CENTER,
                            12,
                            true,
                            ImColor(23, 23, 23, 120),
                            6,
                            6
                    }
            );
        } else if (e.entityType == DEATH_BOX) {
//            auto boxPosition = DeathBox::getPosition(e);
//            auto boxPositionOnScreen = Character::projectToScreen(localPlayer, boxPosition, screenSize);
//            auto energy = DeathBox::getEnergyValue(e);
//            auto energyExtra = DeathBox::getEnergyExtraValue(e);
//            auto itemList = DeathBox::getSortedItems(e);
//            string itemText = "Energy: "+std::to_string(energy)+"("+std::to_string(energyExtra)+")\n";
//            for (int i = 0; i < itemList.length(); ++i) {
//                auto itemData = itemList.getValue(i);
//                auto itemName = itemData.Name.toString();
//                auto itemColor = Item::getItemColor(itemData);
//                itemText += itemName + "\n";
//            }
//            drawText(
//                    itemText,
//                    ImVec2(boxPositionOnScreen.x, boxPositionOnScreen.y),
//                    {
//                            ImColor(255, 255, 255),
//                            CENTER,
//                            12,
//                            true,
//                            ImColor(23, 23, 23, 120),
//                            6,
//                            6
//                    }
//            );
        } else if (e.entityType == VEHICLE) {
            auto vehiclePosition = Vehicle::getPosition(e);
            auto vehiclePositionOnScreen = Character::projectToScreen(localPlayer, vehiclePosition, screenSize);
            drawCircleFilled(vehiclePositionOnScreen.x, vehiclePositionOnScreen.y, 3, ImColor(0, 255, 0), 30);
        }

    }


    drawThreadDelay = local_time::milliseconds() - start;
    std::string diagnosticText = "Read/Render - [" + std::to_string(readThreadDelay) + " ms / " + std::to_string(drawThreadDelay) + " ms]";
    TextOptions infoTextOptions{};
    infoTextOptions.padding = 12;
    infoTextOptions.withBackground = true;
    infoTextOptions.cornerRadius = 6;
    infoTextOptions.fontSize = 16;
    infoTextOptions.gravity = BOTTOM_END;
    infoTextOptions.color = ImColor(255, 80, 255, 255);
    infoTextOptions.backgroundColor = ImColor(60, 20, 60, 120);
    auto a = drawText("Farlight External v" + std::to_string(VERSION) + "." + std::to_string(SUB_VERSION), ImVec2(0, 0), infoTextOptions);

    infoTextOptions.color = ImColor(255, 255, 255, 255);
    if (aiming) {
        infoTextOptions.backgroundColor = ImColor(255, 23, 23, 120);
    } else {
        infoTextOptions.backgroundColor = ImColor(23, 23, 23, 120);
    }
    a = drawText("AIM", ImVec2(a.z, 0), infoTextOptions);
    infoTextOptions.backgroundColor = ImColor(23, 23, 23, 120);
    a = drawText(diagnosticText, ImVec2(a.z, 0), infoTextOptions);


}


void renderMenu() {
    drawSeparator();
    tabButton("ESP", &selectedTab, 0, true);
    tabButton("Debug", &selectedTab, 1, false);
    drawSeparator();
    ImGui::SliderFloat("AIM FOV", &aimBotFov, 20, 300);
    ImGui::SliderFloat("AIM Smooth", &aimBotSmooth, 0, 30);


    std::vector<const char *> items;
    items.reserve(unknownEntity.size());
    for (const auto &item: unknownEntity) {
        items.push_back(item.c_str());
    }
    ImGui::PushItemWidth(-1);
    ImGui::ListBox("Undefined entities", &selectedItem, items.data(), static_cast<int>(items.size()));
    if (ImGui::Button("Copy", ImVec2(100, 62))) {
        toClipboard(GetDesktopWindow(), unknownEntity.at(selectedItem));
    }

}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    print("[~] Loading vulnerable driver...");
    system("kdmapper.exe driver.sys >nul 2>nul");
    print("[~] Locating driver...");
    if (driver::find_driver()) {
        processId = driver::find_process("SolarlandClient-Win64-Shipping.exe");
        if (processId != -1) {
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
            print("[-] Game not found!");
        }
    } else {
        print("[-] Driver not found!");
    }
    Sleep(10000);
    return 0;
}
