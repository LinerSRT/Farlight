#include <iostream>
#include <windows.h>
#include <string>
#include "utils/driver.h"
#include "cheat/overlay.h"
#include "cheat/offsets.h"
#include "ue/engine.h"
#include "utils/input.h"
#include "utils/f_math.h"

int bone = 0;

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
TUObjectArray* ObjObjects = nullptr;
std::vector<Entity> itemList;
std::vector<Entity> playerList;

typedef struct PlayerLocation {
    FVector rootPosition;
    FVector rootScreenPosition;
    FVector headPosition;
    FVector headScreenPosition;

    explicit PlayerLocation(const Entity &entity) {
        BonesType::Bones playerBones(BonesType::newSkin(read<int>(entity.state + offsets::spsSkinId)));
        rootPosition = entity.getBonePosition(playerBones.Root);
        rootScreenPosition = projectWorldToScreen(pointers.localPlayerController, rootPosition, FVector((float) farlight.width, (float) farlight.height, 0));
        headPosition = entity.getBonePosition(playerBones.head);
        headPosition.z += 10;
        headScreenPosition = projectWorldToScreen(pointers.localPlayerController, headPosition, FVector((float) farlight.width, (float) farlight.height, 0));
    }

    [[nodiscard]] float distance() const {
        return read<FVector>(pointers.localPlayerRoot + offsets::scRelativeLocation).distance(headPosition);
    }
} PlayerLocation;

void print(const std::string &value) {
    std::cout << value << std::endl;
}

auto espLine(EspPlayer player) -> VOID {
    ImColor lineColor = ImColor(0.14f, 0.14f, 0.14f, 0.5f);
    //if (player.teamId == pointers.localPlayerTeamId)
    //    lineColor = ImColor(0.14f, 1.0f, 0.14f, 0.5f);
    //if (player.isBot)
    //    lineColor = ImColor(1.0f, 0.14f, 0.14f, 0.5f);
    //if(player.visible)
    //    lineColor = ImColor(1.0f, 1.0f, 0.14f, 0.5f);
    if (player.isTeammate)
        lineColor = ImColor(0.14f, 1.0f, 0.14f, 0.5f);

}


[[noreturn]] void update() {
    while (true) {
        if (GetAsyncKeyState(VK_UP) & 1) {
            bone += 1;
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            bone -= 1;
        }
        std::vector<Entity> tmp_itemList;
        std::vector<Entity> tmp_playerList;
        ObjObjects = read<TUObjectArray*>(baseId + offsets::gObject);
        pointers.uWorld = read<DWORD_PTR>(baseId + offsets::gWorld);
        pointers.gameState = read<DWORD_PTR>(baseId + offsets::wGameState);
        pointers.gameMode = read<DWORD_PTR>(baseId + offsets::wAuthorityGameMode);
        pointers.owningGameInstance = read<DWORD_PTR>(pointers.uWorld + offsets::wOwningGameInstance);
        pointers.persistentLevel = read<DWORD_PTR>(pointers.uWorld + offsets::wPersistentLevel);
        pointers.localPlayer = read<DWORD_PTR>(read<DWORD_PTR>(pointers.owningGameInstance + offsets::giLocalPlayers));
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
            auto entityPawn = pointers.entityList.getValuePointer(i);
            if (entityPawn == 0x0)
                continue;
            Entity entity{};
            int id = read<int>(entityPawn + offsets::oNamePrivate);
            entity.name = nameFromId(baseId, id);
            entity.acknowledgePawn = entityPawn;
            entity.state = read<DWORD_PTR>(entityPawn + offsets::pPlayerState);
            entity.skeletonMesh = read<DWORD_PTR>(entityPawn + offsets::cMesh);
            entity.rootComponent = read<DWORD_PTR>(entityPawn + offsets::pRootComponent);
            if (
                    entity.name.find("SolarItemActor") != std::string::npos ||
                    entity.name.find("SolarGroundPreviewActor") != std::string::npos ||
                    entity.name.find("ASolarTreasureBoxActor") != std::string::npos
                    ) {
                tmp_itemList.push_back(entity);
            } else if (entity.name.find("BP_Character") != std::string::npos || entity.name.find("ABP_SolarCharacterPlayer_C") != std::string::npos) {
                tmp_playerList.push_back(entity);
            }
        }
        itemList = tmp_itemList;
        playerList = tmp_playerList;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

Entity findClosestEnemy() {
    float distance = 10000;
    Entity closestEntity;
    for (const auto &entity: playerList) {
        if (!entity.skeletonMesh || !entity.state || !entity.acknowledgePawn)
            continue;
        auto playerLocation = PlayerLocation(entity);
        auto distanceToPlayer = playerLocation.distance();
        auto playerId = read<int>(entity.state + 0x022C);
        auto localPlayerId = read<int>(pointers.localPlayerState + 0x022C);
        auto teamInfo = read<DWORD_PTR>(entity.state + 0x0C48);
        auto localTeamInfo = read<DWORD_PTR>(pointers.localPlayerState + 0x0C48);
        if (localPlayerId == playerId || teamInfo == localTeamInfo)
            continue;
        if (distanceToPlayer < distance || distance == 10000) {
            distance = distanceToPlayer;
            closestEntity = entity;
        }
    }
    return closestEntity;
}

Entity findClosestEnemyToCrosshair() {
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    float distance = 10000;
    Entity closestEntity;
    for (const auto &entity: playerList) {
        if (!entity.skeletonMesh || !entity.state || !entity.acknowledgePawn)
            continue;
        auto playerLocation = PlayerLocation(entity);
        auto distanceToPlayer = playerLocation.headScreenPosition.distance2D(screenCenter);
        auto playerId = read<int>(entity.state + 0x022C);
        auto localPlayerId = read<int>(pointers.localPlayerState + 0x022C);
        auto teamInfo = read<DWORD_PTR>(entity.state + 0x0C48);
        auto localTeamInfo = read<DWORD_PTR>(pointers.localPlayerState + 0x0C48);
        if (localPlayerId == playerId || teamInfo == localTeamInfo)
            continue;
        if (distanceToPlayer < distance || distance == 10000) {
            distance = distanceToPlayer;
            closestEntity = entity;
        }
    }
    return closestEntity;
}

FVector visibleRadius(FVector position, float radius) {
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);
    float angle = screenCenter.angle(position);
    return screenCenter.direction(angle, radius);
}

