#include "../ue/engine.h"

enum EPlayerWeaponEquipStatus : uint8_t {
    None = 0,
    Holding = 1,
    PendingToHang = 2,
    Hanging = 3
};
struct FPlayerWeaponEquipStatusInfo {
    EPlayerWeaponEquipStatus PrimaryWeaponStatus;
    EPlayerWeaponEquipStatus SecondaryWeaponStatus;
    EPlayerWeaponEquipStatus TertiaryWeaponStatus;
    char Version;
};






struct FWeaponDebugFlag {
    bool bIgnoreActivated;                                           // 0x0000   (0x0001)
    bool bExtraInfo;                                                 // 0x0001   (0x0001)
    bool bMechanicalState;                                           // 0x0002   (0x0001)
    bool bShowShootDir;                                              // 0x0003   (0x0001)
};

enum EWeaponId : int32_t {
    EWeaponId_Invader = 11001,
    EWeaponId_Hound = 12001,
    EWeaponId_WhiteDwarf = 13001,
    EWeaponId_Bar95 = 14001,
    EWeaponId_Defender = 15001,
    EWeaponId_Dikobraz = 16001,
    EWeaponId_Generator = 17001,
    EWeaponId_MF18 = 18001,
    EWeaponId_M4 = 19001,
    EWeaponId_UMP99 = 20001,
    EWeaponId_StellarWind = 21001,
    EWeaponId_Fanatic = 22001,
    EWeaponId_AK = 23001,
    EWeaponId_MadRabbit = 24001,
    EWeaponId_Jupiter = 25001,
    EWeaponId_MadRat = 27001,
    EWeaponId_VSS = 28001,
    EWeaponId_Rhino = 29001,
    EWeaponId_Vega = 30001,
    EWeaponId_MG7 = 31001,
    EWeaponId_UZI = 32001
};

std::string weaponIdToString(EWeaponId weaponId) {
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
            return "Unknown";
    }
}




struct FBasedMovementInfo {
    unsigned char MovementBase[0x8];                                               // 0x0000   (0x0008)
    FName BoneName;                                                   // 0x0008   (0x0008)
    FVector Location;                                                   // 0x0010   (0x000C)
    FRotator Rotation;                                                   // 0x001C   (0x000C)
    bool bServerHasBaseComponent;                                    // 0x0028   (0x0001)
    bool bRelativeRotation;                                          // 0x0029   (0x0001)
    bool bServerHasVelocity;                                         // 0x002A   (0x0001)
    unsigned char UnknownData00_6[0x5];                                       // 0x002B   (0x0005)  MISSED
};

struct WorldSettings {
    float worldToMeters;
    float worldGravityZ;
    float globalGravityZ;
};

class UAmmoConfig2 {
private:
    DWORD_PTR address;
public:
    FString name;
    FString description;
    float chargingToleranceEndTime;
    float chargingWorkingEndTime;
    float chargingHoldingEndTime;
    float chargingTrajectoryTime;
    int32_t fireCostFullShot;
    float maxChargingDamageScale;
    float deltaAngle;
    float lifeTime;
    ETrajectoryType trajectoryType;
    float customValue;
    float maxRange;
    float effRange;
    float radius;
    float initSpeed;
    float projectileMaxGravity;
    float particleStartDistance;
    float trajectoryStartDistance;

    explicit UAmmoConfig2(DWORD_PTR address) : address(address) {
        name = read<FString>(address + 0x0080);
        description = read<FString>(address + 0x0090);
        chargingToleranceEndTime = read<float>(address + 0x0058);
        chargingWorkingEndTime = read<float>(address + 0x005C);
        chargingHoldingEndTime = read<float>(address + 0x0060);
        chargingTrajectoryTime = read<float>(address + 0x0064);
        fireCostFullShot = read<int32_t>(address + 0x006C);
        maxChargingDamageScale = read<float>(address + 0x0074);
        deltaAngle = read<float>(address + 0x0078);
        lifeTime = read<float>(address + 0x00A4);
        trajectoryType = read<ETrajectoryType>(address + 0x00A9);
        customValue = read<float>(address + 0x00AC);
        maxRange = read<float>(address + 0x00F4);
        effRange = read<float>(address + 0x00F8);
        radius = read<float>(address + 0x00FC);
        initSpeed = read<float>(address + 0x0100);
        projectileMaxGravity = read<float>(address + 0x0260);
        particleStartDistance = read<float>(address + 0x0264);
        trajectoryStartDistance = read<float>(address + 0x0268);
    }
};

