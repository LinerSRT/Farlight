#include "../ue/engine.h"


struct FPlayerWeaponEquipStatusInfo {
    EPlayerWeaponEquipStatus PrimaryWeaponStatus;
    EPlayerWeaponEquipStatus SecondaryWeaponStatus;
    EPlayerWeaponEquipStatus TertiaryWeaponStatus;
    char Version;
};

struct FBoardedVehicleInfo {
    DWORD_PTR BoardedVehicle;                                             // 0x0000   (0x0008)
    int32_t BoardedSeatIndex;                                           // 0x0008   (0x0004)
    unsigned char UnknownData00_6[0x4];                                       // 0x000C   (0x0004)  MISSED
};


class GameEntity {
public:
    DWORD_PTR address;
    DWORD_PTR pawn;
    DWORD_PTR controller;
    DWORD_PTR state;
    DWORD_PTR rootComponent;
    DWORD_PTR skeletonMesh;
    DWORD_PTR movement;
    DWORD_PTR capsule;
    DWORD_PTR weaponSystemComponent;
    DWORD_PTR teamInfo;
    EEntityType entityType;
public:
    GameEntity() : address(0), entityType(UNKNOWN_ENTITY) {}

    GameEntity(DWORD_PTR address, EEntityType entityType) : address(address), entityType(entityType) {
        switch (entityType) {
            case CHARACTER:
            case LOBBY_CHARACTER:
                pawn = address;
                controller = read<DWORD_PTR>(pawn + 0x0260);
                state = read<DWORD_PTR>(pawn + offsets::pPlayerState);
                rootComponent = read<DWORD_PTR>(pawn + offsets::pRootComponent);
                skeletonMesh = read<DWORD_PTR>(pawn + offsets::cMesh);
                movement = read<DWORD_PTR>(pawn + offsets::cCharacterMovement);
                capsule = read<DWORD_PTR>(pawn + offsets::cCapsuleComponent);
                weaponSystemComponent = read<DWORD_PTR>(pawn + offsets::pWeaponSystemComponent);
                teamInfo = read<DWORD_PTR>(state + offsets::spsTeam);
                break;
            case LOCAL_CHARACTER:
                controller = read<DWORD_PTR>(address + offsets::pPlayerController);
                pawn = read<DWORD_PTR>(controller + offsets::pcAcknowledgedPawn);
                state = read<DWORD_PTR>(pawn + offsets::pPlayerState);
                rootComponent = read<DWORD_PTR>(pawn + offsets::pRootComponent);
                skeletonMesh = read<DWORD_PTR>(pawn + offsets::cMesh);
                movement = read<DWORD_PTR>(pawn + offsets::cCharacterMovement);
                capsule = read<DWORD_PTR>(pawn + offsets::cCapsuleComponent);
                weaponSystemComponent = read<DWORD_PTR>(pawn + offsets::pWeaponSystemComponent);
                teamInfo = read<DWORD_PTR>(state + offsets::spsTeam);
                break;
            case VEHICLE:
                pawn = address;
                skeletonMesh = read<DWORD_PTR>(pawn + offsets::vVehicleMesh);
                rootComponent = read<DWORD_PTR>(pawn + offsets::vRootComponent);
                break;
            case ITEM:
            case ITEM_BOX:
            case DEATH_BOX:
                rootComponent = read<DWORD_PTR>(address + offsets::pRootComponent);
                break;
            case OBJECT:
            case UNKNOWN_ENTITY:
                break;
        }
    }

    [[nodiscard]] bool isValid() const {
        return entityType != UNKNOWN_ENTITY && address != 0;
    }
};


