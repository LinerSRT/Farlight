
#include <Windows.h>
#define STEAM
namespace offsets {
#ifdef STEAM
    DWORD gWorld = 0x715DD68;
    DWORD gNames = 0x6FED200;
    DWORD gObject = 0x701D150;
#elifdef STANDALONE
    DWORD gWorld = 0x715DD68; // Old: 0x7033488
    DWORD gNames = 0x6FED200; // Old: 0x6EBB780
    DWORD gObject = 0x701D150; // Old: 0x6ED40E0
#endif


    /// Class /Script/CoreUObject.ObjectArray
    DWORD oaObjectArrayToNumElements = 0x14;

    /// Class /Script/CoreUObject.Field
    DWORD fFieldToText = 0x0028; //UObject

    /// Class /Script/CoreUObject.Object
    DWORD oObjectToInternalIndex = 0xC; //UObjectToInternalIndex
    DWORD oObjectToClassPrivate = 0x10; //UObjectToClassPrivate
    DWORD oNamePrivate = 0x0018; //FName
    DWORD oOuterPrivate = 0x0020; //UObject

    /// Class /Script/Engine.Level
    DWORD lOwningWorld = 0x00B8; //UWorld
    DWORD lActorCluster = 0x00D8; //ULevelActorContainer
    DWORD lWorldSettings = 0x0298; //AWorldSettings

    /// Class /Script/Engine.LevelActorContainer
    DWORD lacActors = 0x0028; //TArray<AActor*>

    /// Class /Script/Engine.World
    DWORD wPersistentLevel = 0x0030; //ULevel
    DWORD wLevelSequenceActors = 0x0088; //TArray<AActor*>
    DWORD wAuthorityGameMode = 0x01C0; //AGameModeBase
    DWORD wGameState = 0x01C8; //AGameStateBase
    DWORD wLevels = 0x01E0; //TArray<ULevel*>
    DWORD wOwningGameInstance = 0x0220; //UGameInstance

    /// Class /Script/Engine.GameMode
    DWORD gmMatchState = 0x02C8; //FName
    DWORD gmNumSpectators = 0x02D4; //int32_t
    DWORD gmNumPlayers = 0x02D8; //int32_t
    DWORD gmNumBots = 0x02DC; //int32_t
    DWORD gmServerStatReplicator = 0x0290; //AServerStatReplicator

    /// Class /Script/Engine.ServerStatReplicator
    DWORD ssrNumRelevantActors = 0x02B0; //uint32_t
    DWORD ssrNumRelevantDeletedActors = 0x02B4; //uint32_t

    /// Class /Script/Engine.GameInstance
    DWORD giLocalPlayers = 0x0038; //TArray<ULocalPlayer*>

    /// Class /Script/Engine.NetConnection
    DWORD ncOwningActor = 0x0098; //AActor

    /// Class /Script/Engine.Actor
    DWORD pPlayerController = 0x0030; //APlayerController

    /// Class /Script/Engine.Player
    DWORD pRootComponent = 0x0138; //USceneComponent

    /// Class /Script/Solarland.SolarVehiclePawn
    DWORD vRootComponent = 0x02D0; //UPrimitiveComponent
    DWORD vVehicleMesh = 0x02D8; //UPrimitiveComponent
    DWORD vVehicleID = 0x02E0; //uint32_t
    DWORD vSeatSlots = 0x0378; //TArray<FVehicleSeatSlot>

    /// Class /Script/Engine.SceneComponent
    DWORD scRelativeLocation = 0x011C; //FVector
    DWORD scRelativeRotation = 0x0128; //FRotator
    DWORD scRelativeScale3D = 0x0134; //FRotator
    DWORD scComponentVelocity = 0x0140; //FRotator

    /// Class /Script/Engine.PlayerController
    DWORD pcPlayer = 0x02B8; //UPlayer
    DWORD pcAcknowledgedPawn = 0x02C0; //APawn
    DWORD pcPlayerCameraManager = 0x02D8; //APlayerCameraManager
    DWORD pcTargetViewRotation = 0x02EC; //FRotator

    /// Class /Script/Engine.Pawn
    DWORD pPlayerState = 0x0248; //APlayerState
    DWORD pController = 0x0260; //APlayerState
    DWORD pWeaponSystemComponent = 0x1CC0; //WeaponSystemComponent

    /// Class /Script/Engine.Character
    DWORD cMesh = 0x0288; //USkeletalMeshComponent
    DWORD cCharacterMovement = 0x0290; //UCharacterMovementComponent
    DWORD cCapsuleComponent = 0x0298; //UCharacterMovementComponent

    /// Class /Script/Engine.PlayerCameraManager
    DWORD pcmTransformComponent = 0x0230; //USceneComponent
    DWORD pcmCameraCache = 0x02A0; //FCameraCacheEntry
    DWORD pcmCameraCachePrivate = 0x1C70; //FCameraCacheEntry

