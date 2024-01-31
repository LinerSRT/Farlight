#include <cstdint>

enum EBoneTranslationRetargetingMode : uint8_t {
    Animation = 0,
    Skeleton = 1,
    AnimationScaled = 2,
    AnimationRelative = 3,
    OrientAndScale = 4
};

enum EMovementMode : uint8_t {
    Movement_None = 0,
    Movement_Walking = 1,
    Movement_NavWalking = 2,
    Movement_Falling = 3,
    Movement_Swimming = 4,
    Movement_Flying = 5,
    Movement_Custom = 6
};

enum ESolarCharacterType : uint8_t {
    CharacterType_None = 0,
    CharacterType_Player = 1,
    CharacterType_Bot = 2,
    CharacterType_Monster = 3
};

enum ECharacterBodyScaleType : uint8_t {
    BodyScaleTypeNone = 0,
    BodyScaleTypeWoman = 1,
    BodyScaleTypeMan = 2,
    BodyScaleTypeDuckMan = 3,
    BodyScaleTypeSmallWoman = 4
};
enum ERescueState : uint8_t {
    RescueState_None = 0,
    RescueState_Dying = 1,
    RescueState_Rescuing = 2,
    RescueState_BeingRescue = 3
};

enum EShieldState : uint8_t {
    ShieldState_NONE = 0,
    ShieldState_SPAWN = 1,
    ShieldState_NORMAL = 2,
    ShieldState_RECHARGE = 3,
    ShieldState_BREAK = 4,
    ShieldState_PILE = 5
};

enum EWeaponMechanicalUniqueState : uint8_t {
    Idle = 1,
    PreFire = 2,
    RealFire = 3,
    Rechamber = 4,
    Reloading = 5,
    Bolting = 6,
    Charging = 7,
    Overloading = 8,
    KeyUPHolding = 9
};

enum ESCMDamageType : uint8_t {
    DamageType_Point = 0,
    DamageType_Poison = 1,
    DamageType_Bomb = 2,
    DamageType_AirDrop = 3,
    DamageType_Dying = 4,
    DamageType_VehicleHit = 5,
    DamageType_VehicleExplosion = 6,
    DamageType_Weapon = 7,
    DamageType_UnArmWeapon = 8,
    DamageType_VehicleWeapon = 9,
    DamageType_SummonWeapon = 10,
    DamageType_VehicleAbility = 11,
    DamageType_WeaponAbility = 12,
    DamageType_HelplessDeathVerge = 13,
    DamageType_DeathVergeInWater = 14,
    DamageType_GMCmd = 15,
    DamageType_BackToLobby = 16,
    DamageType_AppEnterBackground = 17,
    DamageType_LongTimeDisconnect = 18,
    DamageType_ClassSkill = 19,
    DamageType_TacticalSkill = 20,
    DamageType_Outbound = 21
};


enum ESolarWeaponBrand : uint8_t {
    WeaponBrand_None = 0,
    WeaponBrand_9A1 = 1,
    WeaponBrand_Wasteland = 2,
    WeaponBrand_Murphy = 3,
    WeaponBrand_Prism = 4
};

enum EWeaponType : uint8_t {
    WeaponType_AssaultRifle = 0,
    WeaponType_SubMachineGun = 1,
    WeaponType_Shotgun = 2,
    WeaponType_Sniper = 3,
    WeaponType_VehicleMounted = 4,
    WeaponType_ItemWeapon = 5,
    WeaponType_SummonWeapon = 6,
    WeaponType_AntiVehicle = 7,
    WeaponType_UnArm = 8,
    WeaponType_Unknown = 9
};

enum EFireMethodType : uint8_t {
    FireMethod_BULLET = 0,
    FireMethod_SKILL = 1,
    FireMethod_SUMMONBULLET = 2
};

enum ETrajectoryType : uint8_t {
    LINE = 0,
    LINE_GRAVITY = 1,
    CURVE = 2,
    CANISTER = 3,
    BEAM = 4,
    Rocket = 5,
    VirtualBullet = 6
};

enum EVirtualBulletType : uint8_t {
    VirtualBulletType_LinearBullet = 0,
    VirtualBulletType_CurveBullet = 1,
    VirtualBulletType_GravityBullet = 2,
    VirtualBulletType_TracingBullet = 3
};

enum ETreasureBoxState : uint8_t {
    TreasureBoxState_None = 0,
    TreasureBoxState_Close = 1,
    TreasureBoxState_Open = 2
};

enum ESingleCruiseState : uint8_t {
    SingleCruiseState_NONE = 0,
    SingleCruiseState_Fly = 1,
    SingleCruiseState_CanParachute = 2,
    SingleCruiseState_RequestParachute = 3,
    SingleCruiseState_Parachute = 4
};

enum ECruiseState : uint8_t {
    CruiseState_None = 0,
    CruiseState_Boarded = 1,
    CruiseState_CruisePendingStarted = 2,
    CruiseState_CruiseStarted = 3,
    CruiseState_ParachuteAllowed = 4,
    CruiseState_ParachutePendingStarted = 5,
    CruiseState_ParachuteStarted = 6,
    CruiseState_End = 7
};

enum ESkydiveStage : uint8_t {
    SkydiveStage_None = 0,
    SkydiveStage_Flying = 1,
    SkydiveStage_Landing = 2
};

enum EWallRunState : uint8_t {
    WallRunState_WallRunning = 0,
    WallRunState_Success = 1,
    WallRunState_Cancel = 2
};
enum EZiplineMoveState : uint8_t {
    ZiplineMoveState_None = 0,
    ZiplineMoveState_PreStart = 1,
    ZiplineMoveState_OnZipline = 2
};