struct Character {
    static int32_t getId(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return read<int32_t>(entity.state + offsets::spsId);
            default:
                return -1;
        }
    }

    static bool isLocalPlayer(GameEntity entity, GameEntity check) {
        return Character::getId(entity) == Character::getId(check);
    }

    static int32_t getSkinId(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return read<int32_t>(entity.state + offsets::spsSkinId);
            default:
                return -1;
        }
    }

    static void showCursor(GameEntity entity, bool show){
        write(entity.controller + 0x0458, show);
    }

    static float getHealth(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return read<float>(entity.state + offsets::spsCurrentHealth);
            default:
                return 0;
        }
    }

    static float getMaxHealth(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return read<float>(entity.state + offsets::spsMaxHealth);
            default:
                return 0;
        }
    }

    static bool isDead(GameEntity entity) {
        return getHealth(entity) == 0 || getMaxHealth(entity) == 0;
    }

    static string getNickname(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return read<FString>(entity.state + offsets::spsNickName).toString();
            default:
                return "Null";
        }
    }

    static int getDriveState(GameEntity entity) {
        auto vehicleInfo = read<DWORD_PTR>(entity.address + 0x1DE0);
        auto boardedSeatIndex = read<int>(vehicleInfo + 0x8);
        if (boardedSeatIndex) {
            if (boardedSeatIndex == -1) {
                return 0;
            } else {
                return 2;
            }
        } else {
            return 1;
        }
    }

    static bool isInVehicle(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return read<DWORD_PTR>(entity.address + 0x1DE0) != 0;
            default:
                return false;
        }
    }

    static bool isTeamWith(GameEntity entity, GameEntity other) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return entity.teamInfo == other.teamInfo;
            default:
                return false;
        }
    }

    static bool isBot(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                return read<FString>(entity.state + offsets::spsSolarPlayerID).toString().starts_with("000-");
            default:
                return false;
        }
    }

    static bool hasUpdatedSkeleton(GameEntity entity) {
        switch (entity.entityType) {
            case CHARACTER:
            case LOCAL_CHARACTER:
                switch (getSkinId(entity)) {
                    case 132700:
                    case 132106:
                    case 131005:
                    case 131208:
                    case 130210:
                    case 132701:
                    case 131800:
                    case 131802:
                    case 130607:
                    case 130609:
                    case 131505:
                    case 132100:
                    case 132702:
                    case 120012:
                    case 131306:
                    case 120017:
                        return true;
                    default:
                        return false;
                }
            default:
                return false;
        }

    }

    static FCameraCacheEntry getCameraCache(GameEntity entity) {
        return read<FCameraCacheEntry>(read<DWORD_PTR>(entity.controller + offsets::pcPlayerCameraManager) + offsets::pcmCameraCachePrivate);
    }

    static FTransform getBoneIndex(GameEntity entity, int boneId) {
        auto bones = read<DWORD_PTR>(entity.skeletonMesh + offsets::smcSkeletalMesh);
        if (!bones)
            bones = read<DWORD_PTR>((entity.skeletonMesh + offsets::smcSkeletalMesh) + 0x10);
        return read<FTransform>(bones + (boneId * 0x30));
    }

    static FVector getBonePosition(GameEntity entity, int boneId) {
        D3DMATRIX matrix = matrixMultiplication(getBoneIndex(entity, boneId).toMatrix(), read<FTransform>(entity.skeletonMesh + 0x250).toMatrix());
        return {matrix._41, matrix._42, matrix._43};
    }

    static FVector getWorldHeadPosition(GameEntity entity, float offset) {
        auto position = getBonePosition(entity, hasUpdatedSkeleton(entity) ? 51 : 46);
        position.z += offset;
        return position;
    }

    static FVector getWorldHeadPosition(GameEntity entity) {
        return getWorldHeadPosition(entity, 0);
    }

    static FVector getWorldPosition(GameEntity entity) {
        return getBonePosition(entity, 0);
    }

    static FVector getCameraPosition(GameEntity entity) {
        return getCameraCache(entity).minimalViewInfo.location;
    }

    static bool isWeaponSlotUsed(GameEntity entity, int slot) {
        auto weaponEquipStatusInfo = read<FPlayerWeaponEquipStatusInfo>(entity.weaponSystemComponent + offsets::playerWeaponEquipStatusInfo);
        switch (slot) {
            case 0:
                return weaponEquipStatusInfo.PrimaryWeaponStatus == EPlayerWeaponEquipStatus::WeaponEquipStatus_Holding;
            case 1:
                return weaponEquipStatusInfo.SecondaryWeaponStatus == EPlayerWeaponEquipStatus::WeaponEquipStatus_Holding;
            case 2:
                return weaponEquipStatusInfo.TertiaryWeaponStatus == EPlayerWeaponEquipStatus::WeaponEquipStatus_Holding;
            default:
                return false;
        }
    }

    static bool isWeaponPresent(GameEntity entity, int slot) {
        auto weaponEquipStatusInfo = read<FPlayerWeaponEquipStatusInfo>(entity.weaponSystemComponent + offsets::playerWeaponEquipStatusInfo);
        switch (slot) {
            case 0:
                return weaponEquipStatusInfo.PrimaryWeaponStatus != EPlayerWeaponEquipStatus::WeaponEquipStatus_None;
            case 1:
                return weaponEquipStatusInfo.SecondaryWeaponStatus != EPlayerWeaponEquipStatus::WeaponEquipStatus_None;
            case 2:
                return weaponEquipStatusInfo.TertiaryWeaponStatus != EPlayerWeaponEquipStatus::WeaponEquipStatus_None;
            default:
                return false;
        }
    }


    static bool hasAnyWeapon(GameEntity entity) {
        return isWeaponPresent(entity, 0) || isWeaponPresent(entity, 1) || isWeaponPresent(entity, 2);
    }

    static bool holdingWeapon(GameEntity entity) {
        return isWeaponSlotUsed(entity, 0) || isWeaponSlotUsed(entity, 1) || isWeaponSlotUsed(entity, 2);
    }

    static DWORD_PTR getActiveWeapon(GameEntity entity) {
        if (isWeaponSlotUsed(entity, 0))
            return WeakPtr::get(read<DWORD_PTR>(entity.weaponSystemComponent + offsets::wspPrimarySlotInfo));
        if (isWeaponSlotUsed(entity, 1))
            return WeakPtr::get(read<DWORD_PTR>(entity.weaponSystemComponent + offsets::wspSecondarySlotInfo));
        if (isWeaponSlotUsed(entity, 2))
            return WeakPtr::get(read<DWORD_PTR>(entity.weaponSystemComponent + offsets::wspTertiarySlotInfo));
        return 0;
    }

    static DWORD_PTR getWeaponConfig(GameEntity entity) {
        if (holdingWeapon(entity))
            return read<DWORD_PTR>(getActiveWeapon(entity) + offsets::spwWeaponConfig);
        return 0;
    }

    static DWORD_PTR getAmmoConfig(GameEntity entity) {
        if (holdingWeapon(entity))
            return read<DWORD_PTR>(getWeaponConfig(entity) + 0x0120);
        return 0;
    }

    static EWeaponMechanicalUniqueState getActiveWeaponState(GameEntity entity) {
        if (holdingWeapon(entity))
            return read<FWeaponMechanicalState>(getActiveWeapon(entity) + 0x0308).uniqueState;
        return EWeaponMechanicalUniqueState::Idle;
    }

    static bool isWeaponDetectEnemy(GameEntity entity) {
        return holdingWeapon(entity) && read<bool>(getActiveWeapon(entity) + offsets::spwDetectedEnemy);
    }

    static int32_t getWeaponId(GameEntity entity) {
        if (hasAnyWeapon(entity))
            return read<int32_t>(getActiveWeapon(entity) + 0x0480);
        return 0;
    }

    static int32_t getRemainAmmo(GameEntity entity) {
        if (hasAnyWeapon(entity))
            return read<int32_t>(getActiveWeapon(entity) + 0x04F0);
        return 0;
    }

    static float getWeaponBulletSpeed(GameEntity entity) {
        if (hasAnyWeapon(entity))
            return read<float>(getAmmoConfig(entity) + 0x0100);
        return FLT_MAX;
    }

    static FVector getVelocity(GameEntity entity) {
        return read<FVector>(entity.movement + 0x025C);
    }

    static FVector getAcceleration(GameEntity entity) {
        return read<FVector>(entity.movement + 0x022C);
    }

    static FVector projectToScreen(GameEntity entity, FVector worldLocation, FVector screenSize) {
        FVector fVector = FVector(0, 0, 0);
        auto cameraCache = getCameraCache(entity);
        auto m = matrix(cameraCache.minimalViewInfo.rotation, FVector(0, 0, 0));
        FVector vAxisX = FVector(m.m[0][0], m.m[0][1], m.m[0][2]);
        FVector vAxisY = FVector(m.m[1][0], m.m[1][1], m.m[1][2]);
        FVector vAxisZ = FVector(m.m[2][0], m.m[2][1], m.m[2][2]);
        FVector delta = worldLocation - cameraCache.minimalViewInfo.location;
        FVector vTransformed = FVector(delta.dot(vAxisY), delta.dot(vAxisZ), delta.dot(vAxisX));
        if (vTransformed.z < 1.f)
            vTransformed.z = 1.f;
        float fovAngle = cameraCache.minimalViewInfo.FOV;
        float screenCenterX = screenSize.x / 2.0f;
        float screenCenterY = screenSize.y / 2.0f;
        fVector.x = screenCenterX + vTransformed.x * (screenCenterX / tanf(fovAngle * (float) M_PI / 360.f)) / vTransformed.z;
        fVector.y = screenCenterY - vTransformed.y * (screenCenterX / tanf(fovAngle * (float) M_PI / 360.f)) / vTransformed.z;
        return fVector;
    }


    static FVector predictAimPosition(GameEntity entity, GameEntity other, float worldGravity, float worldToMeters) {
        FVector predictedPosition = getWorldHeadPosition(other, 5);
        FVector targetVelocity = getVelocity(other);
        float gravity = fabs(worldGravity / worldToMeters);
        float bulletSpeed = getWeaponBulletSpeed(entity) / worldToMeters;
        float time = (getWorldPosition(entity).distance(predictedPosition) / worldToMeters) / fabs(bulletSpeed);
        float bulletDrop = (gravity) * time * time;
        predictedPosition.z += bulletDrop * 120;
        predictedPosition.x += time * (targetVelocity.x);
        predictedPosition.y += time * (targetVelocity.y);
        predictedPosition.z += time * (targetVelocity.z);
        return predictedPosition;
    }
};

