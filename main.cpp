#include <iostream>
#include <windows.h>
#include <string>
#include "utils/driver.h"
#include "cheat/overlay.h"
#include "cheat/offsets.h"
#include "cheat/solarland.h"
#include "utils/input.h"
#include "utils/f_math.h"


typedef struct Entity {
    std::string name;
    DWORD_PTR acknowledgePawn;
    DWORD_PTR skeletonMesh;
    DWORD_PTR state;
    DWORD_PTR rootComponent;

    [[nodiscard]] FVector getBonePosition(int boneId) const {
        return getBoneWithRotation(skeletonMesh, boneId);
    }
} Entity;

int bone = 0;
float aimbotFov = 100.0f;
WorldSettings worldSettings;
SolarCharacter localPlayer;


std::vector<Entity> itemList;
std::vector<Entity> vehicleList;
std::vector<SolarCharacter> solarList;
uint64_t readThreadDelay;
uint64_t drawThreadDelay;

struct Pointers {
    DWORD_PTR uWorld;
    DWORD_PTR gameState;
    DWORD_PTR gameMode;
    DWORD_PTR owningGameInstance;
    DWORD_PTR persistentLevel;
    DWORD_PTR localPlayer;
    DWORD_PTR localPlayerPawn;
    DWORD_PTR localPlayerRoot;
    DWORD_PTR localPlayerState;
    DWORD_PTR localPlayerController;
    DWORD_PTR localTeamInfo;
    BYTE localPlayerTeamId;
    TArray<DWORD_PTR> entityList;;
    DWORD_PTR cameraManager;
    FCameraCacheEntry cameraCache;
};


typedef struct Bounds {
    float left;
    float top;
    float right;
    float bottom;
} Bounds;

typedef struct EspPlayer {
    float x;
    float y;
    int positionInTeam;
    bool isTeammate;
    bool isBot;
    bool isVisible;
} EspPlayer;


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


void print(const std::string &value) {
    std::cout << value << std::endl;
}


void aimFunction(float fov) {
    return;
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    FVector screenSize = FVector((float) farlight.width, (float) farlight.height, 0);
    auto localPlayerPosition = localPlayer.getWorldPosition();
    float dFromCrosshair = FLT_MAX;
    float dToPlayer = FLT_MAX;
    SolarCharacter crosshairCharacter;
    SolarCharacter distanceCharacter;
    SolarCharacter resultCharacter;
    for (const auto &character: solarList) {
        if (character.getId() == localPlayer.getId() || localPlayer.isMyTeammate(character) || character.isDead())
            continue;
        auto headPosition = character.getWorldHeadPosition();
        auto distance = localPlayerPosition.distanceIgnoreZ(headPosition);
        auto aimPosition = localPlayer.targetCorrection(
                worldSettings,
                distance,
                headPosition,
                character.getVelocity()
        );
        auto aimPositionSP = localPlayer.projectWorldToScreen(aimPosition, screenSize);
        auto dCrosshair = aimPositionSP.distance2D(screenCenter);
        if (((dFromCrosshair == FLT_MAX || dCrosshair < dFromCrosshair) && dCrosshair < fov) || !crosshairCharacter.isValid()) {
            dFromCrosshair = dCrosshair;
            crosshairCharacter = character;
        }
        if (dToPlayer == FLT_MAX || distance < dToPlayer) {
            dToPlayer = distance;
            distanceCharacter = character;
        }
    }
    //if(crosshairCharacter.isValid() && distanceCharacter.isValid()){
    //    auto d1 = localPlayerPosition.distanceIgnoreZ(crosshairCharacter.getWorldPosition());
    //    auto d2 = localPlayerPosition.distanceIgnoreZ(distanceCharacter.getWorldPosition());
    //    resultCharacter = d1 > d2 ? crosshairCharacter : distanceCharacter;
    //} else if(crosshairCharacter.isValid()){
    resultCharacter = crosshairCharacter;
    //} else if(distanceCharacter.isValid()){
    //    resultCharacter = distanceCharacter;
    //}
    if (resultCharacter.isValid()) {
        auto characterPosition = resultCharacter.getWorldPosition();
        auto characterHeadPosition = resultCharacter.getWorldHeadPosition();
        auto predictedAimPosition = localPlayer.targetCorrection(
                worldSettings,
                localPlayer.getWorldPosition().distance(characterPosition),
                characterHeadPosition,
                resultCharacter.getVelocity()
        );
        auto aimPositionSP = localPlayer.projectWorldToScreen(predictedAimPosition, screenSize);
        if (GetAsyncKeyState(VK_SHIFT) && !resultCharacter.isDead())
            move_to((float) farlight.width, (float) farlight.height, 10, aimPositionSP.x, aimPositionSP.y);
    }
}


