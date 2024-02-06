#include <string>
#include <cstdint>
#include <cmath>
#include <d3d9.h>
#include <DirectXMath.h>
#include "unreal.h"
#include "../cheat/solar_enum.h"
#include "../utils/strings.h"
#include "utils.h"

namespace local_time {
    static uint64_t milliseconds() {
        return std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    static uint64_t microseconds() {

        return std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    static uint64_t nanoseconds() {
        std::chrono::duration_cast<std::chrono::nanoseconds>
                (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
}
template<class TEnum>
class TEnumAsByte {
public:
    inline TEnumAsByte() {
    }

    inline TEnumAsByte(TEnum _value)
            : value(static_cast<uint8_t>(_value)) {
    }

    explicit inline TEnumAsByte(int32_t _value)
            : value(static_cast<uint8_t>(_value)) {
    }

    explicit inline TEnumAsByte(uint8_t _value)
            : value(_value) {
    }

    inline explicit operator TEnum() const {
        return (TEnum) value;
    }

    inline TEnum getValue() const {
        return (TEnum) value;
    }

private:
    uint8_t value;
};




struct FVector {
    float x;
    float y;
    float z;

    inline FVector()
            : x(0), y(0), z(0) {}

    inline FVector(float x, float y, float z)
            : x(x),
              y(y),
              z(z) {}

    static FVector forward(const FVector &angles) {
        float sp, sy, cp, cy;
        float angle;
        angle = (float) (angles.x * (M_PI / 180.0f));
        sy = sinf(angle);
        cy = cosf(angle);
        angle = (float) (-angles.x * (M_PI / 180.0f));
        sp = sinf(angle);
        cp = cosf(angle);
        return {cp * cy, cp * sy, -sp};
    }

    [[nodiscard]] inline float dot(FVector v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    [[nodiscard]] inline float distance(FVector v) const {
        float xV = this->x - v.x;
        float yV = this->y - v.y;
        float zV = this->z - v.z;
        return sqrtf((xV * xV) + (yV * yV) + (zV * zV));// * 0.0135f;
    }

    [[nodiscard]] inline float distanceIgnoreZ(FVector v) const {
        float xV = this->x - v.x;
        float yV = this->y - v.y;
        return sqrtf((xV * xV) + (yV * yV));
    }

    [[nodiscard]] inline float distanceMeters(FVector v) const {
        return distance(v) * 0.0135f;
    }


    [[nodiscard]] inline float distance2D(FVector v) const {
        float xV = this->x - v.x;
        float yV = this->y - v.y;
        return sqrtf((xV * xV) + (yV * yV));
    }

    [[nodiscard]] inline float angle(FVector v) const {
        double xDistance = x - v.x;
        double yDistance = y - v.y;
        auto angle = (float) (atan(yDistance / xDistance) * (180.0 / M_PI));
        float rotation = xDistance > 0 ? angle - 180 : angle;
        if (abs(angle) == 90)
            return -angle;
        return rotation;
    }

    [[nodiscard]] FVector direction(float angle, float length) const {
        double theta = angle * (M_PI / 180);
        auto dx = (float) (x + cos(theta) * length);
        auto dy = (float) (y + sin(theta) * length);
        return {dx, dy, 0};
    }

    FVector operator+(FVector v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    FVector operator-(FVector v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    FVector operator*(float number) const {
        return {x * number, y * number, z * number};
    }

    FVector operator/(float number) const {
        return {x / number, y / number, z / number};
    }

    [[nodiscard]] std::string debug() const {
        return "{x=" + std::to_string(static_cast<int>(x)) + ", y=" + std::to_string(static_cast<int>(y)) + ", z=" + std::to_string(static_cast<int>(z)) + "}";
    }

    [[nodiscard]] std::string debug(float division) const {
        return "{x=" + std::to_string(static_cast<int>(x / division)) + ", y=" + std::to_string(static_cast<int>(y / division)) + ", z=" + std::to_string(static_cast<int>(z / division)) + "}";
    }
};

struct FRotator {
    float x;
    float y;
    float z;

    [[nodiscard]] std::string debug() const {
        return "{x=" + std::to_string(static_cast<int>(x)) + ", y=" + std::to_string(static_cast<int>(y)) + ", z=" + std::to_string(static_cast<int>(z)) + "}";
    }
};

struct FQuat {
    float x;
    float y;
    float z;
    float w;

    [[nodiscard]] std::string debug() const {
        return "{x=" + std::to_string(static_cast<int>(x)) + ", y=" + std::to_string(static_cast<int>(y)) + ", z=" + std::to_string(static_cast<int>(z)) + +", w=" + std::to_string(static_cast<int>(w)) + "}";
    }
};


struct FTransform {
    FQuat rotation;
    FVector translation;
    char pad[4];
    FVector scale;
    char pad1[4];

    [[nodiscard]] D3DMATRIX toMatrix() const {
        D3DMATRIX m;
        m._41 = translation.x;
        m._42 = translation.y;
        m._43 = translation.z;

        float x2 = rotation.x + rotation.x;
        float y2 = rotation.y + rotation.y;
        float z2 = rotation.z + rotation.z;

        float xx2 = rotation.x * x2;
        float yy2 = rotation.y * y2;
        float zz2 = rotation.z * z2;
        m._11 = (1.0f - (yy2 + zz2)) * scale.x;
        m._22 = (1.0f - (xx2 + zz2)) * scale.y;
        m._33 = (1.0f - (xx2 + yy2)) * scale.z;

        float yz2 = rotation.y * z2;
        float wx2 = rotation.w * x2;
        m._32 = (yz2 - wx2) * scale.z;
        m._23 = (yz2 + wx2) * scale.y;

        float xy2 = rotation.x * y2;
        float wz2 = rotation.w * z2;
        m._21 = (xy2 - wz2) * scale.y;
        m._12 = (xy2 + wz2) * scale.x;

        float xz2 = rotation.x * z2;
        float wy2 = rotation.w * y2;
        m._31 = (xz2 + wy2) * scale.z;
        m._13 = (xz2 - wy2) * scale.x;

        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }
};

D3DMATRIX matrix(FRotator rot, FVector origin) {
    float radPitch = (rot.x * float(M_PI) / 180.f);
    float radYaw = (rot.y * float(M_PI) / 180.f);
    float radRoll = (rot.z * float(M_PI) / 180.f);
    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);
    D3DMATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;
    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;
    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;
    matrix.m[3][0] = origin.x;
    matrix.m[3][1] = origin.y;
    matrix.m[3][2] = origin.z;
    matrix.m[3][3] = 1.f;
    return matrix;
}

D3DMATRIX matrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2) {
    D3DMATRIX pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
    return pOut;
}


struct FFastArraySerializerItem {
    int32_t ReplicationID;                                              // 0x0000   (0x0004)
    int32_t ReplicationKey;                                             // 0x0004   (0x0004)
    int32_t MostRecentArrayReplicationKey;                              // 0x0008   (0x0004)
};

struct FSolarItemData : FFastArraySerializerItem {
    int32_t ItemID;                                                     // 0x000C   (0x0004)
    int64_t ThisID;                                                     // 0x0010   (0x0008)
    FString Name;                                                       // 0x0018   (0x0010)
    FString Icon;                                                       // 0x0028   (0x0010)
    FString Info;                                                       // 0x0038   (0x0010)
    int32_t count;                                                      // 0x0048   (0x0004)
    uint32_t ItemType;                                                  // 0x004C   (0x0004)
    int32_t Quality;                                                    // 0x0050   (0x0004)
    int32_t MaxPile;                                                    // 0x0054   (0x0004)
    bool IfReplace;                                                     // 0x0058   (0x0001)
    unsigned char UnknownData00_5[0x7];                                 // 0x0059   (0x0007)  MISSED
    FString AbilityBP;                                                  // 0x0060   (0x0010)
    FString ItemBP;                                                     // 0x0070   (0x0010)
    int32_t DropTime;                                                   // 0x0080   (0x0004)
    int32_t UnPickupTime;                                               // 0x0084   (0x0004)
    bool IsRecommend;                                                   // 0x0088   (0x0001)
    bool bDirty;                                                        // 0x0089   (0x0001)
    bool IsPickupLock;                                                  // 0x008A   (0x0001)
    unsigned char UnknownData01_5[0x5];                                 // 0x008B   (0x0005)  MISSED
    int64_t UserId;                                                     // 0x0090   (0x0008)
    int32_t Level;                                                      // 0x0098   (0x0004)
    int32_t Prize;                                                      // 0x009C   (0x0004)
    float CurValue;                                                     // 0x00A0   (0x0004)
    bool UseOnPickup;                                                   // 0x00A4   (0x0001)
    uint8_t AppearanceType;                                             // 0x00A5   (0x0001)
    bool bUpdateOverlap;                                                // 0x00A6   (0x0001)
    unsigned char UnknownData02_5[0x1];                                 // 0x00A7   (0x0001)  MISSED
    int32_t SourceType;                                                 // 0x00A8   (0x0004)
    unsigned char UnknownData03_5[0x4];                                 // 0x00AC   (0x0004)  MISSED
    FString OwnerPlayerID;                                              // 0x00B0   (0x0010)
    unsigned char UnknownData04_6[0x8];                                 // 0x00C0   (0x0008)  MISSED
};


struct FMinimalViewInfo {
    FVector location;
    FRotator rotation;
    float FOV;
    float desiredFOV;
    float orthoWidth;
    float orthoNearClipPlane;
    float orthoFarClipPlane;
    float aspectRatio;
};


struct FCameraCacheEntry {
    float timestamp;
    unsigned char UnknownData00_5[0xC];
    FMinimalViewInfo minimalViewInfo;
};


std::string nameFromId(int id) {
    auto chunk = (UINT) ((int) (id) >> 16);
    auto name = (USHORT) id;
    auto poolChunk = read<UINT64>(baseId + offsets::gNames + ((chunk + 2) * 8));
    auto entryOffset = poolChunk + (ULONG) (2 * name);
    auto nameEntry = read<INT16>(entryOffset);
    auto nameLength = nameEntry >> 6;
    char buff[1028];
    if ((DWORD) nameLength && nameLength > 0) {
        read_array(entryOffset + 2, buff, nameLength);
        buff[nameLength] = '\0';
        return {buff};
    } else return "";
}


struct WeakPtr {
    static DWORD_PTR get(DWORD address) {
        auto gObjects = read<DWORD_PTR>(baseId + offsets::gObject);
        auto chunk = read<DWORD_PTR>(gObjects + 0x8 * (address / 0x10000));
        return read<DWORD_PTR>(chunk + 0x18 * (address % 0x10000));
    }
};








//struct FBoneNode {
//    FName name;
//    int32_t parentIndex;
//    TEnumAsByte<EBoneTranslationRetargetingMode> translationRetargetingMode;
//    unsigned char UnknownData00_6[0x3];
//};
//
//struct FVirtualBone {
//    FName sourceBoneName;
//    FName targetBoneName;
//    FName virtualBoneName;
//};
//
//struct FBoxSphereBounds {
//    FVector origin;
//    FVector boxExtent;
//    float sphereRadius;
//};

struct FWeaponMechanicalState {
    EWeaponMechanicalUniqueState uniqueState;
    unsigned char UnknownData00_5[0x3];
    unsigned char UnknownData01_5[0x4];
    unsigned char UnknownData02_5[0x8];
    unsigned char UnknownData03_5[0x8];
    unsigned char UnknownData04_5[0x8];
    unsigned char UnknownData05_6[0x38];
};

//struct FSolarWeaponFireSocketData {
//    TArray<FName> multiBulletSocketNames;                                     // 0x0000   (0x0010)
//    TArray<FName> muzzleSocketNames;                                          // 0x0010   (0x0010)
//    TArray<FName> adsMuzzleSocketNames;                                       // 0x0020   (0x0010)
//    TArray<FName> muzzleBlockCheckSocketNames;                                // 0x0030   (0x0010)
//    TArray<FName> multiCastShellSocketNames;                                  // 0x0040   (0x0010)
//    TArray<FName> adsMultiCastShellSocketNames;                               // 0x0050   (0x0010)
//};

struct FHitResult {
    bool blockingHit: 1;                                           // 0x0000:0 (0x0001)
    bool startPenetrating: 1;                                      // 0x0000:1 (0x0001)
    unsigned char UnknownData00_4[0x3];                                       // 0x0001   (0x0003)  MISSED
    int32_t faceIndex;                                                  // 0x0004   (0x0004)
    float time;                                                       // 0x0008   (0x0004)
    float distance;                                                   // 0x000C   (0x0004)
    FVector location;                                                   // 0x0010   (0x000C)
    FVector impactPoint;                                                // 0x001C   (0x000C)
    FVector normal;                                                     // 0x0028   (0x000C)
    FVector impactNormal;                                               // 0x0034   (0x000C)
    FVector traceStart;                                                 // 0x0040   (0x000C)
    FVector traceEnd;                                                   // 0x004C   (0x000C)
    float penetrationDepth;                                           // 0x0058   (0x0004)
    int32_t item;                                                       // 0x005C   (0x0004)
    //FName boneName;                                                   // 0x0078   (0x0008)
    //FName myBoneName;                                                 // 0x0080   (0x0008)
};

struct FPlayerWeaponShootStatus {
    bool isShooting: 1;
    char startShootTimes;
    char fireTimesOfShoot;
};

struct FBattleCharacterData {
    int32_t CharacterId;                                                // 0x0000   (0x0004)
    int32_t SkinId;                                                     // 0x0004   (0x0004)
    int32_t CharacterLevel;                                             // 0x0008   (0x0004)
    int32_t TalentID;                                                   // 0x000C   (0x0004)
    int32_t UseTimes;                                                   // 0x0010   (0x0004)
    int32_t CardPose;                                                   // 0x0014   (0x0004)
    int32_t Cardbackground;                                             // 0x0018   (0x0004)
    int32_t CharacterExp;                                               // 0x001C   (0x0004)
    int32_t Title;                                                      // 0x0020   (0x0004)
    unsigned char UnknownData00_5[0x4];                                       // 0x0024   (0x0004)  MISSED
    int64_t Expired;                                                    // 0x0028   (0x0008)
    int32_t CombatScore;                                                // 0x0030   (0x0004)
    int32_t BagId;                                                      // 0x0034   (0x0004)
    int32_t TailFlameId;                                                // 0x0038   (0x0004)
    int32_t CapsuleActorId;                                             // 0x003C   (0x0004)
    int32_t DeathBoxId;                                                 // 0x0040   (0x0004)
    int32_t CharacterOwnerType;                                         // 0x0044   (0x0004)
    unsigned char Voice[0x10];                                                      // 0x0048   (0x0010)
    int32_t KillReport;                                                 // 0x0058   (0x0004)
    unsigned char UnknownData01_6[0x4];                                       // 0x005C   (0x0004)  MISSED
};

struct FLastBattleInfo {
    float MvpScore;                                                   // 0x0000   (0x0004)
    int32_t KillNum;                                                    // 0x0004   (0x0004)
    int32_t HitDown;                                                    // 0x0008   (0x0004)
    int32_t AssistNum;                                                  // 0x000C   (0x0004)
    int32_t RescueNum;                                                  // 0x0010   (0x0004)
    int32_t AliveTime;                                                  // 0x0014   (0x0004)
    int32_t CauseDamage;                                                // 0x0018   (0x0004)
};
struct FShieldInfo {
    int32_t shieldID;                                                   // 0x0000   (0x0004)
    float curValue;                                                   // 0x0004   (0x0004)
};



//
//class UObject {
//public:
//    FName namePrivate;
//    DWORD_PTR address;
//public:
//    explicit UObject(DWORD_PTR address) : address(address) {
//        namePrivate = read<FName>(address + offsets::oNamePrivate);
//    }
//};
//
//
//class UCapsuleComponent : public UObject {
//public:
//    float capsuleHalfHeight = read<float>(address + 0x0590);
//    float capsuleRadius = read<float>(address + 0x0594);
//
//    explicit UCapsuleComponent(DWORD_PTR address) : UObject(address) {
//
//    }
//};
//
//class USkeleton : public UObject {
//public:
//    TArray<FBoneNode> boneTree;
//    TArray<FVirtualBone> virtualBones;
//
//    explicit USkeleton(DWORD_PTR address) : UObject(address) {
//        boneTree = read<TArray<FBoneNode>>(address + 0x0038);
//        virtualBones = read<TArray<FVirtualBone>>(address + 0x0180);
//    }
//};
//
//class USkeletalMesh : public UObject {
//public:
//    USkeleton skeleton = USkeleton(read<DWORD_PTR>(address + 0x0060));
//    FBoxSphereBounds importedBounds = read<FBoxSphereBounds>(address + 0x0068);
//    FBoxSphereBounds extendedBounds = read<FBoxSphereBounds>(address + 0x0084);
//
//    explicit USkeletalMesh(DWORD_PTR address) : UObject(address) {
//
//    }
//};
//
//
//class USkinnedMeshComponent : public UObject {
//public:
//    USkeletalMesh skeletalMesh = USkeletalMesh(read<DWORD_PTR>(address + 0x05B0));
//    bool overrideMinLod = driver::readBoolean(address + 0x0726, 4);
//    bool useBoundsFromMasterPoseComponent = driver::readBoolean(address + 0x0726, 8);
//    bool forceWireframe = driver::readBoolean(address + 0x0726, 16);
//    bool displayBones = driver::readBoolean(address + 0x0726, 32);
//    bool disableMorphTarget = driver::readBoolean(address + 0x0726, 64);
//    bool hideSkin = driver::readBoolean(address + 0x0726, 128);
//    bool recentlyRendered = driver::readBoolean(address + 0x0727, 64);
//
//    explicit USkinnedMeshComponent(DWORD_PTR address) : UObject(address) {
//    }
//};
//
//class USkeletalMeshComponent : public USkinnedMeshComponent {
//public:
//    FVector rootBoneTranslation = read<FVector>(address + 0x0808);
//    FVector lineCheckBoundsScale = read<FVector>(address + 0x0814);
//
//    explicit USkeletalMeshComponent(DWORD_PTR address) : USkinnedMeshComponent(address) {
//    }
//};
//
//class UCharacterMovementComponent : public UObject {
//public:
//    float gravityScale = read<float>(address + 0x0150);
//    float maxStepHeight = read<float>(address + 0x0154);
//    float jumpZVelocity = read<float>(address + 0x0158);
//    float jumpOffJumpZFactor = read<float>(address + 0x015C);
//    float walkableFloorAngle = read<float>(address + 0x0160);
//    float walkableFloorZ = read<float>(address + 0x0164);
//    TEnumAsByte<EMovementMode> movementMode = read<TEnumAsByte<EMovementMode>>(address + 0x0168);
//    float groundFriction = read<float>(address + 0x016C);
//    float maxWalkSpeed = read<float>(address + 0x018C);
//    float maxWalkSpeedCrouched = read<float>(address + 0x0190);
//    float maxSwimSpeed = read<float>(address + 0x0194);
//    float maxFlySpeed = read<float>(address + 0x0198);
//    float maxCustomMovementSpeed = read<float>(address + 0x019C);
//    float maxAcceleration = read<float>(address + 0x01A0);
//    float minAnalogWalkSpeed = read<float>(address + 0x01A4);
//    float brakingFrictionFactor = read<float>(address + 0x01A8);
//    float brakingFriction = read<float>(address + 0x01AC);
//    float brakingSubStepTime = read<float>(address + 0x01B0);
//    float brakingDecelerationWalking = read<float>(address + 0x01B4);
//    float brakingDecelerationFalling = read<float>(address + 0x01B8);
//    float brakingDecelerationSwimming = read<float>(address + 0x01BC);
//    float brakingDecelerationFlying = read<float>(address + 0x01C0);
//    float airControl = read<float>(address + 0x01C4);
//    float airControlBoostMultiplier = read<float>(address + 0x01C8);
//    float airControlBoostVelocityThreshold = read<float>(address + 0x01CC);
//    float fallingLateralFriction = read<float>(address + 0x01D0);
//    float crouchedHalfHeight = read<float>(address + 0x01D4);
//    float buoyancy = read<float>(address + 0x01D8);
//    float perchRadiusThreshold = read<float>(address + 0x01DC);
//    float perchAdditionalHeight = read<float>(address + 0x01E0);
//    FRotator rotationRate = read<FRotator>(address + 0x01E4);
//    float maxOutOfWaterStepHeight = read<float>(address + 0x0200);
//    float outOfWaterZ = read<float>(address + 0x0204);
//    float mass = read<float>(address + 0x0208);
//    float standingDownwardForceScale = read<float>(address + 0x020C);
//    float initialPushForceFactor = read<float>(address + 0x0210);
//    float pushForceFactor = read<float>(address + 0x0214);
//    float pushForcePointZOffsetFactor = read<float>(address + 0x0218);
//    float touchForceFactor = read<float>(address + 0x021C);
//    float minTouchForce = read<float>(address + 0x0220);
//    float maxTouchForce = read<float>(address + 0x0224);
//    float repulsionForce = read<float>(address + 0x0228);
//    FVector acceleration = read<FVector>(address + 0x022C);
//    FQuat lastUpdateRotation = read<FQuat>(address + 0x0240);
//    FVector lastUpdateLocation = read<FVector>(address + 0x0250);
//    FVector lastUpdateVelocity = read<FVector>(address + 0x025C);
//    float serverLastTransformUpdateTimeStamp = read<float>(address + 0x0268);
//    float serverLastClientGoodMoveAckTime = read<float>(address + 0x026C);
//    float serverLastClientAdjustmentTime = read<float>(address + 0x0270);
//    FVector pendingImpulseToApply = read<FVector>(address + 0x0274);
//    FVector pendingForceToApply = read<FVector>(address + 0x0280);
//
//    explicit UCharacterMovementComponent(DWORD_PTR address) : UObject(address) {
//
//    }
//
//    explicit UCharacterMovementComponent() : UObject(0) {}
//};
//
//class USceneComponent : public UObject {
//public:
//    FVector relativeLocation;
//    FRotator relativeRotation;
//    FVector relativeScale3D;
//    FVector componentVelocity;
//
//    explicit USceneComponent(DWORD_PTR address) : UObject(address) {
//        relativeLocation = read<FVector>(address + offsets::scRelativeLocation);
//        relativeRotation = read<FRotator>(address + offsets::scRelativeRotation);
//        relativeScale3D = read<FVector>(address + offsets::scRelativeScale3D);
//        componentVelocity = read<FVector>(address + offsets::scComponentVelocity);
//    }
//};
//
//
//class AActor : public UObject {
//public:
//    USceneComponent rootComponent;
//
//    explicit AActor(DWORD_PTR address) : UObject(address), rootComponent(USceneComponent(read<DWORD_PTR>(address + offsets::pRootComponent))) {
//
//    }
//};
//
//class AController : public AActor {
//public:
//    FRotator controlRotation;
//
//    explicit AController(DWORD_PTR address) : AActor(address) {
//        controlRotation = read<FRotator>(address + 0x02A8);
//    }
//};
//
//
//class APlayerState : public AActor {
//public:
//    float score;
//    int32_t playerId;
//    char ping;
//    FString savedNetworkAddress;
//    FString playerNamePrivate;
//
//    explicit APlayerState(DWORD_PTR address) : AActor(address) {
//        score = read<float>(address + 0x0228);
//        playerId = read<int32_t>(address + 0x022C);
//        ping = read<char>(address + 0x0230);
//        savedNetworkAddress = read<FString>(address + 0x0250);
//        playerNamePrivate = read<FString>(address + 0x0310);
//    }
//
//    explicit APlayerState() : AActor(0) {}
//};
//
//class USolarTeamInfoComponent : public UObject {
//public:
//    int32_t KillDownCount = read<int32_t>(address + 0x00B0);
//    int32_t AssistCount = read<int32_t>(address + 0x00B4);
//    int32_t DeathCount = read<int32_t>(address + 0x00B8);
//    int32_t SaveCount = read<int32_t>(address + 0x00BC);
//    explicit USolarTeamInfoComponent(DWORD_PTR address) : UObject(address) {
//
//    }
//    explicit USolarTeamInfoComponent() : UObject(0) {}
//};
//
//class ASolarTeamInfo : public UObject {
//public:
//    char TeamID = read<char>(address +0x0240 );
//    char TeamRank = read<char>(address +0x0241 );
//    char KillCount = read<char>(address +0x0242 );
//    char MemberLimit = read<char>(address +0x0243  );
//    //FText TeamName = read<float>(address +0x0248  );
//    char MatchCount = read<char>(address +0x0260  );
//    TArray<DWORD_PTR> BotMembers = read<TArray<DWORD_PTR>>(address +0x0268   );
//    TArray<DWORD_PTR> PlayerMembers = read<TArray<DWORD_PTR>>(address +0x0278  );
//    TArray<DWORD_PTR> TeamMembers = read<TArray<DWORD_PTR>>(address +0x0288 );
//    USolarTeamInfoComponent TeamInfoDetails = USolarTeamInfoComponent(read<DWORD_PTR>(address +0x0298  ));
//    EMatchResult MatchResult = read<EMatchResult>(address +0x02A1  );
//
//    explicit ASolarTeamInfo(DWORD_PTR address) : UObject(address) {
//
//    }
//
//    explicit ASolarTeamInfo() : UObject(0) {}
//};
//
//class ASolarPlayerState : APlayerState {
//public:
//    int32_t Gold = read<int32_t>(address + 0x0380);
//    TArray<int32_t> CostedGolds = read<TArray<int32_t>>(address + 0x0388);
//    TArray<int32_t> ReceivedGolds = read<TArray<int32_t>>(address + 0x0398);
//    int32_t CharacterId = read<int32_t>(address + 0x03A8);
//    int32_t SkinId = read<int32_t>(address + 0x03AC);
//    FString ClanId = read<FString>(address + 0x03B0);
//    FString ClanName = read<FString>(address + 0x03C0);
//    FString ClanShortName = read<FString>(address + 0x03D0);
//    int32_t BagId = read<int32_t>(address + 0x03E0);
//    int32_t CapsuleID = read<int32_t>(address + 0x03E4);
//    FString SolarPlayerID = read<FString>(address + 0x03E8);
//    FString NickName = read<FString>(address + 0x03F8);
//    uint32_t BattleVoiceId = read<uint32_t>(address + 0x0408);
//    int64_t RTCType = read<int64_t>(address + 0x0410);
//    uint64_t UserId = read<uint64_t>(address + 0x0418);
//    char PosInTeam = read<char>(address + 0x0420);
//    int32_t CharacterLevel = read<int32_t>(address + 0x0424);
//    int32_t AvatarID = read<int32_t>(address + 0x0428);
//    FString AvatarUrl = read<FString>(address + 0x0430);
//    int32_t AvatarFrameID = read<int32_t>(address + 0x0440);
//    int32_t EnterType = read<int32_t>(address + 0x0444);
//    int32_t Gender = read<int32_t>(address + 0x0448);
//    int32_t AccountLevel = read<int32_t>(address + 0x044C);
//    int32_t FriendNum = read<int32_t>(address + 0x0450);
//    int32_t RoleID = read<int32_t>(address + 0x0454);
//    FBattleCharacterData CurUsedCharacterData = read<FBattleCharacterData>(address + 0x0498);
//    FLastBattleInfo LastBattleInfo = read<FLastBattleInfo>(address + 0x0518);
//    TArray<FString> ReCombatLimitPlayer = read<TArray<FString>>(address + 0x0538);
//    int32_t RoleUltimateSkillCDState = read<int32_t>(address + 0x0548);
//    uint32_t PlayerNumber = read<uint32_t>(address + 0x0580);
//    float CurrentHealth = read<float>(address + 0x05D8);
//    float MaxHealth = read<float>(address + 0x05DC);
//    FShieldInfo CurShieldInfo = read<FShieldInfo>(address + 0x05E0);
//    float TemporaryShieldValue = read<float>(address + 0x05E8);
//    char ReviveItemNum = read<char>(address + 0x05EC);
//    char ReviveCountByItem = read<char>(address + 0x05ED);
//    int32_t TalentID = read<int32_t>(address + 0x05F0);
//    int32_t LikeValue = read<int32_t>(address + 0x05F4);
//    int32_t FriendWatchLikeValue = read<int32_t>(address + 0x05F8);
//    int32_t WatchingFriendCount = read<int32_t>(address + 0x05FC);
//    TArray<FString> WatchingFriendPlayerIDs = read<TArray<FString>>(address + 0x0600);
//    int32_t SpectatorNum = read<int32_t>(address + 0x0620);
//    ETalentState TalentState = read<ETalentState>(address + 0x0624);
//    FVector SelfLastVehicleLocation = read<FVector>(address + 0x0630);
//    FVector TeamLastVehicleLocation = read<FVector>(address + 0x063C);
//    char CurrentPlayerStateInGame = read<char>(address + 0x0648);
//    char CurrentCharacterStateInGame = read<char>(address + 0x0649);
//    int32_t AccountFlag = read<int32_t>(address + 0x064C);
//    int32_t ClanFlag = read<int32_t>(address + 0x0650);
//    FString OS = read<FString>(address + 0x06B8);
//    FString OsType = read<FString>(address + 0x06C8);
//    FString DeviceModel = read<FString>(address + 0x06D8);
//    ECommonInputType InputType = read<ECommonInputType>(address + 0x06E8);
//    bool bUsedGamePad = read<bool>(address + 0x06E9);
//    bool bIsSameTeamWithLocalPlayer = read<bool>(address + 0x06EA);
//    bool HasReconnection = read<bool>(address + 0x06EB);
//    bool bHasHangUpBehavior = read<bool>(address + 0x06F0);
//    float HangUpTime = read<float>(address + 0x06F4);
//    float OfflineBattleTime = read<float>(address + 0x0708);
//    int32_t Elo = read<int32_t>(address + 0x070C);
//    int32_t RankLevel = read<int32_t>(address + 0x0710);
//    int32_t RankLevel2 = read<int32_t>(address + 0x0714);
//    int32_t LegendRank = read<int32_t>(address + 0x0718);
//    float StartWaitTime = read<float>(address + 0x071C);
//    float ReportedWaitTime = read<float>(address + 0x0720);
//    char VipType = read<char>(address + 0x0724);
//    int32_t WarmupTag = read<int32_t>(address + 0x0728);
//    float Frustration = read<float>(address + 0x072C);
//    bool bMonitorScreenshot = read<bool>(address + 0x0730);
//    bool bMonitorEigenvalue = read<bool>(address + 0x0731);
//    bool bMonitorReplay = read<bool>(address + 0x0732);
//    char LobbyTeamMemberNum = read<char>(address + 0x0733);
//    bool bTeamIDHasChanged = read<bool>(address + 0x0734);
//    int32_t RepliableMessageID = read<int32_t>(address + 0x0738);
//    float RepliableMessageStartTime = read<float>(address + 0x073C);
//    bool bIsOBPlayer = read<bool>(address + 0x074C);
//    int32_t WarmId = read<int32_t>(address + 0x0750);
//    int32_t WarmType = read<int32_t>(address + 0x0754);
//    bool AllowOtherJoin = read<bool>(address + 0x0758);
//    FString SocialTeamID = read<FString>(address + 0x0760);
//    int32_t GroupNonFriendNum = read<int32_t>(address + 0x0774);
//    int32_t BattleFightNum = read<int32_t>(address + 0x0778);
//    float WaitTimeBeforeEnterSpectateMode = read<float>(address + 0x077C);
//    EPlayerNetStateInGame NetStateInGame = read<EPlayerNetStateInGame>(address + 0x0800);
//    bool bHasLogin = read<bool>(address + 0x0801);
//    bool bHasUpdateByServerData = read<bool>(address + 0x0810);
//    int32_t KillNum = read<int32_t>(address + 0x0820);
//    int32_t AssistNum = read<int32_t>(address + 0x0840);
//    FVector DeathPos = read<FVector>(address + 0x0860);
//    int32_t ReconnectNum = read<int32_t>(address + 0x086C);
//    int32_t StrongholdID = read<int32_t>(address + 0x087C);
//    int32_t SkyDivingStrongholdID = read<int32_t>(address + 0x0880);
//    float MultiKillDurationTime = read<float>(address + 0x0888);
//    float TotalReceivedDamage = read<float>(address + 0x08A0);
//    float HealSelf = read<float>(address + 0x08F8);
//    float ReceivedHeal = read<float>(address + 0x08FC);
//    float LifeTime = read<float>(address + 0x0900);
//    int32_t PlayerRank = read<int32_t>(address + 0x09E0);
//    bool bMVP = read<bool>(address + 0x09E4);
//    float MvpScore = read<float>(address + 0x09E8);
//    float KDA = read<float>(address + 0x09EC);
//    int32_t ResurrectionTimes = read<int32_t>(address + 0x09F0);
//    int32_t GainCombatScore = read<int32_t>(address + 0x0A08);
//    int32_t AccountExp = read<int32_t>(address + 0x0A0C);
//    int32_t Zomborg = read<int32_t>(address + 0x0A60);
//    int32_t ZomborgExtra = read<int32_t>(address + 0x0A64);
//    bool bPresettling = read<bool>(address + 0x0B00);
//    int32_t InteractValue = read<int32_t>(address + 0x0B04);
//    float LastInteractTime = read<float>(address + 0x0B08);
//    float HistoryPerformanceFactor = read<float>(address + 0x0B0C);
//    float VehicleProbability = read<float>(address + 0x0B10);
//    bool bIsTimeoutPlayer = read<bool>(address + 0x0B14);
//    bool bShowBattlePrompt: 1 = read<bool>(address + 0x0B15);
//    FVector PlayerLocation = read<FVector>(address + 0x0BA0);
//    int32_t SkydivingLeaderPosInTeam = read<int32_t>(address + 0x0BC0);
//    char SkydivingFollowFormationPos = read<char>(address + 0x0BC4);
//    ESkydivingState SkydivingState = read<ESkydivingState>(address + 0x0BCC);
//    int32_t PlayerLevel = read<int32_t>(address + 0x0C08);
//    int32_t PlayerExperience = read<int32_t>(address + 0x0C0C);
//    int32_t HistoryShieldUpgradeItemNum = read<int32_t>(address + 0x0C10);
//    float TrackMaxRevealDistance = read<float>(address + 0x0C38);
//    int32_t TailFlameId = read<int32_t>(address + 0x0C40);
//    ASolarTeamInfo Team = ASolarTeamInfo(read<DWORD_PTR>(address + 0x0C48));
//
//    explicit ASolarPlayerState(DWORD_PTR address) : APlayerState(address) {}
//
//    ASolarPlayerState() {
//
//    }
//};
//
//class APawn : public AActor {
//public:
//    float baseEyeHeight;
//    APlayerState playerState;
//    AController controller;
//
//    explicit APawn(DWORD_PTR address) :
//            AActor(address),
//            playerState(APlayerState(read<DWORD_PTR>(address + offsets::pPlayerState))),
//            controller(AController(read<DWORD_PTR>(address + 0x0260))) {
//    }
//
//    explicit APawn() : AActor(0), playerState(0), controller(0) {}
//};
//
//class APlayerCameraManager : public AActor {
//public:
//    USceneComponent transformComponent;
//    float defaultFOV;
//    FCameraCacheEntry cameraCache;
//    FCameraCacheEntry lastFrameCameraCache;
//    FCameraCacheEntry cameraCachePrivate;
//    FCameraCacheEntry lastFrameCameraCachePrivate;
//    float freeCamDistance;
//    FVector freeCamOffset;
//    FVector viewTargetOffset;
//
//    explicit APlayerCameraManager(DWORD_PTR address) : AActor(address), transformComponent(USceneComponent(read<DWORD_PTR>(address + offsets::pcmTransformComponent))) {
//        defaultFOV = read<float>(address + 0x0240);
//        cameraCache = read<FCameraCacheEntry>(address + offsets::pcmCameraCache);
//        lastFrameCameraCache = read<FCameraCacheEntry>(address + 0x0900);
//        cameraCachePrivate = read<FCameraCacheEntry>(address + offsets::pcmCameraCachePrivate);
//        lastFrameCameraCachePrivate = read<FCameraCacheEntry>(address + 0x22D0);
//        freeCamDistance = read<float>(address + 0x2950);
//        freeCamOffset = read<FVector>(address + 0x2954);
//        viewTargetOffset = read<FVector>(address + 0x2960);
//    }
//};
//
//class APlayerController : public AController {
//public:
//    APawn acknowledgedPawn;
//    APlayerCameraManager cameraManager;
//    FRotator targetViewRotation;
//    bool isLocalPlayerController;
//    FVector spawnLocation;
//
//    explicit APlayerController(DWORD_PTR address) : AController(address), acknowledgedPawn(APawn(read<DWORD_PTR>(address + offsets::pcAcknowledgedPawn))), cameraManager(APlayerCameraManager(read<DWORD_PTR>(address + offsets::pcPlayerCameraManager))) {
//        targetViewRotation = read<FRotator>(address + offsets::pcTargetViewRotation);
//        isLocalPlayerController = read<bool>(address + 0x058C);
//        spawnLocation = read<FVector>(address + 0x0590);
//    }
//
//    explicit APlayerController() : AController(0), acknowledgedPawn(0), cameraManager(0) {}
//};
//
//class ACharacter : public APawn {
//public:
//    USkeletalMeshComponent mesh = USkeletalMeshComponent(address + 0x0288);
//    UCharacterMovementComponent characterMovement = UCharacterMovementComponent(read<DWORD_PTR>(address + 0x0290));
//    UCapsuleComponent capsuleComponent = UCapsuleComponent(read<DWORD_PTR>(address + 0x0298));
//    FVector baseTranslationOffset = read<FVector>(address + 0x0304);
//    FQuat baseRotationOffset = read<FQuat>(address + 0x0310);
//    bool isCrouched = driver::readBoolean(address + 0x0330, 1);
//    bool isFalling = driver::readBoolean(address + 0x0330, 16);
//    bool isJumping = driver::readBoolean(address + 0x0331, 1);
//
//    explicit ACharacter(DWORD_PTR address) : APawn(address) {
//
//    }
//
//    explicit ACharacter() : APawn() {
//
//    }
//
//
//};
//
//class UAmmoConfig : public UObject {
//public:
//    float chargingToleranceEndTime = read<float>(address + 0x0058);
//    float chargingWorkingEndTime = read<float>(address + 0x005C);
//    float chargingHoldingEndTime = read<float>(address + 0x0060);
//    float chargingTrajectoryTime = read<float>(address + 0x0064);
//    bool costToChargeScale = read<bool>(address + 0x0068);
//    int32_t fireCostFullShot = read<int32_t>(address + 0x006C);
//    bool forceFullShot = read<bool>(address + 0x0070);
//    float maxChargingDamageScale = read<float>(address + 0x0074);
//    float deltaAngle = read<float>(address + 0x0078);
//    FString name = read<FString>(address + 0x0080);
//    FString des = read<FString>(address + 0x0090);
//    int32_t propId = read<int32_t>(address + 0x00A0);
//    float lifeTime = read<float>(address + 0x00A4);
//    EFireMethodType fireMethodType = read<EFireMethodType>(address + 0x00A8);
//    ETrajectoryType trajectoryType = read<ETrajectoryType>(address + 0x00A9);
//    float customValue = read<float>(address + 0x00AC);
//    float maxRange = read<float>(address + 0x00F4);
//    float effRange = read<float>(address + 0x00F8);
//    float radius = read<float>(address + 0x00FC);
//    float initSpeed = read<float>(address + 0x0100);
//    int32_t fireCostPerAttack = read<int32_t>(address + 0x0108);
//    int32_t fireSpeedChangeTime = read<int32_t>(address + 0x010C);
//    float fireSpeedChangeCOP = read<float>(address + 0x0110);
//    float fastestFireInterval = read<float>(address + 0x0114);
//    float slowestFireInterval = read<float>(address + 0x0118);
//    float baseFireInterval = read<float>(address + 0x011C);
//    float fireIntervalRevertPreTime = read<float>(address + 0x0120);
//    float fireIntervalRevertSpeed = read<float>(address + 0x0124);
//    float boltActionTime = read<float>(address + 0x0128);
//    float startBoltDuration = read<float>(address + 0x012C);
//    float firePrepareTime = read<float>(address + 0x0130);
//    float fireStateBreakTime = read<float>(address + 0x0134);
//    float fireStreakBreakTime = read<float>(address + 0x0138);
//    float baseReloadTime = read<float>(address + 0x013C);
//    float reloadBoltTime = read<float>(address + 0x0140);
//    float postFireOverload = read<float>(address + 0x0144);
//    bool playHitSound = read<bool>(address + 0x0148);
//    bool playExplodeSound = read<bool>(address + 0x0149);
//    float flySoundReceiveRadius = read<float>(address + 0x014C);
//    float flySoundIgnoreDistance = read<float>(address + 0x0150);
//    int32_t singleSoundCount = read<int32_t>(address + 0x0154);
//    float spreadFirePreTime = read<float>(address + 0x0240);
//    float spreadPostFireSpeed = read<float>(address + 0x0244);
//    float spreadRestorePreTime = read<float>(address + 0x0248);
//    float spreadRestoreSpeed = read<float>(address + 0x024C);
//    float projectileMaxGravity = read<float>(address + 0x0260);
//    float particleStartDistance = read<float>(address + 0x0264);
//    float trajectoryStartDistance = read<float>(address + 0x0268);
//    bool penetrable = read<bool>(address + 0x026C);
//    bool takeDamageAfterDeathVerge = read<bool>(address + 0x026D);
//    EVirtualBulletType virtualBulletType = read<EVirtualBulletType>(address + 0x0280);
//    bool stepOnServer = read<bool>(address + 0x0281);
//    bool useSubStepping = read<bool>(address + 0x0282);
//    float maxSimulationTimeStep = read<float>(address + 0x0284);
//    int32_t maxSimulationIterations = read<int32_t>(address + 0x0288);
//    char traceTargetFlag = read<char>(address + 0x0298);
//    bool thirdPersonViewXScaledByDistance = read<bool>(address + 0x0310);
//    bool forceApplyAbility = read<bool>(address + 0x0311);
//    int32_t descriptionLocalTextIDNumber = read<int32_t>(address + 0x0340);
//    int32_t rangeLocalTextID = read<int32_t>(address + 0x0344);
//    int32_t difficultyLocalTextID = read<int32_t>(address + 0x0348);
//
//    explicit UAmmoConfig(DWORD_PTR address) : UObject(address) {
//
//    }
//};
//
//class USingleWeaponConfig : public UObject {
//public:
//    ESolarWeaponBrand weaponBrand = read<ESolarWeaponBrand>(address + 0x0034);
//    EWeaponType weaponType = read<EWeaponType>(address + 0x0035);
//    bool supportSecondaryFire = read<bool>(address + 0x0036);
//    TArray<FSolarWeaponFireSocketData> fireSockets = read<TArray<FSolarWeaponFireSocketData>>(address + 0x0038);
//    bool isSingleWeapon = read<bool>(address + 0x0048);
//    bool canSuspendBolt = read<bool>(address + 0x0049);
//    bool isHeavyFireWeapon = read<bool>(address + 0x004A);
//    bool isFlamethrowerWeapon = read<bool>(address + 0x004B);
//    bool supportSingleShootMode = read<bool>(address + 0x004C);
//    bool supportAutoShootMode = read<bool>(address + 0x004D);
//    bool supportBurstShootMode = read<bool>(address + 0x004E);
//    float maxSpread = read<float>(address + 0x0050);
//    float minSpread = read<float>(address + 0x0054);
//    float hipFireBaseSpread = read<float>(address + 0x0058);
//    bool calcSpreadByStandardDistance = read<bool>(address + 0x005C);
//    float spreadStandardDistance = read<float>(address + 0x0060);
//    float spreadStandardOriginMaxRange = read<float>(address + 0x0064);
//    bool calcSpreadByDistributionCurve = read<bool>(address + 0x0068);
//    float spreadStatusSpeed = read<float>(address + 0x00F8);
//    UAmmoConfig primaryAmmo = UAmmoConfig(read<DWORD_PTR>(address + 0x0120));
//    int32_t primaryAmmoIndex = read<int32_t>(address + 0x0128);
//    int32_t primaryAmmoCap = read<int32_t>(address + 0x012C);
//    UAmmoConfig upgradedSecAmmo = UAmmoConfig(read<DWORD_PTR>(address + 0x0130));
//    bool needOverload = read<bool>(address + 0x0148);
//    float overloadMaxValue = read<float>(address + 0x014C);
//    float normalOverloadCoolingRate = read<float>(address + 0x0150);
//    float overloadCoolingPeriod = read<float>(address + 0x0160);
//    float overloadWarningRate = read<float>(address + 0x0170);
//    bool canAutoFire = read<bool>(address + 0x0174);
//    float autoFireDelayMs = read<float>(address + 0x0178);
//    float autoFireEndDelayMs = read<float>(address + 0x017C);
//    float autoFireCompensationRate = read<float>(address + 0x0180);
//    bool canAimAssist = read<bool>(address + 0x0190);
//    bool oneKeyScope = read<bool>(address + 0x02D0);
//    float scopeOpenTime = read<float>(address + 0x02D4);
//    float scopeOpenFOVTimeScale = read<float>(address + 0x02D8);
//    TArray<int32_t> defaultPartsArray = read<TArray<int32_t>>(address + 0x02F0);
//    bool isGatling = read<bool>(address + 0x0300);
//    float gatlingRotaryAcceleration = read<float>(address + 0x0304);
//    float gatlingFireRotarySpeed = read<float>(address + 0x0308);
//    float gatlingMaxRotarySpeed = read<float>(address + 0x030C);
//    float gatlingHoldTime = read<float>(address + 0x0310);
//    float gatlingRotaryAttenuation = read<float>(address + 0x0314);
//    float gatlingScatteringMagnification = read<float>(address + 0x0318);
//    float headshotDamageFactor = read<float>(address + 0x0338);
//    int32_t primaryFireTxt = read<int32_t>(address + 0x0470);
//    int32_t secondaryFireTxt = read<int32_t>(address + 0x0560);
//    FString weaponTextType = read<FString>(address + 0x0568);
//
//    explicit USingleWeaponConfig(DWORD_PTR address) : UObject(address) {
//
//    }
//};
//
//class ASolarWeapon : public AActor {
//public:
//    FWeaponMechanicalState WeaponState = read<FWeaponMechanicalState>(address + 0x0308);
//    ESCMDamageType DefaultDamageType = read<ESCMDamageType>(address + 0x037C);
//    float OverloadAccum = read<float>(address + 0x03A0);
//    float FireChargingTime = read<float>(address + 0x03D8);
//    int32_t FireChargingPhase = read<int32_t>(address + 0x03DC);
//    float FireChargingSumTime = read<float>(address + 0x03E0);
//    USingleWeaponConfig Config = USingleWeaponConfig(read<DWORD_PTR>(address + 0x0470));
//    int32_t weaponId = read<int32_t>(address + 0x0480);
//    bool infinitePackageAmmo = read<bool>(address + 0x0494);
//    int32_t extraAmmo = read<int32_t>(address + 0x0498);
//    int32_t clipRemainAmmoCount = read<int32_t>(address + 0x04F0);
//    USkeletalMeshComponent mesh = USkeletalMeshComponent(read<DWORD_PTR>(address + 0x0570));
//    int32_t skillFireCount = read<int32_t>(address + 0x05A0);
//    int32_t singleFireCount = read<int32_t>(address + 0x05A4);
//    int32_t fireOverloadCount = read<int32_t>(address + 0x05A8);
//    int32_t fireLastCount = read<int32_t>(address + 0x05AC);
//    bool detectedEnemy = read<bool>(address + 0x066C);
//    bool tracingAimTarget = read<bool>(address + 0x066D);
//    float lastAddSpreadByFireTime = read<float>(address + 0x06EC);
//    float spreadPrepareTimer = read<float>(address + 0x06F0);
//    float pendingBaseSpread = read<float>(address + 0x06F4);
//    float targetBaseSpread = read<float>(address + 0x06F8);
//    float pendingAddSpread = read<float>(address + 0x06FC);
//    float targetAddSpread = read<float>(address + 0x0700);
//    FHitResult AimHitResult = read<FHitResult>(address + 0x0760);
//
//    explicit ASolarWeapon(DWORD_PTR address) : AActor(address) {
//
//    }
//};
//
//
//class ASolarPlayerWeapon : public ASolarWeapon {
//public:
//    FSolarItemData itemData = read<FSolarItemData>(address + 0x0838);
//    int32_t scopePartID = read<int32_t>(address + 0x0938);
//    int32_t gripPartID = read<int32_t>(address + 0x093C);
//    int32_t stockPartID = read<int32_t>(address + 0x0940);
//    int32_t muzzlePartID = read<int32_t>(address + 0x0944);
//    int32_t clipPartID = read<int32_t>(address + 0x0948);
//    int32_t weaponSkinID = read<int32_t>(address + 0x0A00);
//    int32_t defaultSkinID = read<int32_t>(address + 0x0A04);
//    FPlayerWeaponShootStatus ShootStatus = read<FPlayerWeaponShootStatus>(address + 0x0A2C);
//
//    explicit ASolarPlayerWeapon(DWORD_PTR address) : ASolarWeapon(address) {
//
//    }
//};
//
//class UWeaponSystemPlayerBase : public UObject {
//public:
//    ASolarPlayerWeapon primarySlotInfo = ASolarPlayerWeapon(WeakPtr::get(read<DWORD_PTR>(address + 0x0120)));
//    ASolarPlayerWeapon secondarySlotInfo = ASolarPlayerWeapon(WeakPtr::get(read<DWORD_PTR>(address + 0x012C)));
//    ASolarPlayerWeapon tertiarySlotInfo = ASolarPlayerWeapon(WeakPtr::get(read<DWORD_PTR>(address + 0x0138)));
//
//    explicit UWeaponSystemPlayerBase(DWORD_PTR address) : UObject(address) {
//
//    }
//};
//
//class ASolarCharacterBase : public ACharacter {
//public:
//    explicit ASolarCharacterBase(DWORD_PTR address) : ACharacter(address) {
//
//    }
//};
//
//class ASolarItemActor : public AActor {
//public:
//    FSolarItemData itemData = read<FSolarItemData>(address + 0x0270);
//    FVector droppedLocation = read<FVector>(address + 0x0338);
//
//    explicit ASolarItemActor(DWORD_PTR address) : AActor(address) {
//
//    }
//};
//
//class ASolarTreasureBoxActor : public ASolarItemActor {
//public:
//    ETreasureBoxState currentState = read<ETreasureBoxState>(address + 0x0418);
//    TArray<FSolarItemData> itemDataList = read<TArray<FSolarItemData>>(read<DWORD_PTR>(address + 0x0430) + 0x0108);
//
//    explicit ASolarTreasureBoxActor(DWORD_PTR address) : ASolarItemActor(address) {
//
//    }
//};
//
//class ADeathTreasureBox : public ASolarTreasureBoxActor {
//public:
//    FString playerId = read<FString>(address + 0x0618);
//    FString nickName = read<FString>(address + 0x0628);
//    bool isPickedUp = read<bool>(address + 0x0638);
//    std::vector<ASolarPlayerWeapon> weaponList;
//    float ResurrectDistance = read<float>(address + 0x0664);
//
//    explicit ADeathTreasureBox(DWORD_PTR address) : ASolarTreasureBoxActor(address) {
//        auto weaponListPointer = read<TArray<DWORD_PTR>>(address + 0x0640);
//        for (int i = 0; i < weaponListPointer.length(); ++i) {
//            weaponList.emplace_back(weaponListPointer.getValuePointer(i));
//        }
//    }
//};
//
//class ASolarCharacter : public ASolarCharacterBase {
//public:
//    ESolarCharacterType solarCharacterType = read<ESolarCharacterType>(address + 0x06D0);
//    //FVector2 characterCapsuleSizeOverride = read<FVector2>(address + 0x06D4);
//    bool inRoom = read<bool>(address + 0x06DD);
//    ECharacterBodyScaleType bodyScaleType = read<ECharacterBodyScaleType>(address + 0x08A8);
//    int32_t assignedCharacterID = read<int32_t>(address + 0x08AC);
//    int32_t assignedSkinID = read<int32_t>(address + 0x08B0);
//    TArray<int32_t> weaponPartsArray = read<TArray<int32_t>>(address + 0x0EB0);
//    char energyState = read<char>(address + 0x0EE0);
//    float maxEnergyValue = read<float>(address + 0x0EE4);
//    float currEnergyValue = read<float>(address + 0x0EE8);
//    float maxExtraEnergyValue = read<float>(address + 0x0EEC);
//    float currExtraEnergyValue = read<float>(address + 0x0EF0);
//    int32_t energyModuleID = read<int32_t>(address + 0x0EF4);
//    float addedEnergyMax = read<float>(address + 0x0EF8);
//    int32_t extraEnergyModuleID = read<int32_t>(address + 0x0EFC);
//    float addedExtraEnergyMax = read<float>(address + 0x0F00);
//    float solarChargeRatio = read<float>(address + 0x0F14);
//    //FVector2 inputVector = read<FVector2>(address + 0x0F28);
//    FRotator characterLookAt = read<FRotator>(address + 0x0F48);
//    float pendingRegeneration = read<float>(address + 0x0F98);
//    float diedTime = read<float>(address + 0x0F9C);
//    TEnumAsByte<ERescueState> CurrentRescueState = read<TEnumAsByte<ERescueState>>(address + 0x1038);
//    TEnumAsByte<ERescueState> LastRescueState = read<TEnumAsByte<ERescueState>>(address + 0x1039);
//    float rescuedStartTime = read<float>(address + 0x1070);
//    float rescueTime = read<float>(address + 0x10A4);
//    std::vector<ASolarWeapon> deathBoxWeaponArray;
//    TArray<FSolarItemData> DeathBoxArray = read<TArray<FSolarItemData>>(address + 0x10C0);
//    float DeathBoxEnergy = read<float>(address + 0x10D0);
//    bool bIsElectricStoreOpened = read<bool>(address + 0x1148);
//    int32_t CurrShieldId = read<int32_t>(address + 0x120C);
//    int32_t CurUniqueShieldId = read<int32_t>(address + 0x1210);
//    float MaxShieldValue = read<float>(address + 0x1214);
//    int32_t NextUpgradeShieldID = read<int32_t>(address + 0x1218);
//    int32_t NextUpgradeShieldEnergy = read<int32_t>(address + 0x121C);
//    float CurrShieldValue = read<float>(address + 0x1220);
//    EShieldState CurrShieldState = read<EShieldState>(address + 0x1224);
//    int32_t CurrShieldLevel = read<int32_t>(address + 0x1228);
//    ADeathTreasureBox CurChargingDeathBox = ADeathTreasureBox(read<DWORD_PTR>(address + 0x14D8));
//    std::vector<ADeathTreasureBox> DeathBoxes;
//    float ArmorReduceValue = read<float>(address + 0x1824);
//    float HeadHitValue = read<float>(address + 0x1828);
//    float StandJogSpeed = read<float>(address + 0x19A8);
//    float StandRunSpeed = read<float>(address + 0x19AC);
//    float StandSprintSpeed = read<float>(address + 0x19B0);
//    float CrouchJogSpeed = read<float>(address + 0x19B4);
//    float CrouchRunSpeed = read<float>(address + 0x19B8);
//    float CrouchSprintSpeed = read<float>(address + 0x19BC);
//    bool bIsSprintingMove = read<bool>(address + 0x19D8);
//    UCharacterMovementComponent CachedSolarCharacterMovement = UCharacterMovementComponent(read<DWORD_PTR>(address + 0x1A30));
//    float PrepareToParachuteStartTime = read<float>(address + 0x1AE0);
//    float PrepareToParachuteDuration = read<float>(address + 0x1AE4);
//    FVector PrepareToParachuteLocation = read<FVector>(address + 0x1AE8);
//    FVector AgreedParachuteLocation = read<FVector>(address + 0x1AF4);
//    int32_t CapsuleID = read<int32_t>(address + 0x1B18);
//    ECruiseState CruiseState = read<ECruiseState>(address + 0x1B48);
//    ESkydiveStage CurrentSkydiveStage = read<ESkydiveStage>(address + 0x1C3C);
//    UWeaponSystemPlayerBase WeaponSystemComponent = UWeaponSystemPlayerBase(read<DWORD_PTR>(address + 0x1CC0));
//    //UWeaponSystemVehicleComponent *WeaponSystemVehicle = read<float>(address + 0x1DC0);
//    //FBoardedVehicleInfo BoardedVehicleInfo = read<float>(address + 0x1DE0);
//    bool bActiveEMP = read<bool>(address + 0x1EF8);
//    bool bActiveInfiniteAmmo = read<bool>(address + 0x1EF9);
//    bool bNoticedCantCharge = read<bool>(address + 0x1EFA);
//    EWallRunState WallRunState = read<EWallRunState>(address + 0x2021);
//    EZiplineMoveState CurrentZiplineMoveState = read<EZiplineMoveState>(address + 0x2550);
//
//    explicit ASolarCharacter(DWORD_PTR address) : ASolarCharacterBase(address) {
//        auto deathBoxPointer = read<TArray<DWORD_PTR>>(address + 0x10B0);
//        for (int i = 0; i < deathBoxPointer.length(); ++i) {
//            deathBoxWeaponArray.emplace_back(deathBoxPointer.getValuePointer(i));
//        }
//        auto deathBoxesPointer = read<TArray<DWORD_PTR>>(address + 0x14E0);
//        for (int i = 0; i < deathBoxesPointer.length(); ++i) {
//            DeathBoxes.emplace_back(deathBoxesPointer.getValuePointer(i));
//        }
//    }
//
//    explicit ASolarCharacter() : ASolarCharacterBase(0) {}
//};
//
//class UPlayer : public UObject {
//public:
//    APlayerController playerController = APlayerController(read<DWORD_PTR>(address + 0x0030));
//
//    explicit UPlayer(DWORD_PTR address) : UObject(address) {}
//};