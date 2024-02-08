#include <iostream>
#include <windows.h>
#include <string>
#include "utils/driver.h"
#include "cheat/overlay.h"
#include "cheat/solarland.h"
#include "cheat/config.h"
#include "utils/f_math.h"
#include <shlobj.h>

#define VERSION 1
#define SUB_VERSION 1

WorldSettings worldSettings;
GameEntity localPlayer;


std::vector<GameEntity> entityList;
std::vector<std::string> unknownEntity;

bool mouseDisabled = false;
bool aiming = false;
GameEntity targetCharacter;

uint64_t readThreadDelay;
uint64_t drawThreadDelay;
static float valuesRead[200] = {};
static int valuesReadOffset = 0;
static float valuesDraw[200] = {};
static int valuesDrawOffset = 0;


void print(const std::string &value) {
    std::cout << value << std::endl;
}


[[noreturn]] void update() {
    while (true) {
        auto start = local_time::milliseconds();
        std::vector<GameEntity> tmp_entityList;
        std::vector<std::string> tmp_unknownEntity;
        auto uWorld = read<DWORD_PTR>(baseId + offsets::gWorld);
        auto gameState = read<DWORD_PTR>(uWorld + offsets::wGameState);
        auto gameMode = read<DWORD_PTR>(uWorld + offsets::wAuthorityGameMode);
        auto owningGameInstance = read<DWORD_PTR>(uWorld + offsets::wOwningGameInstance);
        auto persistentLevel = read<DWORD_PTR>(uWorld + offsets::wPersistentLevel);
        localPlayer = GameEntity(read<DWORD_PTR>(read<DWORD_PTR>(owningGameInstance + offsets::giLocalPlayers)), LOCAL_CHARACTER);
        auto list = read<TArray<DWORD_PTR>>(persistentLevel + offsets::ncOwningActor);
        for (int i = 0; i < list.length(); ++i) {
            auto pointer = list.getValuePointer(i);
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
        valuesRead[valuesReadOffset] = readThreadDelay;
        valuesReadOffset = (valuesReadOffset + 1) % IM_ARRAYSIZE(valuesRead);
        if (Overlay::show && !mouseDisabled){
            input::holdKey(0x4F);
            mouseDisabled = true;
        } else if(!Overlay::show && mouseDisabled){
            input::holdKey(0x4F);
            mouseDisabled = false;
        }
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
    if (config::aimbot::showFov)
        drawCircle(screenCenter.x, screenCenter.y, fov, ImColor(255, 255, 255), 180);
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
    if (input::getAsyncKeyState(VK_SHIFT)) {
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
    if (config::aimbot::enable)
        aimFunction(config::aimbot::fov, config::aimbot::smoothing);
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
            //auto characterWasRendered = driver::readBoolean(e.skeletonMesh + 0x0727, 6);
            auto characterWasRendered = driver::readBoolean(e.rootComponent + 0x014C, 5);
            drawBox(
                    characterOnScreenBottom.x - characterWidth / 2,
                    characterOnScreenBottom.y - characterHeight,
                    characterWidth,
                    characterHeight,
                    characterWasRendered ? ImColor(255, 0, 0) : ImColor(20, 255, 20),
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
            if (config::esp::showSnapLines)
                drawLine(screenCenter.x, screenSize.y, characterOnScreenBottom.x, characterOnScreenBottom.y, Character::isBot(e) ? ImColor(255, 0, 0) : ImColor(255, 255, 255), 1);
            drawCircle(aipPositionOnScreen.x, aipPositionOnScreen.y, 3, ImColor(255, 0, 0), 30);
        } else if (e.entityType == ITEM) {
            auto itemData = Item::getItemData(e);
            if (itemData.Quality < config::esp::items::qualityFilter)
                continue;
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

    bool renderedMesh = driver::readBoolean(localPlayer.skeletonMesh + 0x0727, 6);
    bool renderedComp = driver::readBoolean(localPlayer.rootComponent + 0x014C, 5);
    a = drawText("Was rendered " + std::to_string(renderedMesh) + " - " + std::to_string(renderedComp), ImVec2(a.z, 0), infoTextOptions);


    valuesDraw[valuesDrawOffset] = drawThreadDelay;
    valuesDrawOffset = (valuesDrawOffset + 1) % IM_ARRAYSIZE(valuesDraw);
}


void renderMenu() {
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("CheatBar", tab_bar_flags)) {
        if (ImGui::BeginTabItem("ESP")) {
            ImGui::Checkbox("Enable ESP", &config::esp::enable);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Aimbot")) {
            ImGui::Checkbox("Enable aimbot", &config::aimbot::enable);
            if (config::aimbot::enable) {
                ImGui::Separator();
                ImGui::Combo("Type", &config::aimbot::type, "Distance to crosshair\0Distance to player\0");
                ImGui::Combo("Target", &config::aimbot::target, "Head\0Neck\0Chest\0Random\0");
                ImGui::Separator();
                ImGui::SliderFloat("Field of view", &config::aimbot::fov, 20, 300);
                ImGui::SliderFloat("Smooth", &config::aimbot::smoothing, 0, 30);
                ImGui::Separator();
                ImGui::Checkbox("Show field of view", &config::aimbot::showFov);
                ImGui::Checkbox("Predict aim position by distance and velocity", &config::aimbot::predictPosition);
                ImGui::Checkbox("Draw dot at predicted position", &config::aimbot::showPredictedPosition);
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Triggerbot")) {
            ImGui::Checkbox("Enable triggerbot", &config::triggerbot::enable);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Debug")) {
            ImGui::Text("This is not enabled in release build");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Performance")) {
            char entitySize[32];
            sprintf(entitySize, "Processing %d entities", static_cast<int>(entityList.size()));
            ImGui::Text(entitySize);
            char entityUndefinedSize[32];
            sprintf(entityUndefinedSize, "Unprocessed %d entities", static_cast<int>(unknownEntity.size()));
            ImGui::Text(entityUndefinedSize);

            if (ImGui::CollapsingHeader("Unprocessed entities", ImGuiTreeNodeFlags_None)) {
                for (const auto &item: unknownEntity)
                    ImGui::Text(item.c_str());
            }

            float averageRead = 0.0f;
            for (float value: valuesRead)
                averageRead += value;
            averageRead /= (float) IM_ARRAYSIZE(valuesRead);
            char overlayRead[32];
            sprintf(overlayRead, "Read average %f ms", averageRead);
            ImGui::PlotLines("", valuesRead, IM_ARRAYSIZE(valuesRead), valuesReadOffset, overlayRead, 0, 30, ImVec2(-1, 80.0f));
            float averageDraw = 0.0f;
            for (float value: valuesDraw)
                averageDraw += value;
            averageDraw /= (float) IM_ARRAYSIZE(valuesDraw);
            char overlayDraw[32];
            sprintf(overlayDraw, "Draw average %f ms", averageDraw);
            ImGui::PlotLines("", valuesDraw, IM_ARRAYSIZE(valuesDraw), valuesDrawOffset, overlayDraw, 0, 30, ImVec2(-1, 80.0f));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("About")) {
            ImGui::Text("This cheat was made for fun. If you got pay for them - you got scammed!\nDeveloper: LineR");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }


    //std::vector<const char *> items;
    //items.reserve(unknownEntity.size());
    //for (const auto &item: unknownEntity) {
    //    items.push_back(item.c_str());
    //}
    //ImGui::PushItemWidth(-1);
    //ImGui::ListBox("Undefined entities", &selectedItem, items.data(), static_cast<int>(items.size()));
    //if (ImGui::Button("Copy", ImVec2(100, 62))) {
    //    toClipboard(GetDesktopWindow(), unknownEntity.at(selectedItem));
    //}

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
            print("[~] Cleaning EAC");
            wchar_t *appdataRoaming;
            if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &appdataRoaming) != S_OK) {
                print("[-] Failed to found path");
            } else {
                auto appDataPath = strings::toString(appdataRoaming);
                auto eacFolder = appDataPath + "\\EasyAntiCheat";
                auto gmeFolder = appDataPath + "\\GMEGLOBAL";
                if (directory_delete(eacFolder.c_str())) {
                    print("[+] Deleted: " + eacFolder);
                } else {
                    print("[-] Failed to delete: " + eacFolder);
                }
                if (directory_delete(gmeFolder.c_str())) {
                    print("[+] Deleted: " + gmeFolder);
                } else {
                    print("[-] Failed to delete: " + gmeFolder);
                }
            }
            wchar_t *appdataLocal;
            if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, nullptr, &appdataLocal) != S_OK) {
                print("[-] Failed to found path");
            } else {
                auto appDataPath = strings::toString(appdataLocal);
                auto solarFolder = appDataPath + "\\Solarland";
                if (directory_delete(solarFolder.c_str())) {
                    print("[+] Deleted: " + solarFolder);
                } else {
                    print("[-] Failed to delete: " + solarFolder);
                }
            }
        }
    } else {
        print("[-] Driver not found!");
    }
    Sleep(10000);
    return 0;
}
