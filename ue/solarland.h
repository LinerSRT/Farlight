#include "engine.h"

enum ESolarCharacterType : uint8_t {
    ESolarCharacterType__ESolarCharacterType_None0 = 0,
    ESolarCharacterType__ESolarCharacterType_Player1 = 1,
    ESolarCharacterType__ESolarCharacterType_Bot2 = 2,
    ESolarCharacterType__ESolarCharacterType_Monster3 = 3,
    ESolarCharacterType__ESolarCharacterType_MAX4 = 4
};

enum ESolarWeaponBrand : uint8_t {
    ESolarWeaponBrand__WeaponBrand_None0 = 0,
    ESolarWeaponBrand__WeaponBrand_9A1 = 1,
    ESolarWeaponBrand__WeaponBrand_Wasteland2 = 2,
    ESolarWeaponBrand__WeaponBrand_Murphy3 = 3,
    ESolarWeaponBrand__WeaponBrand_Prism4 = 4,
    ESolarWeaponBrand__WeaponBrand_MAX5 = 5
};

enum EWeaponType : uint8_t {
    EWeaponType__AssualtRifle0 = 0,
    EWeaponType__Submachinegun1 = 1,
    EWeaponType__Shotgun2 = 2,
    EWeaponType__Sniper3 = 3,
    EWeaponType__VehicleMounted4 = 4,
    EWeaponType__ItemWeapon5 = 5,
    EWeaponType__SummonWeapon6 = 6,
    EWeaponType__AntiVehicle7 = 7,
    EWeaponType__Unarm8 = 8,
    EWeaponType__Unknown9 = 9,
    EWeaponType__EWeaponType_MAX10 = 10
};

enum EItemType : uint32_t {
    EItemType__NONE0 = 0,
    EItemType__SHIELD1 = 101,
    EItemType__ARMOR2 = 102,
    EItemType__BULLET3 = 103,
    EItemType__CARIRIDGE_BAG4 = 104,
    EItemType__ARMOR_MATERIAL5 = 105,
    EItemType__ENERGY_MODULE6 = 106,
    EItemType__EXTRA_ENERGY7 = 107,
    EItemType__RADAR_OPERATOR8 = 108,
    EItemType__BACKPACK_ENERGY9 = 109,
    EItemType__BACKUP_ENERGY10 = 110,
    EItemType__SHIELD_RECHARGER11 = 112,
    EItemType__BACKPACK_ITEM12 = 113,
    EItemType__WEAPON_PARTS13 = 114,
    EItemType__JETPACK_MODULE_HORIZONTAL14 = 111,
    EItemType__JETPACK_MODULE_VERTICAL15 = 115,
    EItemType__REVIVE_ITEM16 = 116,
    EItemType__TREASUREBOX17 = 120,
    EItemType__AIRDROPBOX18 = 121,
    EItemType__DEATHBOX19 = 122,
    EItemType__NEUTRAL_CARD20 = 130,
    EItemType__COLLECTION_ITEM21 = 131,
    EItemType__SHIELD_UPGRADE_MATERIAL22 = 148,
    EItemType__EXP_ITEM23 = 149,
    EItemType__WEAPON24 = 151,
    EItemType__WEAPON_SKIN25 = 171,
    EItemType__BACKPACK26 = 201,
    EItemType__TAILFLAME27 = 202,
    EItemType__CARDPOSE28 = 203,
    EItemType__CARDBACKGROUND29 = 204,
    EItemType__CAPSULE30 = 251,
    EItemType__CHAR_SKIN_MIN31 = 301,
    EItemType__CHAR_ANIMATION_MVP32 = 302,
    EItemType__CHAR_SKIN_MAX33 = 350,
    EItemType__CHARACTER34 = 351,
    EItemType__EXPERIENCE35 = 401,
    EItemType__GIFTBAG36 = 404,
    EItemType__CHARACTER_TRIALCARD37 = 405,
    EItemType__CHARACTERSKIN_TRIALCARD38 = 406,
    EItemType__ACTIVENESS39 = 411,
    EItemType__WEAPONSKIN_TRIALCARD40 = 412,
    EItemType__GIFTBAG_ONBACKPACK41 = 414,
    EItemType__BACKPACK_TRIALCARD42 = 415,
    EItemType__TAILFLAME_TRIALCARD43 = 416,
    EItemType__DIAMOND_VOUCHER44 = 417,
    EItemType__LOTCOIN45 = 421,
    EItemType__ZOMBORG46 = 422,
    EItemType__WISHCOIN47 = 423,
    EItemType__SURPRISECOIN48 = 424,
    EItemType__TOKEN49 = 430,
    EItemType__BUSINESSCARDFRAME50 = 432,
    EItemType__AVATARFRAME51 = 434,
    EItemType__CHARACTER_SHARD52 = 435,
    EItemType__CHARACTER_SKIN_SHARD53 = 436,
    EItemType__WEAPON_SKIN_SHARD54 = 437,
    EItemType__BACKPACK_SHARD55 = 438,
    EItemType__TAILFLAME_SHARD56 = 439,
    EItemType__CAPSULE_SHARD57 = 440,
    EItemType__VEHICLE_SKIN_SHARD58 = 441,
    EItemType__ACCOUNT_AVATAR59 = 443,
    EItemType__EMOTE60 = 444,
    EItemType__SIGNIN_CARD61 = 447,
    EItemType__RAFFLE_TICKET62 = 448,
    EItemType__HOMEBUILD63 = 501,
    EItemType__HOMEINSTALLATION64 = 502,
    EItemType__HOMEITEM65 = 503,
    EItemType__HOMEDRAWING66 = 504,
    EItemType__HOMEPART67 = 505,
    EItemType__HOMEDEVICE68 = 506,
    EItemType__HOMEMAX69 = 520,
    EItemType__VEHICLE_SKIN70 = 701,
    EItemType__SUPPLYBOX71 = 801,
    EItemType__DISPLAY_ITEM72 = 999,
    EItemType__EItemType_MAX73 = 1000
};

