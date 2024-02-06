#include <iostream>
#include <windows.h>
#include <string>
#include "utils/driver.h"
#include "cheat/overlay.h"
#include "cheat/solarland.h"
#include "utils/f_math.h"

#define VERSION 1
#define SUB_VERSION 0

int bone = 0;
float aimbotFov = 100.0f;
WorldSettings worldSettings;
SolarCharacter localPlayer;


std::vector<SolarCharacter> playerList;
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


ImColor qualityColors[7] = {
        ImColor(255, 255, 255),
        ImColor(255, 255, 255),
        ImColor(28, 184, 76),
        ImColor(48, 187, 216),
        ImColor(214, 109, 219),
        ImColor(230, 112, 55),
        ImColor(217, 37, 18)
};

Pointers pointers;
DWORD offset = 0;

void print(const std::string &value) {
    std::cout << value << std::endl;
}


[[noreturn]] void update() {
    while (true) {
        auto start = local_time::milliseconds();
        std::vector<SolarCharacter> tmp_solarList;
        pointers.uWorld = read<DWORD_PTR>(baseId + offsets::gWorld);
        pointers.gameState = read<DWORD_PTR>(pointers.uWorld + offsets::wGameState);
        pointers.gameMode = read<DWORD_PTR>(pointers.uWorld + offsets::wAuthorityGameMode);
        pointers.owningGameInstance = read<DWORD_PTR>(pointers.uWorld + offsets::wOwningGameInstance);
        pointers.persistentLevel = read<DWORD_PTR>(pointers.uWorld + offsets::wPersistentLevel);
        localPlayer = SolarCharacter(read<DWORD_PTR>(read<DWORD_PTR>(pointers.owningGameInstance + offsets::giLocalPlayers)), false);
        pointers.entityList = read<TArray<DWORD_PTR>>(pointers.persistentLevel + offsets::ncOwningActor);
        for (int i = 0; i < pointers.entityList.length(); ++i) {
            auto pointer = pointers.entityList.getValuePointer(i);
            if (pointer == 0x0)
                continue;
            auto name = UObject::getClassName(pointer);
            if (EntityType::isPickup(name)) {

            }
            if (EntityType::isPlayer(name)) {
                tmp_solarList.emplace_back(pointer, true);
            }
            if (EntityType::isVehicle(name)) {

            }
            if (EntityType::isWorldSettings(name) || !worldSettings.isValid())
                worldSettings = WorldSettings(pointer);
        }
        playerList = tmp_solarList;
        readThreadDelay = local_time::milliseconds() - start;

    }
}


void aimAt(SolarCharacter c, float smooth) {
    if (c.getId() == localPlayer.getId())
        return;
    //if (localPlayer.isMyTeammate(c))
    //    return;
    if (c.isDead())
        return;
    auto characterHeadPosition = c.getWorldHeadPosition(5);
    auto characterDistance = localPlayer.getWorldHeadPosition().distance(characterHeadPosition) / worldSettings.worldToMeters;
    auto aimPosition = localPlayer.targetCorrection(worldSettings, characterDistance, characterHeadPosition, c.getVelocity());
    auto aipPositionOnScreen = localPlayer.projectWorldToScreen(aimPosition, FVector((float) farlight.width, (float) farlight.height, 0));
    if (GetAsyncKeyState(VK_SHIFT)) {
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
    }
}


void aimFunction(float fov, float smooth) {
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    SolarCharacter targetCharacter;
    float lastDistanceFromCrossHair = FLT_MAX;
    for (const auto &c: playerList) {
        auto characterHeadPosition = c.getWorldHeadPosition(5);
        auto characterHeadPositionOnScreen = localPlayer.projectWorldToScreen(characterHeadPosition, FVector((float) farlight.width, (float) farlight.height, 0));
        auto distanceToCrosshair = screenCenter.distance2D(characterHeadPositionOnScreen);
        if (distanceToCrosshair < lastDistanceFromCrossHair && distanceToCrosshair < fov) {
            targetCharacter = c;
            lastDistanceFromCrossHair = distanceToCrosshair;
        }
    }
    drawCircle(screenCenter.x, screenCenter.y, fov, ImColor(255, 255, 255), 30);
    if (targetCharacter.isValid()) {
        aimAt(targetCharacter, smooth);
    }
}