struct ItemBox {
    static FVector getPosition(GameEntity entity) {
        return read<FVector>(entity.rootComponent + offsets::scRelativeLocation);
    }

    static float getDistance(GameEntity localPlayer, GameEntity item, float worldToMeters) {
        return Character::getWorldPosition(localPlayer).distance(getPosition(item)) / worldToMeters;
    }

    static ETreasureBoxState getState(GameEntity entity) {
        return read<ETreasureBoxState>(entity.address + 0x0418);
    }

    static TArray<FSolarItemData> getItems(GameEntity entity) {
        auto itemDataArray = read<DWORD_PTR>(entity.address + 0x0430);
        return read<TArray<FSolarItemData>>(itemDataArray + 0x0108);
    }

    static TArray<FSolarItemData> getSortedItems(GameEntity entity) {
        return read<TArray<FSolarItemData>>(entity.address + 0x0548);
    }

    static std::vector<FSolarItemData> getBurstItems(GameEntity entity) {
        std::vector<FSolarItemData> itemData;
        auto array = read<TArray<DWORD_PTR>>(entity.address + 0x05E8);
        for (int i = 0; i < array.length(); ++i) {
            itemData.emplace_back(read<FSolarItemData>(array.getValuePointer(i) + 0x0270));
        }
        return itemData;
    }