enum EItemAppearanceType : uint8_t {
    EItemAppearanceType__EApprenace_None0 = 0,
    EItemAppearanceType__EApprenace_Outline1 = 1,
    EItemAppearanceType__EApprenace_Float2 = 2,
    EItemAppearanceType__EApprenace_All3 = 3,
    EItemAppearanceType__EApprenace_MAX4 = 4
};

enum EShieldState : uint8_t {
    EShieldState__NONE0 = 0,
    EShieldState__SPAWN1 = 1,
    EShieldState__NORMAL2 = 2,
    EShieldState__RECHARGE3 = 3,
    EShieldState__BREAK4 = 4,
    EShieldState__PILE5 = 5,
    EShieldState__MAX6 = 6
};

enum ESkydiveStage : uint8_t {
    ESkydiveStage__None0 = 0,
    ESkydiveStage__Flying1 = 1,
    ESkydiveStage__Landing2 = 2,
    ESkydiveStage__ESkydiveStage_MAX3 = 3
};

enum ECruiseState : uint8_t {
    ECruiseState__None0 = 0,
    ECruiseState__Boarded1 = 1,
    ECruiseState__CruisePendingStarted2 = 2,
    ECruiseState__CruiseStarted3 = 3,
    ECruiseState__ParachuteAllowed4 = 4,
    ECruiseState__ParachutePendingStarted5 = 5,
    ECruiseState__ParachuteStarted6 = 6,
    ECruiseState__End7 = 7,
    ECruiseState__Max8 = 8
};

struct FInWaterActorInfo {
    bool bIsInWater;
    unsigned char UnknownData00_5[0x3];
    float WaterLineHeight;
};

struct FCriticalHitInfo {
    unsigned char UnknownData00_5[0x8];                                      // 0x0014   (0x0054)  MISSED
    float Damage;                                                     // 0x0008   (0x0004)
    float DamageTime;                                                 // 0x000C   (0x0004)
    int32_t LastDamageWeaponID;                                         // 0x0010   (0x0004)
    unsigned char UnknownData00_6[0x54];                                      // 0x0014   (0x0054)  MISSED
};