bool isInRadius(FVector screenCenter, FVector position, float radius) {
    return screenCenter.distance2D(position) < radius;
}


FVector target_prediction(FVector TargetPosition, FVector ComponentVelocity, float player_distance, float ProjectileSpeed) {
    float gravity = abs(-336.0f); // Gravity (Probably other ways to get this but I set it as a constant value)
    float time = player_distance / abs(ProjectileSpeed);
    float bulletDrop = (gravity / 250) * time * time;
    return {TargetPosition.x += time * (ComponentVelocity.x), TargetPosition.y += time * (ComponentVelocity.y), TargetPosition.z += time * (ComponentVelocity.z)};
}


void render() {
    return;


    FVector screenSize = FVector((float) farlight.width, (float) farlight.height, 0);
    FVector screenCenter = FVector(static_cast<float>((float) farlight.width / 2.0f), static_cast<float>((float) farlight.height / 2.0f), 0);

    float clearRadius = 200.0f;
    drawCircleOutline((float) farlight.width / 2.0f, (float) farlight.height / 2.0f, clearRadius, ImColor(255, 255, 255), 1.0f);


    auto localPlayerPosition = read<FVector>(pointers.localPlayerRoot + offsets::scRelativeLocation);
    //for (const auto &entity: itemList) {
    //    auto itemPosition = read<FVector>(entity.rootComponent + offsets::scRelativeLocation);
    //    auto itemScreenPosition = projectWorldToScreen(pointers.localPlayerController, itemPosition, screenSize);
    //    auto solarItemData = read<FSolarItemData>(entity.acknowledgePawn + offsets::siaItemData);
    //    auto distance = localPlayerPosition.distance(itemPosition);
    //    drawText(solarItemData.Name.toString(), itemScreenPosition.x, itemScreenPosition.y + 16.0f, 16.0f, qualityColors[solarItemData.Quality], true);
    //}

    for (const auto &entity: playerList) {
        if (!entity.skeletonMesh || !entity.state || !entity.acknowledgePawn)
            continue;
        auto playerLocation = PlayerLocation(entity);
        auto closestEnemyLocation = PlayerLocation(findClosestEnemyToCrosshair());
        if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_DOWN))
            move_to((float) farlight.width, (float) farlight.height, randomFloat(4.0f, 15.0f), closestEnemyLocation.headScreenPosition.x, closestEnemyLocation.headScreenPosition.y);


        auto playerRoot = read<DWORD_PTR>(entity.acknowledgePawn + offsets::pRootComponent);
        auto componentVelocity = read<FVector>(playerRoot + 0x0140);
        drawText(componentVelocity.debug(), playerLocation.headScreenPosition.x, playerLocation.headScreenPosition.y, 12, ImColor(255, 255, 255), true);


        auto playerTop = playerLocation.headScreenPosition;
        auto playerBottom = playerLocation.rootScreenPosition;
        auto playerHeight = abs(playerTop.y - playerBottom.y);
        auto playerWidth = playerHeight * 0.65f;
        auto distanceToPlayer = playerLocation.distance();


        auto characterID = read<int32_t>(entity.state + offsets::spsCharacterId);
        auto skinId = read<int32_t>(entity.state + offsets::spsSkinId);
        auto playerName = read<FString>(entity.state + offsets::spsNickName);
        auto playerHealth = read<float>(entity.state + offsets::spsCurrentHealth);
        auto playerMaxHealth = read<float>(entity.state + offsets::spsMaxHealth);
        auto playerId = read<int>(entity.state + 0x022C);
        auto localPlayerId = read<int>(pointers.localPlayerState + 0x022C);
        auto previousLocation = read<FRotator>(entity.state + 0x0F20);


        if (localPlayerId == playerId)
            continue;
        if (playerHealth == 0 || playerMaxHealth == 0)
            continue;

        auto teamInfo = read<DWORD_PTR>(entity.state + 0x0C48);
        auto localTeamInfo = read<DWORD_PTR>(pointers.localPlayerState + 0x0C48);
        auto isBot = read<FString>(entity.state + offsets::spsSolarPlayerID).toString().starts_with("000-");


        FVector startPoint = visibleRadius(playerBottom, clearRadius);
        std::string playerHint = "";
        if (isInRadius(screenCenter, playerBottom, clearRadius)) {
            playerHint = (isBot ? "BOT" : playerName.toString().substr(0, playerName.toString().find('#'))) + " [" + std::to_string(static_cast<int>(distanceToPlayer)) + "m]";
        } else {
            playerHint = "[" + std::to_string(static_cast<int>(distanceToPlayer)) + "m]";
        }
        FVector textStartPoint = visibleRadius(playerBottom, clearRadius - 5);
        ImColor lineColor = ImColor(102, 178, 255);
        if (isBot)
            lineColor = ImColor(255, 51, 51);
        if (teamInfo == localTeamInfo)
            lineColor = ImColor(178, 255, 102);


        if (isInRadius(screenCenter, playerBottom, clearRadius - 25)) {
            drawText(playerHint, playerBottom.x, playerBottom.y, 14.0f, lineColor, true);
        } else {
            drawText(playerHint, textStartPoint.x, textStartPoint.y, 14.0f, lineColor, true);
        }
        if (isInRadius(screenCenter, playerBottom, clearRadius)) {
            drawText(playerHint, playerBottom.x, playerBottom.y, 14.0f, lineColor, true);
            drawCircleFilled(playerBottom.x, playerBottom.y, 2.0f, lineColor, 30);
        } else {
            drawLine(startPoint.x, startPoint.y, playerBottom.x, playerBottom.y, ImColor(0, 0, 0), 1.5f);
            drawLine(startPoint.x, startPoint.y, playerBottom.x, playerBottom.y, lineColor, 1.0f);
            drawCircleFilled(startPoint.x, startPoint.y, 2.0f, lineColor, 30);
            drawCircleFilled(playerBottom.x, playerBottom.y, 2.0f, lineColor, 30);
        }


    }
}