class USingleWeaponConfig2 {
private:
    DWORD_PTR address;
public:
    ESolarWeaponBrand weaponBrand;
    EWeaponType weaponType;
    bool isSingleWeapon;
    bool canSuspendBolt;
    bool isHeavyFireWeapon;
    bool isFlamethrowerWeapon;
    bool supportSingleShootMode;
    bool supportAutoShootMode;
    bool supportBurstShootMode;
    float maxSpread;
    float minSpread;
    bool needOverload;
    float scopeOpenTime;
    float scopeOpenFOVTimeScale;
    FString weaponTextType;

    explicit USingleWeaponConfig2(DWORD_PTR address) : address(address) {
        weaponBrand = read<ESolarWeaponBrand>(address + 0x0034);
        weaponType = read<EWeaponType>(address + 0x0035);
        isSingleWeapon = read<bool>(address + 0x0048);
        isHeavyFireWeapon = read<bool>(address + 0x004A);
        isFlamethrowerWeapon = read<bool>(address + 0x004B);
        supportSingleShootMode = read<bool>(address + 0x004C);
        supportAutoShootMode = read<bool>(address + 0x004D);
        supportBurstShootMode = read<bool>(address + 0x004E);
        maxSpread = read<float>(address + 0x0050);
        minSpread = read<float>(address + 0x0054);
        needOverload = read<bool>(address + 0x0148);
        scopeOpenTime = read<float>(address + 0x02D4);
        scopeOpenFOVTimeScale = read<float>(address + 0x02D8);
        weaponTextType = read<FString>(address + 0x0568);
    }

    [[nodiscard]]  UAmmoConfig getPrimaryAmmoConfig() const {
        return UAmmoConfig(read<DWORD_PTR>(address + 0x0120));
    }
};

class SolarCharacter {
private:
    DWORD_PTR address;
    //DWORD_PTR controller;
    //DWORD_PTR pawn;
    DWORD_PTR state;
    DWORD_PTR skeletonMesh;
    DWORD_PTR rootComponent;
    DWORD_PTR weaponSystemComponent;
    DWORD_PTR solarTeamInfo;
    int32_t id;
    int32_t skinId;
    int32_t characterID;
    FString name;
    float health;
    float maxHealth;
    float eyeHeight;
    float eyeHeightCrouched;
    bool isPlayerBot;
    UCharacterMovementComponent movement;
    APlayerController controller;
    APawn pawn;

    void load() {
        id = read<int32_t>(state + 0x022C);
        skinId = read<int32_t>(state + offsets::spsSkinId);
        characterID = read<int32_t>(state + offsets::spsCharacterId);
        name = read<FString>(state + offsets::spsNickName);
        health = read<float>(state + offsets::spsCurrentHealth);
        maxHealth = read<float>(state + offsets::spsMaxHealth);
        isPlayerBot = read<FString>(state + offsets::spsSolarPlayerID).toString().starts_with("000-");
        eyeHeight = read<float>(pawn + 0x0234);
        eyeHeightCrouched = read<float>(address + 0x032C);
        solarTeamInfo = read<DWORD_PTR>(state + 0x0C48);
        movement = UCharacterMovementComponent(read<DWORD_PTR>(pawn + 0x290));
    }

    [[nodiscard]] FTransform getBoneIndex(int index) const {
        auto bones = read<DWORD_PTR>(skeletonMesh + offsets::smcSkeletalMesh);
        if (!bones)
            bones = read<DWORD_PTR>((skeletonMesh + offsets::smcSkeletalMesh) + 0x10);
        return read<FTransform>(bones + (index * 0x30));
    }

    [[nodiscard]] bool boneRequireCorrection() const {
        return skinId == 132700 ||
               skinId == 132106 ||
               skinId == 131005 ||
               skinId == 131208 ||
               skinId == 130210 ||
               skinId == 132701 ||
               skinId == 131800 ||
               skinId == 131802 ||
               skinId == 130607 ||
               skinId == 130609 ||
               skinId == 131505 ||
               skinId == 132100 ||
               skinId == 132702 ||
               skinId == 120012 ||
               skinId == 131306 ||
               skinId == 120017;
    }


public:
    SolarCharacter() {
        address = 0;
    };

    explicit SolarCharacter(DWORD_PTR address) : address(address) {
        controller = APlayerController(read<DWORD_PTR>(address + offsets::pPlayerController));
        //pawn = read<DWORD_PTR>(controller + offsets::pcAcknowledgedPawn);
        pawn = controller.acknowledgedPawn;
        //state = read<DWORD_PTR>(pawn + offsets::pPlayerState);
        skeletonMesh = read<DWORD_PTR>(address + offsets::cMesh);
        rootComponent = read<DWORD_PTR>(address + offsets::pRootComponent);
        weaponSystemComponent = read<DWORD_PTR>(pawn + offsets::pWeaponSystemComponent);
        load();
    }