    static int getHighestItemQuality(GameEntity entity) {
        return read<int>(entity.address + 0x05C8);
    }
};

struct DeathBox : ItemBox {
    static float getEnergyValue(GameEntity entity) {
        return read<float>(entity.address + 0x060C);
    }

    static float getEnergyExtraValue(GameEntity entity) {
        return read<float>(entity.address + 0x0610);
    }

    static FString getPlayerId(GameEntity entity) {
        return read<FString>(entity.address + 0x0618);
    }

    static FString getPlayerNickname(GameEntity entity) {
        return read<FString>(entity.address + 0x0628);
    }

    static EBResurrectionState getResurrectionState(GameEntity entity) {
        return read<EBResurrectionState>(entity.address + 0x0678);
    }
};

struct Item {
    static FVector getPosition(GameEntity entity) {
        return read<FVector>(entity.rootComponent + offsets::scRelativeLocation);
    }

    static float getDistance(GameEntity localPlayer, GameEntity item, float worldToMeters) {
        return Character::getWorldPosition(localPlayer).distance(getPosition(item)) / worldToMeters;
    }

    static FSolarItemData getItemData(GameEntity entity) {
        return read<FSolarItemData>(entity.address + offsets::siaItemData);
    }

    static ImColor getItemColor(int qualility) {
        ImColor qualityColors[7] = {
                ImColor(255, 255, 255),
                ImColor(255, 255, 255),
                ImColor(28, 184, 76),
                ImColor(48, 187, 216),
                ImColor(214, 109, 219),
                ImColor(230, 112, 55),
                ImColor(217, 37, 18)
        };
        return qualityColors[qualility];
    }