struct FHitResult {
    bool bBlockingHit: 1;                                           // 0x0000:0 (0x0001)
    bool bStartPenetrating: 1;                                      // 0x0000:1 (0x0001)
    unsigned char UnknownData00_4[0x3];                                       // 0x0001   (0x0003)  MISSED
    int32_t FaceIndex;                                                  // 0x0004   (0x0004)
    float Time;                                                       // 0x0008   (0x0004)
    float Distance;                                                   // 0x000C   (0x0004)
    FVector Location;                                                   // 0x0010   (0x000C)
    FVector ImpactPoint;                                                // 0x001C   (0x000C)
    FVector Normal;                                                     // 0x0028   (0x000C)
    FVector ImpactNormal;                                               // 0x0034   (0x000C)
    FVector TraceStart;                                                 // 0x0040   (0x000C)
    FVector TraceEnd;                                                   // 0x004C   (0x000C)
    float PenetrationDepth;                                           // 0x0058   (0x0004)
    int32_t Item;                                                       // 0x005C   (0x0004)
    unsigned char UnknownData00_5[0x8];                                       // 0x0001   (0x0003)  MISSED
    unsigned char UnknownData00_6[0x8];                                       // 0x0001   (0x0003)  MISSED
    unsigned char UnknownData00_7[0x8];                                       // 0x0001   (0x0003)  MISSED
    FName BoneName;                                                   // 0x0078   (0x0008)
    FName MyBoneName;                                                 // 0x0080   (0x0008)
};


struct FSolarItemData {
    int32_t ItemID;                                                     // 0x000C   (0x0004)
    int64_t ThisID;                                                     // 0x0010   (0x0008)
    FString Name;                                                       // 0x0018   (0x0010)
    FString Icon;                                                       // 0x0028   (0x0010)
    FString Info;                                                       // 0x0038   (0x0010)
    int32_t count;                                                      // 0x0048   (0x0004)
    EItemType ItemType;                                                   // 0x004C   (0x0004)
    int32_t Quality;                                                    // 0x0050   (0x0004)
    int32_t MaxPile;                                                    // 0x0054   (0x0004)
    bool IfReplace;                                                  // 0x0058   (0x0001)
    unsigned char UnknownData00_5[0x7];                                       // 0x0059   (0x0007)  MISSED
    FString AbilityBP;                                                  // 0x0060   (0x0010)
    FString ItemBP;                                                     // 0x0070   (0x0010)
    int32_t DropTime;                                                   // 0x0080   (0x0004)
    int32_t UnPickupTime;                                               // 0x0084   (0x0004)
    bool IsRecommend;                                                // 0x0088   (0x0001)
    bool bDirty;                                                     // 0x0089   (0x0001)
    bool IsPickupLock;                                               // 0x008A   (0x0001)
    unsigned char UnknownData01_5[0x5];                                       // 0x008B   (0x0005)  MISSED
    int64_t UserId;                                                     // 0x0090   (0x0008)
    int32_t Level;                                                      // 0x0098   (0x0004)
    int32_t Prize;                                                      // 0x009C   (0x0004)
    float CurValue;                                                   // 0x00A0   (0x0004)
    bool UseOnPickup;                                                // 0x00A4   (0x0001)
    EItemAppearanceType AppearanceType;                                             // 0x00A5   (0x0001)
    bool bUpdateOverlap;                                             // 0x00A6   (0x0001)
    unsigned char UnknownData02_5[0x1];                                       // 0x00A7   (0x0001)  MISSED
    int32_t SourceType;                                                 // 0x00A8   (0x0004)
    unsigned char UnknownData03_6[0xC];                                       // 0x00AC   (0x000C)  MISSED
};

class APlayerState : public UObject {
public:
    float score;
    int32_t playerID;
    char ping;
    bool isSpectator;
    bool onlySpectator;
    bool isABot;
    bool isInactive;
    bool fromPreviousLevel;
    FString savedNetworkAddress;
    FString playerNamePrivate;