std::string toHex(uintptr_t value) {
    std::stringstream result;
    result << std::hex << value;
    return "0x" + result.str();
}

enum EPlayerWeaponEquipStatus : uint8_t {
    EPlayerWeaponEquipStatus__None0 = 0,
    EPlayerWeaponEquipStatus__Holding1 = 1,
    EPlayerWeaponEquipStatus__PendingToHang2 = 2,
    EPlayerWeaponEquipStatus__Hanging3 = 3,
    EPlayerWeaponEquipStatus__EPlayerWeaponEquipStatus_MAX4 = 4
};
struct FPlayerWeaponSlotInfo {
    TWeakObjectPtr Weapon;
    bool bServerHasWeapon: 1;
    bool bIsSwapping: 1;
    bool bIsDestroying: 1;
    EPlayerWeaponEquipStatus SwapTargetEquipStatus;
    unsigned char UnknownData00_6[0x2];
};


void renderMenu() {
    drawSeparator();
    tabButton("ESP", &selectedTab, 0, true);
    tabButton("Debug", &selectedTab, 1, false);
    drawSeparator();


    auto weaponSystemComponent = read<DWORD_PTR>(pointers.localPlayerPawn + offsets::pWeaponSystemComponent);
    auto playerWeaponSlotInfo = read<FPlayerWeaponSlotInfo>(weaponSystemComponent + offsets::wspPrimarySlotInfo);
    auto primaryWeaponPointer = playerWeaponSlotInfo.Weapon.getPointer();
    auto weaponConfig = read<DWORD_PTR>(primaryWeaponPointer + 0x0470);
    auto weaponTextType = read<FString>(weaponConfig + 0x0568);




    ImGui::Text(("WeaponTextType: " + weaponTextType.toString()).c_str());



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