    static ImColor getItemColor(FSolarItemData data) {
        return getItemColor(data.Quality);
    }

    static ImColor getItemColor(GameEntity entity) {
        return getItemColor(getItemData(entity));
    }

};

struct Vehicle {
    static FVector getPosition(GameEntity entity) {
        return read<FVector>(entity.rootComponent + offsets::scRelativeLocation);
    }

    static float getDistance(GameEntity localPlayer, GameEntity item, float worldToMeters) {
        return Character::getWorldPosition(localPlayer).distance(getPosition(item)) / worldToMeters;
    }
};

std::string weaponIdToString(int weaponId) {
    switch (weaponId) {
        case EWeaponId_Invader:
            return "Invader";
        case EWeaponId_Hound:
            return "Hound";
        case EWeaponId_WhiteDwarf:
            return "White dwarf";
        case EWeaponId_Bar95:
            return "Bar-95";
        case EWeaponId_Defender:
            return "Defender";
        case EWeaponId_Dikobraz:
            return "Porcupine";
        case EWeaponId_Generator:
            return "Generator";
        case EWeaponId_MF18:
            return "MF18";
        case EWeaponId_M4:
            return "M4";
        case EWeaponId_UMP99:
            return "UMP99";
        case EWeaponId_StellarWind:
            return "Stellar wind";
        case EWeaponId_Fanatic:
            return "Fanatic";
        case EWeaponId_AK:
            return "AK";
        case EWeaponId_MadRabbit:
            return "Mad rabiit";
        case EWeaponId_Jupiter:
            return "Jupiter";
        case EWeaponId_MadRat:
            return "Mad rat";
        case EWeaponId_VSS:
            return "VSS";
        case EWeaponId_Rhino:
            return "Rhino";
        case EWeaponId_Vega:
            return "Vega";
        case EWeaponId_MG7:
            return "MG-7";
        case EWeaponId_UZI:
            return "UZI";
        default:
            return "No weapon";
    }
}


class WorldSettings {
private:
    DWORD_PTR address;
public:
    int32_t visibilityRange;
    float worldToMeters;
    float worldGravityZ;
    float globalGravityZ;
    float globalViewDistance;

    explicit WorldSettings(DWORD_PTR address) : address(address) {
        visibilityRange = read<int32_t>(address + 0x0230);
        worldToMeters = read<float>(address + 0x02A0);
        worldGravityZ = read<float>(address + 0x02B0);
        globalGravityZ = read<float>(address + 0x02B4);
        globalViewDistance = read<float>(address + 0x030C);
    }

    WorldSettings() {
        address = 0;
    }

    [[nodiscard]] bool isValid() const {
        return address != 0;
    }
};