    explicit APlayerState(DWORD_PTR pointer) :
            UObject(pointer),
            score(read<float>(pointer + 0x0228)),
            playerID(read<int32_t>(pointer + 0x022C)),
            ping(read<char>(pointer + 0x0230)),
            isSpectator((read<uint8_t>(pointer + 0x0232) & 1) == 0),
            onlySpectator((read<uint8_t>(pointer + 0x0232) & 2) == 0),
            isABot((read<uint8_t>(pointer + 0x0232) & 3) == 0),
            isInactive((read<uint8_t>(pointer + 0x0232) & 5) == 0),
            fromPreviousLevel((read<uint8_t>(pointer + 0x0232) & 6) == 0),
            savedNetworkAddress(read<FString>(pointer + 0x0250)),
            playerNamePrivate(read<FString>(pointer + 0x0310)) {}
};


class APawn : public UObject {
public:
    float baseEyeHeight;
    USceneComponent rootComponent;
    APlayerState playerState;

    explicit APawn(DWORD_PTR pointer) :
            UObject(pointer),
            baseEyeHeight(read<float>(pointer + 0x0234)),
            rootComponent(USceneComponent(pointer + 0x0138)),
            playerState(APlayerState(pointer + 0x0248)) {}
};

class ADefaultPawn : public APawn {
public:
    UStaticMeshComponent meshComponent;


    explicit ADefaultPawn(DWORD_PTR pointer) :
            APawn(pointer),
            meshComponent(UStaticMeshComponent(pointer + 0x02A0)) {}
};

class ACharacter : public APawn {
public:
    USkeletalMeshComponent mesh;
    UCharacterMovementComponent characterMovement;
    UCapsuleComponent capsuleComponent;
    FBasedMovementInfo basedMovement;
    FBasedMovementInfo replicatedBasedMovement;
    FVector baseTranslationOffset;
    FQuat baseRotationOffset;
    bool isCrouched;
    bool pressedJump;
    bool clientUpdating;
    bool clientWasFalling;
    bool wasJumping;

    explicit ACharacter(DWORD_PTR pointer) :
            APawn(pointer),
            mesh(USkeletalMeshComponent(pointer + 0x0288)),
            characterMovement(UCharacterMovementComponent(pointer + 0x0290)),
            capsuleComponent(UCapsuleComponent(pointer + 0x0298)),
            basedMovement(FBasedMovementInfo(pointer + 0x02A0)),
            replicatedBasedMovement(FBasedMovementInfo(pointer + 0x02D0)),
            baseTranslationOffset(read<FVector>(pointer + 0x0304)),
            baseRotationOffset(read<FQuat>(pointer + 0x0310)),
            isCrouched((read<uint8_t>(pointer + 0x0330) & 0x01) != 0),
            pressedJump(false),
            clientUpdating(false),
            clientWasFalling((read<uint8_t>(pointer + 0x0331) & 0x10) != 0),
            wasJumping(false) {}
};

class AActor : public UObject {
public:
    ADefaultPawn pawn;
    USceneComponent rootComponent;

    explicit AActor(DWORD_PTR pointer) : UObject(pointer), pawn(ADefaultPawn(pointer + 0x0120)), rootComponent(USceneComponent(pointer + 0x0138)) {}
};

struct FTViewTarget {
    AActor *target;
    unsigned char UnknownData00_5[0x8];
    FMinimalViewInfo POV;
    APlayerState *playerState;
    unsigned char UnknownData01_6[0x8];
};

class APlayerCameraManager : public AActor {
public:
    USceneComponent transformComponent;
    float defaultFOV;
    float defaultOrthoWidth;
    float defaultAspectRatio;
    FCameraCacheEntry cameraCache;
    FCameraCacheEntry lastFrameCameraCache;
    FTViewTarget viewTarget;
    FTViewTarget pendingViewTarget;
    FCameraCacheEntry cameraCachePrivate;
    FCameraCacheEntry lastFrameCameraCachePrivate;
    float freeCamDistance;
    FVector freeCamOffset;
    FVector viewTargetOffset;
    float viewPitchMin;
    float viewPitchMax;
    float viewYawMin;
    float viewYawMax;
    float viewRollMin;
    float viewRollMax;
    float serverUpdateCameraTimeout;