    explicit SolarCharacter(DWORD_PTR address, bool pawnController) : address(address) {
        if (pawnController) {
            controller = read<DWORD_PTR>(address + 0x0260);
            pawn = address;
            state = read<DWORD_PTR>(address + offsets::pPlayerState);
            skeletonMesh = read<DWORD_PTR>(address + offsets::cMesh);
            rootComponent = read<DWORD_PTR>(address + offsets::pRootComponent);
            weaponSystemComponent = read<DWORD_PTR>(pawn + offsets::pWeaponSystemComponent);
        } else {
            controller = read<DWORD_PTR>(address + offsets::pPlayerController);
            pawn = read<DWORD_PTR>(controller + offsets::pcAcknowledgedPawn);
            state = read<DWORD_PTR>(address + offsets::pPlayerState);
            skeletonMesh = read<DWORD_PTR>(address + offsets::cMesh);
            rootComponent = read<DWORD_PTR>(address + offsets::pRootComponent);
            weaponSystemComponent = read<DWORD_PTR>(pawn + offsets::pWeaponSystemComponent);
        }
        load();
    }

    [[nodiscard]]  FVector getBonePosition(int boneId) const {
        D3DMATRIX matrix = matrixMultiplication(getBoneIndex(boneId).toMatrix(), read<FTransform>(skeletonMesh + 0x250).toMatrix());
        return {matrix._41, matrix._42, matrix._43};
    }

    [[nodiscard]] FCameraCacheEntry getCameraCache() const {
        return read<FCameraCacheEntry>(read<uintptr_t>(controller + offsets::pcPlayerCameraManager) + offsets::pcmCameraCachePrivate);
    }

    [[nodiscard]] bool isMyTeammate(SolarCharacter character) const {
        return solarTeamInfo == character.getSolarTeamInfo();
    }

    [[nodiscard]] bool isWeaponSlotUsed(int slot) const {
        auto weaponEquipStatusInfo = read<FPlayerWeaponEquipStatusInfo>(weaponSystemComponent + offsets::playerWeaponEquipStatusInfo);
        switch (slot) {
            case 0:
                return weaponEquipStatusInfo.PrimaryWeaponStatus == EPlayerWeaponEquipStatus::Holding;
            case 1:
                return weaponEquipStatusInfo.SecondaryWeaponStatus == EPlayerWeaponEquipStatus::Holding;
            case 2:
                return weaponEquipStatusInfo.TertiaryWeaponStatus == EPlayerWeaponEquipStatus::Holding;
            default:
                return false;
        }
    }

    [[nodiscard]] DWORD_PTR getActiveWeapon() const {
        if (isWeaponSlotUsed(0))
            return WeakPtr::get(read<DWORD_PTR>(weaponSystemComponent + offsets::wspPrimarySlotInfo));
        if (isWeaponSlotUsed(1))
            return WeakPtr::get(read<DWORD_PTR>(weaponSystemComponent + offsets::wspSecondarySlotInfo));
        if (isWeaponSlotUsed(2))
            return WeakPtr::get(read<DWORD_PTR>(weaponSystemComponent + offsets::wspTertiarySlotInfo));
        return 0x0;
    }

    [[nodiscard]] EWeaponMechanicalUniqueState getActiveWeaponState() const {
        auto weaponMechanicalState = read<FWeaponMechanicalState>(getActiveWeapon() + 0x0308);
        return weaponMechanicalState.uniqueState;
    }

    [[nodiscard]] int getActiveWeaponSlot() const {
        if (isWeaponSlotUsed(0))
            return 0;
        if (isWeaponSlotUsed(1))
            return 1;
        if (isWeaponSlotUsed(2))
            return 3;
        return -1;
    }

    [[nodiscard]] FPlayerWeaponShootStatus getShootStatus() const {
        return read<FPlayerWeaponShootStatus>(getActiveWeapon() + offsets::spwShootStatus);
    }

    [[nodiscard]] bool isWeaponDetectEnemy() const {
        return read<bool>(getActiveWeapon() + offsets::spwDetectedEnemy);
    }

    [[nodiscard]] int32_t getWeaponId() const {
        return read<int32_t>(getActiveWeapon() + 0x0480);
    }

    [[nodiscard]]  USingleWeaponConfig getWeaponConfig() const {
        return USingleWeaponConfig(read<DWORD_PTR>(getActiveWeapon() + offsets::spwWeaponConfig));
    }

    [[nodiscard]]  FVector getVelocity() const {
        return movement.lastUpdateVelocity;
        //return read<FVector>(rootComponent + 0x0140);
    }

    [[nodiscard]]FVector getWorldPosition() const {
        return getBonePosition(0);
    }

    [[nodiscard]]FVector getCameraPosition() const {
        return getCameraCache().minimalViewInfo.location;
    }

    [[nodiscard]]FVector getWorldHeadPosition() const {
        auto position = getBonePosition(boneRequireCorrection() ? 51 : 46);
        position.z += 10;
        return position;
    }