[[noreturn]] void update() {
    while (true) {
        auto start = local_time::milliseconds();
        if (GetAsyncKeyState(VK_UP) & 1) {
            bone += 1;
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            bone -= 1;
        }
        std::vector<Entity> tmp_itemList;
        std::vector<Entity> tmp_playerList;
        std::vector<Entity> tmp_vehicleList;
        std::vector<SolarCharacter> tmp_solarList;
        pointers.uWorld = read<DWORD_PTR>(baseId + offsets::gWorld);
        pointers.gameState = read<DWORD_PTR>(baseId + offsets::wGameState);
        pointers.gameMode = read<DWORD_PTR>(baseId + offsets::wAuthorityGameMode);
        pointers.owningGameInstance = read<DWORD_PTR>(pointers.uWorld + offsets::wOwningGameInstance);
        pointers.persistentLevel = read<DWORD_PTR>(pointers.uWorld + offsets::wPersistentLevel);
        pointers.localPlayer = read<DWORD_PTR>(read<DWORD_PTR>(pointers.owningGameInstance + offsets::giLocalPlayers));
        localPlayer = SolarCharacter(pointers.localPlayer);
        pointers.localPlayerController = read<DWORD_PTR>(pointers.localPlayer + offsets::pPlayerController);
        pointers.localPlayerPawn = read<DWORD_PTR>(pointers.localPlayerController + offsets::pcAcknowledgedPawn);
        pointers.localPlayerRoot = read<DWORD_PTR>(pointers.localPlayerPawn + offsets::pRootComponent);
        pointers.localPlayerState = read<DWORD_PTR>(pointers.localPlayerPawn + offsets::pPlayerState);
        pointers.entityList = read<TArray<DWORD_PTR>>(pointers.persistentLevel + offsets::ncOwningActor);
        pointers.cameraManager = read<DWORD_PTR>(pointers.localPlayerController + offsets::pcPlayerCameraManager);
        pointers.cameraCache = read<FCameraCacheEntry>(pointers.cameraManager + offsets::pcmCameraCache);
        pointers.localTeamInfo = read<DWORD_PTR>(pointers.localPlayerState + offsets::spsTeam);
        pointers.localPlayerTeamId = read<char>(pointers.localTeamInfo + offsets::stiTeamID);
        for (int i = 0; i < pointers.entityList.length(); ++i) {
            auto pointer = pointers.entityList.getValuePointer(i);
            if (pointer == 0x0)
                continue;
            Entity entity{};
            entity.name = nameFromId(read<int>(pointer + offsets::oNamePrivate));

            entity.acknowledgePawn = pointer;
            entity.state = read<DWORD_PTR>(pointer + offsets::pPlayerState);
            entity.skeletonMesh = read<DWORD_PTR>(pointer + offsets::cMesh);
            entity.rootComponent = read<DWORD_PTR>(pointer + offsets::pRootComponent);
            if (EntityType::isPickup(entity.name)) {
                tmp_itemList.push_back(entity);
            }
            if (EntityType::isPlayer(entity.name)) {
                tmp_solarList.emplace_back(pointer, true);
                tmp_playerList.push_back(entity);
            }
            if (EntityType::isVehicle(entity.name)) {
                tmp_vehicleList.push_back(entity);
            }
            if (EntityType::isWorldSettings(entity.name)) {
                worldSettings.worldToMeters = read<float>(pointer + 0x02A0);
                worldSettings.worldGravityZ = read<float>(pointer + 0x02B0);
                worldSettings.globalGravityZ = read<float>(pointer + 0x02B4);
            }
        }
        itemList = tmp_itemList;
        vehicleList = tmp_vehicleList;
        solarList = tmp_solarList;
        aimFunction(aimbotFov);
        readThreadDelay = local_time::milliseconds() - start;
        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}


FVector visibleRadius(FVector position, float radius) {
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    float angle = screenCenter.angle(position);
    return screenCenter.direction(angle, radius);
}

bool isInRadius(FVector screenCenter, FVector position, float radius) {
    return screenCenter.distance2D(position) < radius;
}


void render() {
    auto start = local_time::milliseconds();
    FVector screenSize = FVector((float) farlight.width, (float) farlight.height, 0);
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);


    drawCircle(screenCenter.x, screenCenter.y, aimbotFov, ImColor(255, 255, 255), 1.0f);


    auto playerPosition = localPlayer.getWorldPosition();
    for (const auto &character: solarList) {
        //if (character.getId() == localPlayer.getId())
        //    continue;
        //if (localPlayer.isMyTeammate(character))
        //    continue;
        if (character.isDead())
            continue;

        character.getVelocity();

        auto characterPosition = character.getWorldPosition();
        auto characterHeadPosition = character.getWorldHeadPosition(20);
        auto characterOnScreenBottom = localPlayer.projectWorldToScreen(characterPosition, screenSize);
        auto characterOnScreenTop = localPlayer.projectWorldToScreen(characterHeadPosition, screenSize);
        auto characterHeight = characterOnScreenBottom.y - characterOnScreenTop.y;
        auto characterWidth = characterHeight * 0.5f;


        drawBox(
                characterOnScreenBottom.x - characterWidth / 2,
                characterOnScreenBottom.y - characterHeight,
                characterWidth,
                characterHeight,
                ImColor(255, 0, 255),
                0.3f
        );
        auto characterName = character.getName().toString();
        characterName = characterName.substr(0, characterName.find('#'));
        characterName = strings::isStartWith(characterName, "[") ? characterName.substr(characterName.find(']') + 1, characterName.size()) : characterName;
        std::string weaponState = "No weapon";
        switch (character.getActiveWeaponState()) {
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
        switch (character.getWeaponId()) {
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

        TextOptions options{};
        options.padding = 8;
        options.withBackground = false;
        options.color = character.isBot() ? ImColor(255, 80, 80, 255) : ImColor(80, 80, 255, 255);
        options.cornerRadius = 6;
        options.fontSize = 16;
        options.gravity = START;
        auto lastDraw = drawText(
                characterName,
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, characterOnScreenBottom.y - characterHeight),
                options
        );
        lastDraw = drawText(
                "Character id: "+std::to_string(character.getId()),
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );
        lastDraw = drawText(
                "Skin id: "+std::to_string(character.getSkinId()),
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );
        options.color = ImColor(255, 255, 0);
        lastDraw = drawText(
                "-------------",
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );
        lastDraw = drawText(
                "Weapon state: " + weaponState,
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );
        lastDraw = drawText(
                "Weapon name: " + weaponName,
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );
        lastDraw = drawText(
                "Weapon slot: " + std::to_string(character.getActiveWeaponSlot()),
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );
        lastDraw = drawText(
                std::to_string(localPlayer.getCameraPosition().distance(characterPosition) / worldSettings.worldToMeters) + "m",
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );
        options.color = ImColor(0, 120, 0);
        lastDraw = drawText(
                "-------------",
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );


        lastDraw = drawText(
                "Root location :" + character.getWorldPosition().debug(),
                ImVec2(characterOnScreenBottom.x - characterWidth / 2, lastDraw.w),
                options
        );

//        auto predictedAimPosition = localPlayer.targetCorrection(
//                worldSettings,
//                playerPosition.distance(characterPosition),
//                characterHeadPosition,
//                character.getVelocity()
//        );
//        auto characterRootSP = localPlayer.projectWorldToScreen(characterPosition, screenSize);
//        auto characterHeadSP = localPlayer.projectWorldToScreen(characterHeadPosition, screenSize);
//        auto aimPositionSP = localPlayer.projectWorldToScreen(predictedAimPosition, screenSize);
//
//        ImColor lineColor = ImColor(102, 178, 255);
//        if (character.isBot())
//            lineColor = ImColor(255, 51, 51);
//        if (localPlayer.isMyTeammate(character))
//            lineColor = ImColor(178, 255, 102);
//        std::string name = character.isBot() ? "BOT" : character.getName().toString();
//        std::string playerHint = name.substr(0, name.find('#')) + " [" + std::to_string(static_cast<int>(playerPosition.distance(characterPosition))) + "m]";
//        drawText(playerHint, characterRootSP.x, characterRootSP.y, 14.0f, lineColor, true);
//        FVector startPoint = visibleRadius(aimPositionSP, aimbotFov);
//        if (isInRadius(screenCenter, aimPositionSP, aimbotFov)) {
//            drawCircleFilled(aimPositionSP.x, aimPositionSP.y, 2.0f, lineColor, 30);
//            drawCircleFilled(characterRootSP.x, characterRootSP.y, 2.0f, lineColor, 30);
//        } else {
//            drawLine(startPoint.x, startPoint.y, aimPositionSP.x, aimPositionSP.y, ImColor(0, 0, 0), 1.5f);
//            drawLine(startPoint.x, startPoint.y, aimPositionSP.x, aimPositionSP.y, lineColor, 1.0f);
//            drawCircleFilled(startPoint.x, startPoint.y, 2.0f, lineColor, 30);
//            drawCircleFilled(aimPositionSP.x, aimPositionSP.y, 2.0f, lineColor, 30);
//        }

//        drawText(weaponState, characterRootSP.x, characterRootSP.y + 40, 14.0f, lineColor, true);
//        drawText(std::to_string(character.getSkinId()), characterRootSP.x, characterRootSP.y + 50, 14.0f, lineColor, true);


//
//        auto barWidth = 250;
//        auto barHeight = 32;
//        auto left = characterRootSP.x - (float) barWidth / 2.0f;
//        auto right = left + (float) barWidth;
//        auto top = characterHeadSP.y + characterHeight;
//        auto bottom = top + (float) barHeight;
//        FVector weaponIconPos = FVector(left + 24, top + 16, 0);

//
//        std::string distance = std::to_string(static_cast<int>(playerPosition.distanceMeters(characterPosition))) + "m";
//        ImVec2 weaponTextSize = Overlay::DroidSans->CalcTextSizeA(14, FLT_MAX, 0.0f, weaponName.c_str());
//        FVector weaponTextPos = FVector(weaponIconPos.x + 20, weaponIconPos.y - weaponTextSize.y / 2, 0);
//        ImVec2 distanceTextSize = Overlay::DroidSans->CalcTextSizeA(14, FLT_MAX, 0.0f, distance.c_str());
//        FVector distanceTextPos = FVector(weaponTextPos.x + weaponTextSize.x + 16, weaponIconPos.y - weaponTextSize.y / 2, 0);
//
//
//        drawRectFilled(
//                left,
//                top,
//                weaponTextPos.x + weaponTextSize.x + 8,
//                bottom,
//                ImColor(255, 23, 23, 30), 8, ImDrawFlags_RoundCornersAll);
//
//
//        drawRectFilled(
//                weaponTextPos.x + weaponTextSize.x + 8,
//                top,
//                distanceTextPos.x + distanceTextSize.x + 8,
//                bottom,
//                ImColor(23, 255, 23, 30), 8, ImDrawFlags_RoundCornersAll);
//
//        drawImage(weaponIcon, weaponIconPos.x, weaponIconPos.y, 32, 16, true);
//        drawText(weaponName, weaponTextPos.x, weaponTextPos.y, 14, ImColor(255, 255, 255), false);
//        drawText(distance, distanceTextPos.x, distanceTextPos.y, 14, ImColor(255, 255, 255), false);
//
//
//        drawLine(characterHeadSP.x, characterHeadSP.y, aimPositionSP.x, aimPositionSP.y, ImColor(0, 0, 0), 3);
//        drawLine(characterHeadSP.x, characterHeadSP.y, aimPositionSP.x, aimPositionSP.y, ImColor(255, 0, 0, 120), 2.5);
//
//        drawCircleFilled(characterRootSP.x, characterRootSP.y, 4, ImColor(255, 255, 255), 30);
//        //drawCircleFilled(characterHeadSP.x, characterHeadSP.y, 4, ImColor(255, 255, 0), 30);
//        drawCircleFilled(aimPositionSP.x, aimPositionSP.y, 3, ImColor(0, 0, 0), 30);
//        drawCircleFilled(aimPositionSP.x, aimPositionSP.y, 2.5, ImColor(255, 0, 0), 30);
////
////        drawCircleFilled(predictedAimScreenPosition.x, predictedAimScreenPosition.y, 3, ImColor(255, 0, 255), 30);
////        drawCircleFilled(characterScreenPosition.x, characterScreenPosition.y, 3, ImColor(0, 0, 255), 30);
////
//        drawText("Bot " + std::to_string(character.isBot()), characterHeadSP.x, characterHeadSP.y - 50, 14, ImColor(255, 255, 0), true);
////        drawText("Eye "+std::to_string(character.getEyeHeight()), characterScreenPosition.x, characterScreenPosition.y - 70, 14, ImColor(255, 255, 0), true);
//
////        switch (pointers.player.getWeaponConfig().weaponType) {
////            case EWeaponType::WeaponType_AssaultRifle:
////                drawText("Assault Rifle", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_SubMachineGun:
////                drawText("SubMachineGun", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_Shotgun:
////                drawText("Shotgun", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_Sniper:
////                drawText("Sniper Rifle", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_VehicleMounted:
////                drawText("Vehicle Mounted", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_ItemWeapon:
////                drawText("Item Weapon", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_SummonWeapon:
////                drawText("Summon Weapon", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_AntiVehicle:
////                drawText("Anti Vehicle", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_UnArm:
////                drawText("UnArm", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////            case WeaponType_Unknown:
////                drawText("Unknown", characterScreenPosition.x, characterScreenPosition.y - 30, 14, ImColor(255, 255, 0), true);
////                break;
////        }
    }


    drawThreadDelay = local_time::milliseconds() - start;
    std::string diagnosticText = "Read/Render - [" + std::to_string(readThreadDelay) + " ms / " + std::to_string(drawThreadDelay) + " ms]";
    drawText(diagnosticText, ImVec2(0, 0), ImColor(255, 255, 255), 14, false, true, ImColor(23, 23, 23, 120), 16, true, 8);
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