    explicit APlayerCameraManager(DWORD_PTR pointer) :
            AActor(pointer),
            transformComponent(USceneComponent(pointer + 0x0230)),
            defaultFOV(read<float>(pointer + 0x0240)),
            defaultOrthoWidth(read<float>(pointer + 0x0248)),
            defaultAspectRatio(read<float>(pointer + 0x0250)),
            cameraCache(read<FCameraCacheEntry>(pointer + 0x02A0)),
            lastFrameCameraCache(read<FCameraCacheEntry>(pointer + 0x0900)),
            viewTarget(read<FTViewTarget>(pointer + 0x0F60)),
            pendingViewTarget(read<FTViewTarget>(pointer + 0x15D0)),
            cameraCachePrivate(read<FCameraCacheEntry>(pointer + 0x1C70)),
            lastFrameCameraCachePrivate(read<FCameraCacheEntry>(pointer + 0x22D0)),
            freeCamDistance(read<float>(pointer + 0x2950)),
            freeCamOffset(read<FVector>(pointer + 0x2954)),
            viewTargetOffset(read<FVector>(pointer + 0x2960)),
            viewPitchMin(read<float>(pointer + 0x2A24)),
            viewPitchMax(read<float>(pointer + 0x2A28)),
            viewYawMin(read<float>(pointer + 0x2A2C)),
            viewYawMax(read<float>(pointer + 0x2A30)),
            viewRollMin(read<float>(pointer + 0x2A34)),
            viewRollMax(read<float>(pointer + 0x2A38)),
            serverUpdateCameraTimeout(read<float>(pointer + 0x2A40)) {}
};


class APlayerController : public UObject {
public:
    APawn acknowledgedPawn;
    APlayerCameraManager cameraManager;
    FRotator targetViewRotation;
    float smoothTargetViewRotationSpeed;
    TArray<DWORD_PTR> hiddenActors;
    bool isLocalPlayerController;

    explicit APlayerController(DWORD_PTR pointer) :
            UObject(pointer),
            acknowledgedPawn(APawn(pointer + 0x02C0)),
            cameraManager(APlayerCameraManager(pointer + 0x02D8)),
            targetViewRotation(read<FRotator>(pointer + 0x02EC)),
            smoothTargetViewRotationSpeed(read<float>(pointer + 0x0304)),
            hiddenActors(read<TArray<DWORD_PTR>>(pointer + 0x0310)),
            isLocalPlayerController(read<bool>(pointer + 0x058C)) {}


    AActor getHiddenActor(int position) {
        return AActor(hiddenActors.getValuePointer(position));
    }

    [[nodiscard]] int getHiddenActorsSize() const {
        return hiddenActors.length();
    }
};

class SolarActor : public AActor {
public:
    APlayerController playerController;

    explicit SolarActor(DWORD_PTR pointer) : AActor(pointer), playerController(APlayerController(pointer + 0x0030)) {}
};

class ULevel : public UObject {
public:


    TArray<DWORD_PTR> actors;

    explicit ULevel(DWORD_PTR pointer) : UObject(pointer), actors(read<TArray<DWORD_PTR>>(pointer + 0x0098)) {}

    [[nodiscard]] SolarActor getActorAt(int position) {
        return SolarActor(actors.getValuePointer(position));
    }

    DWORD_PTR getCharacterPointer() {
        for (int i = 0; i < getActorSize(); ++i) {
            auto actor = getActorAt(i);
            if (actor.getNamePrivate().toString(baseId).find("BP_Character") != std::string::npos) {
                return actor.getPointer();
            }
        }
        return 0;
    }

    [[nodiscard]] int getActorSize() const {
        return actors.length();
    }

    std::vector<DWORD_PTR> getItems() {
        std::vector<DWORD_PTR> list;
        for (int i = 0; i < actors.length(); ++i) {
            auto actor = SolarActor(actors.getValuePointer(i));
            if (actor.getNamePrivate().toString(baseId).find("SolarItemActor") != std::string::npos || actor.getNamePrivate().toString(baseId).find("SolarGroundPreviewActor") != std::string::npos) {
                list.push_back(actor.getPointer());
            }
        }
        return list;
    }