void render() {
    auto start = local_time::milliseconds();
    FVector screenSize = FVector((float) farlight.width, (float) farlight.height, 0);
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    aimFunction(150, 0);
    for (const auto &c: playerList) {
        if (c.getId() == localPlayer.getId())
            continue;
        //if (localPlayer.isMyTeammate(c) && c.getId() != localPlayer.getId())
        //    continue;
        if (c.isDead())
            continue;
        auto characterPosition = c.getWorldPosition();
        auto characterHeadPosition = c.getWorldHeadPosition(20);
        auto characterOnScreenBottom = localPlayer.projectWorldToScreen(characterPosition, screenSize);
        auto characterOnScreenTop = localPlayer.projectWorldToScreen(characterHeadPosition, screenSize);
        auto characterDistance = localPlayer.getWorldHeadPosition().distance(characterHeadPosition) / worldSettings.worldToMeters;
        auto characterHeight = characterOnScreenBottom.y - characterOnScreenTop.y;
        auto characterWidth = characterHeight * 0.5f;
        auto aimPosition = localPlayer.targetCorrection(worldSettings, characterDistance, c.getWorldHeadPosition(5), c.getVelocity());
        auto aipPositionOnScreen = localPlayer.projectWorldToScreen(aimPosition, screenSize);;
        drawBox(
                characterOnScreenBottom.x - characterWidth / 2,
                characterOnScreenBottom.y - characterHeight,
                characterWidth,
                characterHeight,
                c.isBot() ? ImColor(255, 0, 0) : ImColor(20, 255, 20),
                0.3f
        );
        float distanceScale = fmath::map(characterDistance, 0, 150, 1, 0);
        float healthBarWidth = 6 * distanceScale;
        float healthBarHeight = characterHeight;
        float healthScale = c.getHealth() / c.getMaxHealth();
        drawVerticalProgressBar(characterOnScreenBottom.x + characterWidth / 2 + 1, characterOnScreenBottom.y, healthBarWidth, healthBarHeight, healthScale, ImColor(255, static_cast<int>(255 * healthScale), static_cast<int>(255 * healthScale)));

        auto characterName = c.isBot() ? "BOT" : c.getName().toString();
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
                        c.isBot() ? ImColor(80, 23, 23, 120) : ImColor(23, 23, 23, 120),
                        8 * distanceScale,
                        8 * distanceScale
                }
        );
        drawText(
                std::to_string(static_cast<int>(localPlayer.getWorldHeadPosition().distance(characterHeadPosition) / worldSettings.worldToMeters)) + " m",
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


        std::string weaponState = "";
        switch (c.getActiveWeaponState()) {
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

        std::string weaponName = "No weapon";
        PDIRECT3DTEXTURE9 weaponIcon = Icons::Weapon::arm;
        switch (c.getWeaponId()) {
            case EWeaponId::EWeaponId_Invader:
                weaponIcon = Icons::Weapon::invader;
                weaponName = "Invader";
                break;
            case EWeaponId::EWeaponId_Hound:
                weaponIcon = Icons::Weapon::hound;
                weaponName = "Hound";
                break;
            case EWeaponId::EWeaponId_WhiteDwarf:
                weaponIcon = Icons::Weapon::whiteDwarf;
                weaponName = "White dwarf";
                break;
            case EWeaponId::EWeaponId_Bar95:
                weaponIcon = Icons::Weapon::bar95;
                weaponName = "Bar-95";
                break;
            case EWeaponId::EWeaponId_Defender:
                weaponIcon = Icons::Weapon::defender;
                weaponName = "Defender";
                break;
            case EWeaponId::EWeaponId_Dikobraz:
                weaponIcon = Icons::Weapon::dikobraz;
                weaponName = "Porcupine";
                break;
            case EWeaponId::EWeaponId_Generator:
                weaponIcon = Icons::Weapon::generator;
                weaponName = "Generator";
                break;
            case EWeaponId::EWeaponId_MF18:
                weaponIcon = Icons::Weapon::MF18;
                weaponName = "MF-18";
                break;
            case EWeaponId::EWeaponId_M4:
                weaponIcon = Icons::Weapon::M4A;
                weaponName = "M4";
                break;
            case EWeaponId::EWeaponId_UMP99:
                weaponIcon = Icons::Weapon::UMP99;
                weaponName = "UMP-99";
                break;
            case EWeaponId::EWeaponId_StellarWind:
                weaponIcon = Icons::Weapon::stellarWind;
                weaponName = "Stellar wind";
                break;
            case EWeaponId::EWeaponId_Fanatic:
                weaponIcon = Icons::Weapon::fanatic;
                weaponName = "Fanatic";
                break;
            case EWeaponId::EWeaponId_AK:
                weaponIcon = Icons::Weapon::AK;
                weaponName = "AK";
                break;
            case EWeaponId::EWeaponId_MadRabbit:
                weaponIcon = Icons::Weapon::madRabbit;
                weaponName = "Mad rabbit";
                break;
            case EWeaponId::EWeaponId_Jupiter:
                weaponIcon = Icons::Weapon::jupiter;
                weaponName = "Jupiter";
                break;
            case EWeaponId::EWeaponId_MadRat:
                weaponIcon = Icons::Weapon::madRat;
                weaponName = "Mad rat";
                break;
            case EWeaponId::EWeaponId_VSS:
                weaponIcon = Icons::Weapon::vss;
                weaponName = "VSS";
                break;
            case EWeaponId::EWeaponId_Rhino:
                weaponIcon = Icons::Weapon::rhino;
                weaponName = "Rhino";
                break;
            case EWeaponId::EWeaponId_Vega:
                weaponIcon = Icons::Weapon::vega;
                weaponName = "Vega";
                break;
            case EWeaponId::EWeaponId_MG7:
                weaponIcon = Icons::Weapon::MG7;
                weaponName = "MG7";
                break;
            case EWeaponId::EWeaponId_UZI:
                weaponIcon = Icons::Weapon::UZI;
                weaponName = "UZI";
                break;
        }

        drawText(
                weaponIdToString(c.getWeaponId()) + (c.getActiveWeaponSlot() == -1 ? "" : " [" + std::to_string(c.getRemainAmmo()) + "]") + "{" + weaponState + "}",
                ImVec2(characterOnScreenBottom.x, characterOnScreenBottom.y + (characterHeight * 0.2f)),
                {
                        ImColor(255, 255, 255),
                        CENTER,
                        fmax(10.0f, 16.0f * distanceScale),
                        true,
                        c.getActiveWeaponSlot() == -1 ? ImColor(80, 23, 23, 120) : ImColor(80, 80, 23, 120),
                        8 * distanceScale,
                        8 * distanceScale
                }
        );
        drawLine(screenCenter.x, screenSize.y, characterOnScreenBottom.x, characterOnScreenBottom.y, c.isBot() ? ImColor(255, 0, 0) : ImColor(255, 255, 255), 1);
        drawCircle(aipPositionOnScreen.x, aipPositionOnScreen.y, 3, ImColor(255, 0, 0), 30);

//
//        TextOptions options{};
//        options.padding = 8;
//        options.withBackground = false;
//        options.color = c.isBot() ? ImColor(255, 80, 80, 255) : ImColor(80, 255, 255, 255);
//        options.cornerRadius = 6;
//        options.fontSize = 16;
//        options.gravity = START;
//        auto lastDraw = drawText(
//                characterName,
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, characterOnScreenBottom.y - characterHeight),
//                options
//        );
//        lastDraw = drawText(
//                "Character id: " + std::to_string(c.getId()),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Skin id: " + std::to_string(c.getSkinId()),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Health: " + std::to_string(static_cast<int>(c.getHealth())) + "/" + std::to_string(static_cast<int>(c.getMaxHealth())),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        options.color = ImColor(255, 255, 0);
//        lastDraw = drawText(
//                "-------------",
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Weapon state: " + weaponState,
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Weapon name: " + weaponName,
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Weapon slot: " + std::to_string(c.getActiveWeaponSlot()),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Remain ammo: " + std::to_string(c.getRemainAmmo()),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Detected enemy: " + std::to_string(c.isWeaponDetectEnemy()),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//
//        options.color = ImColor(255, 120, 0);
//        lastDraw = drawText(
//                "-------------",
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Location :" + c.getWorldPosition().debug(),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Velocity: " + c.getVelocity().debug(worldSettings.worldToMeters),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Acceleration: " + c.getAcceleration().debug(worldSettings.worldToMeters),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        lastDraw = drawText(
//                "Distance: " + std::to_string(localPlayer.getWorldHeadPosition().distance(characterHeadPosition) / worldSettings.worldToMeters) + "m",
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        options.color = ImColor(0, 120, 20);
//        lastDraw = drawText(
//                "-------------",
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
//        auto worldTimeSeconds = read<float>(pointers.uWorld + 0x618);
//        lastDraw = drawText(
//                "Recently rendered: " + std::to_string(c.wasRecentlyRendered(pointers.uWorld, 0.2f)),
//                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
//                options
//        );
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
    infoTextOptions.backgroundColor = ImColor(23, 23, 23, 120);
    a = drawText(diagnosticText, ImVec2(a.z, 0), infoTextOptions);
}


void renderMenu() {
    drawSeparator();
    tabButton("ESP", &selectedTab, 0, true);
    tabButton("Debug", &selectedTab, 1, false);
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
        printf("UWORLD: %llX\n", baseId + offsets::gWorld);
        printf("UOBJECT: %llX\n", baseId + offsets::gObject);
        printf("UNAMES: %llX\n", baseId + offsets::gNames);
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
