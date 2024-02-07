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

enum ETalentState : uint8_t {
    TalentState_Activating = 0,
    TalentState_CoolingDown = 1,
    TalentState_Unactivated = 2,
    TalentState_Invalid = 3
};

enum class ECommonInputType : uint8_t {
    InputType_MouseAndKeyboard = 0,
    InputType_Gamepad = 1,
    InputType_Touch = 2,
    InputType_Count = 3
};


enum EPlayerNetStateInGame : uint8_t {
    NetStateInGame_None0 = 0,
    NetStateInGame_Online1 = 1,
    NetStateInGame_Offline2 = 2
};

enum ESkydivingState : uint8_t {
    SkydivingState_NotStarted0 = 0,
    SkydivingState_PreStart1 = 1,
    SkydivingState_InProgress2 = 2,
    SkydivingState_Completed3 = 3
};

enum EMatchResult : uint8_t {
    MatchResult_None = 0,
    MatchResult_Victory = 1,
    MatchResult_Fail = 2,
    MatchResult_Draw = 3
};

enum EPlayerWeaponEquipStatus : uint8_t {
    WeaponEquipStatus_None = 0,
    WeaponEquipStatus_Holding = 1,
    WeaponEquipStatus_PendingToHang = 2,
    WeaponEquipStatus_Hanging = 3
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

enum EBResurrectionState : uint8_t {
    NoneState = 0,
    WaitForApply = 1,
    NotApply = 2,
    Resurrected = 3,
    WaitForResurrect = 4,
    GiveUp = 5,
    Timeout = 6,
    AllTeammatesDied = 7,
    EBResurrectionState_MAX = 8
};

enum EItemType : uint32_t {
    NONE = 0,
    SHIELD = 101,
    ARMOR = 102,
    BULLET = 103,
    CARTRIDGE_BAG = 104,
    ARMOR_MATERIAL = 105,
    ENERGY_MODULE = 106,
    EXTRA_ENERGY = 107,
    RADAR_OPERATOR = 108,
    BACKPACK_ENERGY = 109,
    BACKUP_ENERGY = 110,
    SHIELD_RECHARGER = 112,
    BACKPACK_ITEM = 113,
    WEAPON_PARTS = 114,
    JETPACK_MODULE_HORIZONTAL = 111,
    JETPACK_MODULE_VERTICAL = 115,
    REVIVE_ITEM = 116,
    TREASURE_BOX = 120,
    AIRDROP_BOX = 121,
    DEATH_BOX_1 = 122,
    HOT_SPRINT_BOX = 123,
    NEUTRAL_CARD = 130,
    COLLECTION_ITEM = 131,
    SHIELD_UPGRADE_MATERIAL = 148,
    EXP_ITEM = 149,
    WEAPON = 151,
    WEAPON_SKIN = 171,
    MISSION_SPAWN = 160,
    MISSION_CHEST = 161,
    MISSION_WORSHIP = 162,
    BACKPACK = 201,
    TAIL_FLAME = 202,
    CARD_POSE = 203,
    CARD_BACKGROUND = 204,
    CAPSULE = 251,
    CHAR_SKIN_MIN = 301,
    CHAR_ANIMATION_MVP = 302,
    CHAR_SKIN_MAX = 350,
    CHARACTER_1 = 351,
    EXPERIENCE = 401,
    GIFT_BAG = 404,
    CHARACTER_TRIAL_CARD = 405,
    CHARACTER_SKIN_TRIAL_CARD = 406,
    ACTIVENESS = 411,
    WEAPON_SKIN_TRIAL_CARD = 412,
    GIFT_BAG_ON_BACKPACK = 414,
    BACKPACK_TRIAL_CARD = 415,
    TAIL_FLAME_TRIAL_CARD = 416,
    DIAMOND_VOUCHER = 417,
    LOT_COIN = 421,
    ZOMBORG = 422,
    WISH_COIN = 423,
    SURPRISE_COIN = 424,
    TOKEN = 430,
    BUSINESS_CARD_FRAME = 432,
    AVATAR_FRAME = 434,
    CHARACTER_SHARD = 435,
    CHARACTER_SKIN_SHARD = 436,
    WEAPON_SKIN_SHARD = 437,
    BACKPACK_SHARD = 438,
    TAIL_FLAME_SHARD = 439,
    CAPSULE_SHARD = 440,
    VEHICLE_SKIN_SHARD = 441,
    ACCOUNT_AVATAR = 443,
    EMOTE = 444,
    SIGN_IN_CARD = 447,
    RAFFLE_TICKET = 448,
    VEHICLE_SKIN = 701,
    SUPPLY_BOX = 801,
    RANDOM_PACK = 901,
    DISPLAY_ITEM = 999,
    EItemType_MAX = 1000
};

enum EWeaponPartType : uint8_t {
    Muzzle0 = 0,
    Scope1 = 1,
    Clip2 = 2,
    Rune3 = 3,
    Grip4 = 4,
    GunStock5 = 5,
    Scope2x6 = 6,
    Scope4x7 = 7,
    Scope8x8 = 8,
    MAX9 = 9
};

enum EEntityType : uint8_t {
    UNKNOWN_ENTITY = 99,
    CHARACTER = 0, //ASolarCharacter
    LOCAL_CHARACTER = 1, //ASolarCharacter
    LOBBY_CHARACTER = 2, //ASolarLobbyCharacter
    VEHICLE = 3, //ASolarVehiclePawn
    OBJECT = 4, //ASummonItemBase
    ITEM = 5, //ASummonItemBase
    ITEM_BOX = 6, //ASummonItemBase
    DEATH_BOX = 7 //ADeathTreasureBox, ABP_DBox_Default_C
};