    std::vector<DWORD_PTR> getCharacters() {
        std::vector<DWORD_PTR> list;
        for (int i = 0; i < actors.length(); ++i) {
            auto actor = SolarActor(actors.getValuePointer(i));
            if (actor.getNamePrivate().toString(baseId).find("BP_Character") != std::string::npos) {
                list.push_back(actor.getPointer());
            }
        }
        return list;
    }
};


class UWorld : public UObject {
public:
    ULevel persistentLevel;
    UGameInstance owningGameInstance;
    TArray<DWORD_PTR> levels;

    explicit UWorld(DWORD_PTR pointer) :
            UObject(pointer),
            persistentLevel(ULevel(pointer + 0x0030)),
            owningGameInstance(UGameInstance(pointer + 0x0220)),
            levels(read<TArray<DWORD_PTR>>(pointer + 0x01E0)) {}


    ULevel getLevel(int position) {
        return ULevel(levels.getValuePointer(position));
    }


    [[nodiscard]] ULevel getWorldLevel() {
        for (int i = 0; i < getLevelsSize(); ++i) {
            auto level = getLevel(i);
            if (!level.getItems().empty())
                return level;
        }
        return getLevel(0);
    }


    [[nodiscard]] int getLevelsSize() const {
        return levels.length();
    }
};

class UAmmoConfig : public UObject {
public:
    FString Name;
    FString Des;

    explicit UAmmoConfig(DWORD_PTR pointer) : UObject(pointer), Name(read<FString>(pointer + 0x0080)), Des(read<FString>(pointer + 0x0090)) {}
};

class USingleWeaponConfig : public UObject {
public:
    ESolarWeaponBrand WeaponBrand;
    EWeaponType WeaponType;
    bool bSupportSecondaryFire;
    bool bIsSingleWeapon;
    bool bCanSuspendBolt;
    bool bIsHeavyFireWeapon;
    bool bIsFlamethrowerWeapon;
    bool bSupportSingleShootMode;
    bool bSupportAutoShootMode;
    bool bSupportBurstShootMode;
    float MaxSpread;
    float MinSpread;
    float HipFireBaseSpread;
    float ShoulderFireBaseSpread;
    float ADSBaseSpread;
    float VhADSBaseSpread;
    bool bCalcSpreadByStandardDistance;
    float SpreadStandardDistance;
    float SpreadStandardOriginMaxRange;
    UAmmoConfig PrimaryAmmo;
    int32_t PrimaryAmmoIndex;
    int32_t PrimaryAmmoCap;
    UAmmoConfig SecondaryAmmo;
    int32_t SecondaryAmmoIndex;
    int32_t SecondaryAmmoCap;
    float ScopeOpenTime;
    FString WeaponTextType;

    explicit USingleWeaponConfig(DWORD_PTR pointer) :
            UObject(pointer),
            WeaponBrand(read<ESolarWeaponBrand>(pointer + 0x0034)),
            WeaponType(read<EWeaponType>(pointer + 0x0035)),
            bSupportSecondaryFire(read<bool>(pointer + 0x0036)),
            bIsSingleWeapon(read<bool>(pointer + 0x00B0)),
            bCanSuspendBolt(read<bool>(pointer + 0x00B1)),
            bIsHeavyFireWeapon(read<bool>(pointer + 0x00B2)),
            bIsFlamethrowerWeapon(read<bool>(pointer + 0x00B3)),
            bSupportSingleShootMode(read<bool>(pointer + 0x00B4)),
            bSupportAutoShootMode(read<bool>(pointer + 0x00B5)),
            bSupportBurstShootMode(read<bool>(pointer + 0x00B6)),
            MaxSpread(read<bool>(pointer + 0x00B8)),
            MinSpread(read<bool>(pointer + 0x00BC)),
            HipFireBaseSpread(read<bool>(pointer + 0x00C0)),
            ShoulderFireBaseSpread(read<bool>(pointer + 0x00C4)),
            ADSBaseSpread(read<bool>(pointer + 0x00C8)),
            VhADSBaseSpread(read<bool>(pointer + 0x00CC)),
            bCalcSpreadByStandardDistance(read<bool>(pointer + 0x00D0)),
            SpreadStandardDistance(read<bool>(pointer + 0x00D4)),
            SpreadStandardOriginMaxRange(read<bool>(pointer + 0x00D8)),
            PrimaryAmmo(UAmmoConfig(pointer + 0x0280)),
            PrimaryAmmoIndex(read<bool>(pointer + 0x0288)),
            PrimaryAmmoCap(read<bool>(pointer + 0x028C)),
            SecondaryAmmo(UAmmoConfig(pointer + 0x0290)),
            SecondaryAmmoIndex(read<bool>(pointer + 0x0298)),
            SecondaryAmmoCap(read<bool>(pointer + 0x029C)),
            ScopeOpenTime(read<float>(pointer + 0x0444)),
            WeaponTextType(read<FString>(pointer + 0x06D8)) {}
};