namespace EntityType {
    bool isVehicle(const std::string &type) {
        return strings::isContain(type, "Vehicle");
    }

    bool isWorldSettings(const std::string &type) {
        return strings::isEqual(type, "SolarWorldSettings");
    }

    bool isPlayer(const std::string &type) {
        return strings::isContain(type, "SolarLobbyCharacter") ||
               strings::isContain(type, "BP_Character");
    }

    bool isDeadBox(std::string &type) {
        return strings::isEqual(type, "BP_DBox_Default_C");
    }

    bool isTreasureBox(std::string &type) {
        return strings::isEqual(type, "BP_SI_ElectricboxC_C") ||
               strings::isEqual(type, "BP_SI_Tacticalbox_C");
    }

    bool isDropBox(std::string &type) {
        return strings::isEqual(type, "BP_SI_AirdropBox_TreasureBox_Hoarder_C") ||
               strings::isEqual(type, "BP_SI_AirdropBox_Signal_C");
    }

    bool isPickup(std::string &type) {
        return strings::isEqual(type, "SolarItemActor") ||
               strings::isEqual(type, "SolarGroundPreviewActor");
    }

    bool isElectricBox(std::string &type) {
        return strings::isEqual(type, "BP_SI_ElectricboxB_C") ||
               strings::isEqual(type, "BP_SI_ElectricboxC_C");
    }
}


namespace BonesType {
    bool newSkin(int id) {
        return (id == 132700 || id == 132106 || id == 131005 || id == 131208 || id == 130210 || id == 132701 || id == 131800 || id == 131802 || id == 130607 || id == 131505 || id == 132100 || id == 132702 || id == 120017);
    }

    struct Bones {
        int Root;
        int Bip001;
        int pelvis;
        int spine_01;
        int spine_02;
        int spine_03;
        int clavicle_l;
        int upperarm_l;
        int lowerarm_l;
        int hand_l;
        int thumb_01_l;
        int thumb_02_l;
        int thumb_03_l;
        int index_01_l;
        int index_02_l;
        int index_03_l;
        int middle_01_l;
        int middle_02_l;
        int middle_03_l;
        int ring_01_l;
        int ring_02_l;
        int ring_03_l;
        int pinky_01_l;
        int pinky_02_l;
        int pinky_03_l;
        int clavicle_r;
        int upperarm_r;
        int lowerarm_r;
        int hand_r;
        int thumb_01_r;
        int thumb_02_r;
        int thumb_03_r;
        int index_01_r;
        int index_02_r;
        int index_03_r;
        int middle_01_r;
        int middle_02_r;
        int middle_03_r;
        int ring_01_r;
        int ring_02_r;
        int ring_03_r;
        int pinky_01_r;
        int pinky_02_r;
        int pinky_03_r;
        int RightHandWeaponAttach;
        int neck_01;
        int head;
        int BackpackAttach;
        int thigh_l;
        int calf_l;
        int foot_l;
        int ball_l;
        int thigh_r;
        int calf_r;
        int foot_r;
        int ball_r;
        int VB_spine_03_RightHandWeaponAttach;
        int VB_VB_spine_03_RightHandWeaponAttach_hand_r;
        int VB_VB_VB_spine_03_RightHandWeaponAttach_hand_r_lowerarm_r;

        explicit Bones(bool IsNewSkinID) {
            if (IsNewSkinID) {
                initNewBones();
            } else {
                initDefaultBones();
            }
        }