    [[nodiscard]]FVector getWorldHeadPosition(float offset) const {
        auto position = getBonePosition(boneRequireCorrection() ? 51 : 46);
        position.z += offset;
        return position;
    }

    [[nodiscard]] FBasedMovementInfo getBasedMovementInfo() const{
        return read<FBasedMovementInfo>(address + 0x02A0);
    }

    [[nodiscard]]FRotator getViewDirection() const {
        return read<FRotator>(address + 0x0F48);
    }


    [[nodiscard]] FVector projectWorldToScreen(FVector worldLocation, FVector screenSize) const {
        FVector fVector = FVector(0, 0, 0);
        auto cameraCache = getCameraCache();
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

    [[nodiscard]] FVector targetCorrection(WorldSettings worldSettings, float targetDistance, FVector targetPosition, FVector targetVelocity) const {
        FVector predictedPosition = targetPosition;
        float gravity = fabs(worldSettings.worldGravityZ / worldSettings.worldToMeters);
        //float bulletSpeed = getWeaponConfig().getPrimaryAmmoConfig().initSpeed;
        float bulletSpeed = 1;
        float time = targetDistance / fabs(bulletSpeed);
        float bulletDrop = (gravity / 250) * time * time;
        predictedPosition.z += bulletDrop * 120;
        predictedPosition.x += time * (targetVelocity.x / 4);
        predictedPosition.y += time * (targetVelocity.y / 4);
        predictedPosition.z += time * (targetVelocity.z / 4);
        return predictedPosition;
    }

    [[nodiscard]] bool isDead() const {
        return health == 0 || maxHealth == 0;
    }

    [[nodiscard]] int32_t getId() const {
        return id;
    }

    [[nodiscard]]int32_t getSkinId() const {
        return skinId;
    }

    [[nodiscard]]int32_t getCharacterId() const {
        return characterID;
    }

    [[nodiscard]]const FString &getName() const {
        return name;
    }

    [[nodiscard]]float getHealth() const {
        return health;
    }

    [[nodiscard]]float getMaxHealth() const {
        return maxHealth;
    }

    [[nodiscard]] bool isBot() const {
        return isPlayerBot;
    }

    [[nodiscard]] float getEyeHeight() const {
        return eyeHeight;
    }

    [[nodiscard]] float getEyeHeightCrouched() const {
        return eyeHeightCrouched;
    }

    [[nodiscard]] DWORD_PTR getSolarTeamInfo() const {
        return solarTeamInfo;
    }

    DWORD_PTR getAddress() const {
        return address;
    }

    DWORD_PTR getRootComponent() const {
        return rootComponent;
    }

    DWORD_PTR getState() const {
        return state;
    }

    DWORD_PTR getPawn() const {
        return pawn;
    }

    [[nodiscard]] bool isValid() const {
        return address != 0;
    }

};


namespace EntityType {
    bool isVehicle(const std::string &type) {
        return strings::isEqual(type, "BP_VH_Hover_WL04_C") ||
               strings::isEqual(type, "BP_VH_Hover_9A03_C") ||
               strings::isEqual(type, "BP_VH_Hover_Soroll02_C") ||
               strings::isEqual(type, "BP_VH_Hover_Soroll03_C") ||
               strings::isEqual(type, "BP_VH_Hover_Soroll04_C") ||
               strings::isEqual(type, "BP_VH_TransTL_WL07_C") ||
               strings::isEqual(type, "BP_SolarVH_Tire_9A04_new_C") ||
               strings::isEqual(type, "BP_SolarVH_Tire_WL01_new_C") ||
               strings::isEqual(type, "VH_Leg_9A02_New_C") ||
               strings::isEqual(type, "BP_VH_Leg_WL03_New_Procedural_C") ||
               strings::isEqual(type, "BP_VH_Leg_WL06_New_C");
    }

    bool isWorldSettings(const std::string &type) {
        return strings::isEqual(type, "SolarWorldSettings");
    }

    bool isPlayer(const std::string &type) {
        return strings::isEqual(type, "BP_SolarLobbyCharacter_C") ||
               strings::isEqual(type, "BP_Character_BattleRoyaleMap01_C") ||
               strings::isEqual(type, "BP_Character_BattleRoyale_C") ||
               strings::isEqual(type, "BP_Character_TrainingMode_C") ||
               strings::isEqual(type, "BP_Character_TeamDeathMatch_C") ||
               strings::isEqual(type, "BP_Character_HunterXHunterTeamVer2_C") ||
               strings::isEqual(type, "BP_Character_Solo_C") ||
               strings::isContain(type, "BP_Character") ||
               strings::isEqual(type, "BP_Character_Bounty_C");
    }

    bool isDeadBox(std::string &type) {
        return strings::isEqual(type, "BP_DI_DeathBoxA_TreasureBox_C");
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