class ASolarWeapon : public AActor {
public:
    AActor AimTarget;
    USingleWeaponConfig config;
    int32_t weaponId;
    bool bTeamDamageEnable;
    bool bInfiniteAmmo;
    bool bInfinitePackageAmmo;
    int32_t ExtraAmmo;
    bool bDetectedEnemy;
    FHitResult AimHitResult;

    explicit ASolarWeapon(DWORD_PTR pointer) :
            AActor(pointer),
            AimTarget(AActor(pointer + 0x04D0)),
            config(USingleWeaponConfig(pointer + 0x0500)),
            weaponId(read<int32_t>(pointer + 0x0518)),
            bTeamDamageEnable(read<bool>(pointer + 0x0524)),
            bInfiniteAmmo(read<bool>(pointer + 0x0525)),
            bInfinitePackageAmmo(read<bool>(pointer + 0x0526)),
            ExtraAmmo(read<int32_t>(pointer + 0x0528)),
            bDetectedEnemy(read<bool>(pointer + 0x0760)),
            AimHitResult(read<FHitResult>(pointer + 0x0760)) {}
};

class ASolarPlayerWeapon : public ASolarWeapon {
public:
    explicit ASolarPlayerWeapon(DWORD_PTR pointer) : ASolarWeapon(pointer) {}
};

class ASolarCharacterBase : public ACharacter {
public:
    FInWaterActorInfo inWaterActorInfo;

    explicit ASolarCharacterBase(DWORD_PTR pointer) : ACharacter(pointer), inWaterActorInfo(read<FInWaterActorInfo>(pointer + 0x0508)) {}
};

class ASolarCharacter : public ASolarCharacterBase {
public:
    ESolarCharacterType characterType;
    char energyState;
    float maxEnergyValue;
    float currEnergyValue;
    float maxExtraEnergyValue;
    float currExtraEnergyValue;
    FRotator characterLookAt;
    float pendingRegeneration;
    float diedTime;
    float rescueTime;
    TArray<ASolarPlayerWeapon> deathBoxWeaponArray;
    TArray<FSolarItemData> deathBoxArray;
    float deathBoxEnergy;
    int32_t currShieldId;
    int32_t curUniqueShieldId;
    float maxShieldValue;
    int32_t nextUpgradeShieldID;
    int32_t nextUpgradeShieldEnergy;
    float currShieldValue;
    EShieldState currShieldState;
    int32_t currShieldLevel;
    float armorReduceValue;
    float headHitValue;
    TArray<FCriticalHitInfo> CriticalHits;
    float StandJogSpeed;
    float StandRunSpeed;
    float StandSprintSpeed;
    float CrouchJogSpeed;
    float CrouchRunSpeed;
    float CrouchSprintSpeed;
    bool bIsSprintingMove;
    UCharacterMovementComponent cachedSolarCharacterMovement;
    FVector PrepareToParachuteLocation;
    FVector AgreedParachuteLocation;
    int32_t CapsuleID;
    ECruiseState CruiseState;
    ESkydiveStage CurrentSkydiveStage;
    ASolarPlayerWeapon TempSwitchWeapon;
    ASolarPlayerWeapon CachedCurrentWeapon;
    FString DebugStringBotAIInfo;
    FString DebugStringToDrawGASServer;
    FString DebugStringDSTransferLog;