        void initDefaultBones() {
            Root = 0;
            Bip001 = 1;
            pelvis = 2;
            spine_01 = 3;
            spine_02 = 4;
            spine_03 = 5;
            clavicle_l = 6;
            upperarm_l = 7;
            lowerarm_l = 8;
            hand_l = 9;
            thumb_01_l = 10;
            thumb_02_l = 11;
            thumb_03_l = 12;
            index_01_l = 13;
            index_02_l = 14;
            index_03_l = 15;
            middle_01_l = 16;
            middle_02_l = 17;
            middle_03_l = 18;
            ring_01_l = 19;
            ring_02_l = 20;
            ring_03_l = 21;
            pinky_01_l = 22;
            pinky_02_l = 23;
            pinky_03_l = 24;
            clavicle_r = 25;
            upperarm_r = 26;
            lowerarm_r = 27;
            hand_r = 28;
            thumb_01_r = 29;
            thumb_02_r = 30;
            thumb_03_r = 31;
            index_01_r = 32;
            index_02_r = 33;
            index_03_r = 34;
            middle_01_r = 35;
            middle_02_r = 36;
            middle_03_r = 37;
            ring_01_r = 38;
            ring_02_r = 39;
            ring_03_r = 40;
            pinky_01_r = 41;
            pinky_02_r = 42;
            pinky_03_r = 43;
            RightHandWeaponAttach = 44;
            neck_01 = 45;
            head = 46;
            BackpackAttach = 47;
            thigh_l = 48;
            calf_l = 49;
            foot_l = 50;
            ball_l = 51;
            thigh_r = 52;
            calf_r = 53;
            foot_r = 54;
            ball_r = 55;
            VB_spine_03_RightHandWeaponAttach = 56;
            VB_VB_spine_03_RightHandWeaponAttach_hand_r = 57;
            VB_VB_VB_spine_03_RightHandWeaponAttach_hand_r_lowerarm_r = 58;
        }

        void initNewBones() {
            Root = 0;
            Bip001 = 1;
            pelvis = 2;
            spine_01 = 3;
            spine_02 = 4;
            spine_03 = 5;
            clavicle_l = 6;
            upperarm_l = 7;
            lowerarm_l = 8;
            hand_l = 9;
            thumb_01_l = 10;
            thumb_02_l = 11;
            thumb_03_l = 12;
            index_01_l = 13;
            index_02_l = 14;
            index_03_l = 15;
            middle_01_l = 16;
            middle_02_l = 17;
            middle_03_l = 18;
            ring_01_l = 19;
            ring_02_l = 20;
            ring_03_l = 21;
            pinky_01_l = 22;
            pinky_02_l = 23;
            pinky_03_l = 24;
            upperarm_r = 29;
            lowerarm_r = 30;
            hand_r = 31;
            thumb_01_r = 32;
            thumb_02_r = 33;
            thumb_03_r = 34;
            index_01_r = 35;
            index_02_r = 36;
            index_03_r = 37;
            middle_01_r = 38;
            middle_02_r = 39;
            middle_03_r = 40;
            ring_01_r = 41;
            ring_02_r = 42;
            ring_03_r = 43;
            pinky_01_r = 44;
            pinky_02_r = 45;
            pinky_03_r = 46;
            RightHandWeaponAttach = 47;
            neck_01 = 50;
            head = 51;
            BackpackAttach = 52;
            thigh_l = 53;
            calf_l = 54;
            foot_l = 55;
            ball_l = 56;
            thigh_r = 59;
            calf_r = 60;
            foot_r = 61;
            ball_r = 62;
            VB_spine_03_RightHandWeaponAttach = 65;
            VB_VB_spine_03_RightHandWeaponAttach_hand_r = 66;
            VB_VB_VB_spine_03_RightHandWeaponAttach_hand_r_lowerarm_r = 67;
        }
    };
}


FTransform getBoneIndex(DWORD_PTR mesh, int index) {
    auto bones = read<DWORD_PTR>(mesh + offsets::smcSkeletalMesh);
    if (!bones)
        bones = read<DWORD_PTR>((mesh + offsets::smcSkeletalMesh) + 0x10);
    return read<FTransform>(bones + (index * 0x30));
}

FVector getBoneWithRotation(DWORD_PTR mesh, int id) {
    FTransform bone = getBoneIndex(mesh, id);
    auto world = read<FTransform>(mesh + 0x250);
    D3DMATRIX matrix = matrixMultiplication(bone.toMatrix(), world.toMatrix());
    return {matrix._41, matrix._42, matrix._43};
}