    /// Class /Script/Solarland.SolarPlayerState
    DWORD spsId = 0x022C; //int32_t
    DWORD spsGold = 0x0380; //int32_t
    DWORD spsCharacterId = 0x03A8; //int32_t
    DWORD spsSkinId = 0x03AC; //int32_t
    DWORD spsClanId = 0x03B0; //FString
    DWORD spsClanName = 0x03C0; //FString
    DWORD spsClanShortName = 0x03D0; //FString
    DWORD spsSolarPlayerID = 0x03E8; //FString
    DWORD spsNickName = 0x03F8; //FString
    DWORD spsUserId = 0x0418; //uint64_t
    DWORD spsPosInTeam = 0x0420; //char
    DWORD spsCharacterLevel = 0x0424; //int32_t
    DWORD spsAccountLevel = 0x044C; //int32_t
    DWORD spsPlayerNumber = 0x0580; //uint32_t
    DWORD spsCurrentHealth = 0x05D8; //float
    DWORD spsMaxHealth = 0x05DC; //float
    DWORD spsCurShieldInfo = 0x05E0; //FShieldInfo
    DWORD spsTemporaryShieldValue = 0x05E8; //float
    DWORD spsSpectatorNum = 0x0620; //int32_t
    DWORD spsTalentState = 0x0624; //ETalentState
    DWORD spsOS = 0x06B8; //FString
    DWORD spsOsType = 0x06C8; //FString
    DWORD spsDeviceModel = 0x06D8; //FString
    DWORD spsInputType = 0x06E8; //ECommonInputType
    DWORD spsUsedGamePad = 0x06E9; //bool
    DWORD spsIsSameTeamWithLocalPlayer = 0x06EA; //bool
    DWORD spsLobbyTeamMemberNum = 0x0733; //char
    DWORD spsKillNum = 0x0820; //int32_t
    DWORD spsAssistNum = 0x0840; //int32_t
    DWORD spsDeathPos = 0x0860; //FVector
    DWORD spsLifeTime = 0x0900; //float
    DWORD spsPlayerRank = 0x09E0; //int32_t
    DWORD spsMVP = 0x09E4; //bool
    DWORD spsMvpScore = 0x09E8; //float
    DWORD spsKDA = 0x09EC; //float
    DWORD spsPlayerLocation = 0x0BA0; //FVector
    DWORD spsTeam = 0x0C48; //ASolarTeamInfo

    /// Class /Script/Solarland.SolarTeamInfo
    DWORD stiTeamID = 0x0240; //char
    DWORD stiTeamRank = 0x0241; //char
    DWORD stiKillCount = 0x0242; //char
    DWORD stiTeamName = 0x0248; //FText
    DWORD stiBotMembers = 0x0268; //TArray<ASolarPlayerState*>
    DWORD stiPlayerMembers = 0x0278; //TArray<ASolarPlayerState*>
    DWORD stiTeamMembers = 0x0288; //TArray<ASolarPlayerState*>
    DWORD stiTeamInfoDetails = 0x0298; //USolarTeamInfoComponent

    /// Class /Script/Solarland.SolarItemActor
    DWORD siaItemData = 0x0270; //FSolarItemData
    DWORD siaDroppedLocation = 0x0338; //FVector
    DWORD siaItemCurrentState = 0x03B4; //TEnumAsByte<EItemState>

    /// Class /Script/Engine.SkeletalMeshComponent
    DWORD RootBoneTranslation = 0x0808; //FVector

    /// Class /Script/Engine.SkinnedMeshComponent
    DWORD smcSkeletalMesh = 0x5D0; //USkeletalMesh

    /// Class /Script/Solarland.WeaponSystemPlayerBase
    DWORD wspPrimarySlotInfo = 0x0120; //FPlayerWeaponSlotInfo
    DWORD wspSecondarySlotInfo = 0x012C; //FPlayerWeaponSlotInfo
    DWORD wspTertiarySlotInfo = 0x0138; //FPlayerWeaponSlotInfo

    /// Struct /Script/Solarland.PlayerWeaponSlotInfo
    DWORD pwsWeapon = 0x0000; //TWeakObjectPtr<ASolarPlayerWeapon*>

    /// Struct /Script/Solarland.PlayerWeaponEquipStatusInfo
    DWORD playerWeaponEquipStatusInfo = 0x016C; //PlayerWeaponEquipStatusInfo


    /// Class /Script/Solarland.SolarPlayerWeapon
    DWORD spwShootStatus = 0x0A2C;
    DWORD spwDetectedEnemy = 0x066C;
    DWORD spwWeaponConfig = 0x0470;
}