    ASolarCharacter(DWORD_PTR pointer) : ASolarCharacterBase(pointer),
                                         characterType(read<ESolarCharacterType>(pointer + 0x06C0)),
                                         energyState(read<char>(pointer + 0x0EB0)),
                                         maxEnergyValue(read<float>(pointer + 0x0EB4)),
                                         currEnergyValue(read<float>(pointer + 0x0EB8)),
                                         maxExtraEnergyValue(read<float>(pointer + 0x0EBC)),
                                         currExtraEnergyValue(read<float>(pointer + 0x0EC0)),
                                         characterLookAt(read<FRotator>(pointer + 0x0F20)),
                                         pendingRegeneration(read<float>(pointer + 0x0F88)),
                                         diedTime(read<float>(pointer + 0x0F8C)),
                                         rescueTime(read<float>(pointer + 0x1034)),
                                         deathBoxWeaponArray(read<TArray<ASolarPlayerWeapon>>(pointer + 0x1040)),
                                         deathBoxArray(read<TArray<FSolarItemData>>(pointer + 0x1050)),
                                         deathBoxEnergy(read<ESolarCharacterType>(pointer + 0x06C0)),
                                         currShieldId(read<int32_t>(pointer + 0x116C)),
                                         curUniqueShieldId(read<int32_t>(pointer + 0x1170)),
                                         maxShieldValue(read<float>(pointer + 0x1174)),
                                         nextUpgradeShieldID(read<int32_t>(pointer + 0x1178)),
                                         nextUpgradeShieldEnergy(read<int32_t>(pointer + 0x117C)),
                                         currShieldValue(read<float>(pointer + 0x1180)),
                                         currShieldState(read<EShieldState>(pointer + 0x1184)),
                                         currShieldLevel(read<int32_t>(pointer + 0x1188)),
                                         armorReduceValue(read<float>(pointer + 0x179C)),
                                         headHitValue(read<float>(pointer + 0x17A0)),
                                         CriticalHits(read<TArray<FCriticalHitInfo>>(pointer + 0x17D0)),
                                         StandJogSpeed(read<float>(pointer + 0x1910)),
                                         StandRunSpeed(read<float>(pointer + 0x1914)),
                                         StandSprintSpeed(read<float>(pointer + 0x1918)),
                                         CrouchJogSpeed(read<float>(pointer + 0x191C)),
                                         CrouchRunSpeed(read<float>(pointer + 0x1920)),
                                         CrouchSprintSpeed(read<float>(pointer + 0x1924)),
                                         bIsSprintingMove(read<bool>(pointer + 0x1940)),
                                         cachedSolarCharacterMovement(UCharacterMovementComponent(pointer + 0x1998)),
                                         PrepareToParachuteLocation(read<FVector>(pointer + 0x1A40)),
                                         AgreedParachuteLocation(read<FVector>(pointer + 0x1A4C)),
                                         CapsuleID(read<int32_t>(pointer + 0x1A70)),
                                         CruiseState(read<ECruiseState>(pointer + 0x1AA0)),
                                         CurrentSkydiveStage(read<ESkydiveStage>(pointer + 0x1B5C)),
                                         TempSwitchWeapon(ASolarPlayerWeapon(pointer + 0x1C20)),
                                         CachedCurrentWeapon(ASolarPlayerWeapon(pointer + 0x1C28)),
                                         DebugStringBotAIInfo(read<FString>(pointer + 0x1FA8)),
                                         DebugStringToDrawGASServer(read<FString>(pointer + 0x1FB8)),
                                         DebugStringDSTransferLog(read<FString>(pointer + 0x1FC8)) {}
};


class UPlayer : public UObject {
public:
    APlayerController playerController;                                           // 0x0030   (0x0008)
    explicit UPlayer(DWORD_PTR pointer) : UObject(pointer), playerController(APlayerController(pointer + 0x0030)) {}
};

class ASolarGroundPreviewActor : public AActor {
public:
    explicit ASolarGroundPreviewActor(DWORD_PTR pointer) : AActor(pointer) {}
};
