#include <string>
#include <cstdint>
#include <cmath>
#include <d3d9.h>
#include <DirectXMath.h>
#include "../utils/driver.h"


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

template<class T>
class TArray {
public:
    [[nodiscard]] int length() const {
        return dataCount;
    }

    [[nodiscard]] bool isValid() const {
        if (dataCount > dataMax)
            return false;
        if (!memoryData)
            return false;
        return true;
    }

    [[nodiscard]] uint64_t getAddress() const {
        return memoryData;
    }

    T getValue(int position) {
        return read<T>(memoryData + position * 0x8);
    }

    T getObject(int position) {
        return T(getValuePointer(position));
    }


    DWORD_PTR getValuePointer(int position) {
        return read<DWORD_PTR>(memoryData + position * 0x8);
    }


protected:
    uint64_t memoryData;
    uint32_t dataCount;
    uint32_t dataMax;
};

struct FString : private TArray<wchar_t> {

    [[nodiscard]] std::string toString() const {
        auto *buffer = new wchar_t[dataCount];
        read_array(memoryData, buffer, dataCount);
        std::string s = wchar_to_UTF8(buffer);
        delete[] buffer;
        return s;
    }

    static std::string wchar_to_UTF8(const wchar_t *in) {
        std::string out;
        unsigned int codepoint = 0;
        for (in; *in != 0; ++in) {
            if (*in >= 0xd800 && *in <= 0xdbff)
                codepoint = ((*in - 0xd800) << 10) + 0x10000;
            else {
                if (*in >= 0xdc00 && *in <= 0xdfff)
                    codepoint |= *in - 0xdc00;
                else
                    codepoint = *in;
                if (codepoint <= 0x7f)
                    out.append(1, static_cast<char>(codepoint));
                else if (codepoint <= 0x7ff) {
                    out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                    out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                } else if (codepoint <= 0xffff) {
                    out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                    out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                    out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                } else {
                    out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                    out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                    out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                    out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                }
                codepoint = 0;
            }
        }
        return out;
    }
};

typedef uint32_t FNameEntryId;

struct FName {
    FNameEntryId index = 0;
    int32_t number = 0;

    std::string toString(const uintptr_t gameBase) const {
        auto chunk = (UINT) ((int) (index) >> 16);
        auto name = (USHORT) index;
        auto poolChunk = read<UINT64>(gameBase + 0x6E61180 + ((chunk + 2) * 8));
        auto entryOffset = poolChunk + (ULONG) (2 * name);
        auto nameEntry = read<INT16>(entryOffset);
        auto nameLength = nameEntry >> 6;
        char buff[1028];
        if ((DWORD) nameLength && nameLength > 0) {
            read_array(entryOffset + 2, buff, nameLength);
            buff[nameLength] = '\0';
            return std::string(buff);
        } else return "";
    }
};

struct FVector {
    float X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
    float Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
    float Z;

    inline FVector()
            : X(0), Y(0), Z(0) {}

    inline FVector(float x, float y, float z)
            : X(x),
              Y(y),
              Z(z) {}

    static FVector forward(const FVector &angles) {
        float sp, sy, cp, cy;
        float angle;
        angle = (float) (angles.X * (M_PI / 180.0f));
        sy = sinf(angle);
        cy = cosf(angle);
        angle = (float) (-angles.X * (M_PI / 180.0f));
        sp = sinf(angle);
        cp = cosf(angle);
        return {cp * cy, cp * sy, -sp};
    }

    inline float dot(FVector v) const {
        return X * v.X + Y * v.Y + Z * v.Z;
    }

    [[nodiscard]] inline float distance(FVector v) const {
        float xV = this->X - v.X;
        float yV = this->Y - v.Y;
        float zV = this->Z - v.Z;
        return sqrtf((xV * xV) + (yV * yV) + (zV * zV)) * 0.0135f;
    }

    FVector operator+(FVector v) const {
        return {X + v.X, Y + v.Y, Z + v.Z};
    }

    FVector operator-(FVector v) const {
        return {X - v.X, Y - v.Y, Z - v.Z};
    }

    FVector operator*(float number) const {
        return {X * number, Y * number, Z * number};
    }

    std::string debug() {
        return "{x=" + std::to_string(static_cast<int>(X)) + ", y=" + std::to_string(static_cast<int>(Y)) + ", z=" + std::to_string(static_cast<int>(Z)) + "}";
    }
};

struct FRotator {
public:
    explicit FRotator(float xx = 0, float yy = 0, float zz = 0) : x(xx), y(yy), z(zz) {}

    float x, y, z;
};

struct FQuat {
    float x;
    float y;
    float z;
    float w;
};


struct FTransform {
    FQuat rotation;
    FVector translation;
    char pad[4];
    FVector scale;
    char pad1[4];

    D3DMATRIX toMatrix() {
        D3DMATRIX m;
        m._41 = translation.X;
        m._42 = translation.Y;
        m._43 = translation.Z;

        float x2 = rotation.x + rotation.x;
        float y2 = rotation.y + rotation.y;
        float z2 = rotation.z + rotation.z;

        float xx2 = rotation.x * x2;
        float yy2 = rotation.y * y2;
        float zz2 = rotation.z * z2;
        m._11 = (1.0f - (yy2 + zz2)) * scale.X;
        m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
        m._33 = (1.0f - (xx2 + yy2)) * scale.Z;

        float yz2 = rotation.y * z2;
        float wx2 = rotation.w * x2;
        m._32 = (yz2 - wx2) * scale.Z;
        m._23 = (yz2 + wx2) * scale.Y;

        float xy2 = rotation.x * y2;
        float wz2 = rotation.w * z2;
        m._21 = (xy2 - wz2) * scale.Y;
        m._12 = (xy2 + wz2) * scale.X;

        float xz2 = rotation.x * z2;
        float wy2 = rotation.w * y2;
        m._31 = (xz2 + wy2) * scale.Z;
        m._13 = (xz2 - wy2) * scale.X;

        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }
};

struct FNode {
    FName name;
    FName parentName;
    FTransform transform;
    FString displayName;
    bool bAdvanced;
    unsigned char UnknownData00_6[0xF];
};

D3DMATRIX matrix(FVector rot, FVector origin) {
    float radPitch = (rot.X * float(M_PI) / 180.f);
    float radYaw = (rot.Y * float(M_PI) / 180.f);
    float radRoll = (rot.Z * float(M_PI) / 180.f);
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
    matrix.m[3][0] = origin.X;
    matrix.m[3][1] = origin.Y;
    matrix.m[3][2] = origin.Z;
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

FTransform getBoneIndex(DWORD_PTR mesh, int index) {
    auto bones = read<DWORD_PTR>(mesh + 0x5D0);
    if (!bones)
        bones = read<DWORD_PTR>(mesh + 0x5D0 + 0x10);
    return read<FTransform>(bones + (index * 0x30));
}

FVector getBoneWithRotation(DWORD_PTR mesh, int id) {
    FTransform bone = getBoneIndex(mesh, id);
    auto world = read<FTransform>(mesh + 0x250);
    D3DMATRIX matrix = matrixMultiplication(bone.toMatrix(), world.toMatrix());
    return {matrix._41, matrix._42, matrix._43};
}

struct FMinimalViewInfo {
    FVector Location;
    FVector Rotation;
    float FOV;
    float DesiredFOV;
    float OrthoWidth;
    float OrthoNearClipPlane;
    float OrthoFarClipPlane;
    float AspectRatio;
};

struct FCameraCacheEntry {
    float timestamp;
    FMinimalViewInfo POV;
};

struct FBasedMovementInfo {
    FName boneName;
    FVector location;
    FRotator rotation;
    bool serverHasBaseComponent;
    bool relativeRotation;
    bool serverHasVelocity;

    FBasedMovementInfo(DWORD_PTR pointer) :
        boneName(read<FName>(pointer + 0x0008)),
        location(read<FVector>(pointer + 0x0010)),
        rotation(read<FRotator>(pointer + 0x001C)),
        serverHasBaseComponent(read<bool>(pointer + 0x0028)),
        relativeRotation(read<bool>(pointer + 0x0029)),
        serverHasVelocity(read<bool>(pointer + 0x002A))
    {}
};


FVector worldToScreen(FVector worldPosition, FVector location, FVector rotation, float fov) {
    FVector screen = FVector(0, 0, 0);
    D3DMATRIX tempMatrix = matrix(rotation, FVector(0, 0, 0));
    FVector vAxisX, vAxisY, vAxisZ;
    vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
    FVector vDelta = worldPosition - location;
    FVector vTransformed = FVector(vDelta.dot(vAxisY), vDelta.dot(vAxisZ), vDelta.dot(vAxisX));
    if (vTransformed.Z < 1.f)
        vTransformed.Z = 1.f;
    float screenCenterX = (float) farlight.width / 2.0f;
    float screenCenterY = (float) farlight.height / 2.0f;
    screen.X = screenCenterX + vTransformed.X * (screenCenterX / tanf(fov * (float) M_PI / 360.f)) / vTransformed.Z;
    screen.Y = screenCenterY - vTransformed.Y * (screenCenterX / tanf(fov * (float) M_PI / 360.f)) / vTransformed.Z;
    return screen;
}


std::string nameFromId(const uintptr_t gameBase, int index) {
    auto chunk = (UINT) ((int) (index) >> 16);
    auto name = (USHORT) index;
    auto poolChunk = read<UINT64>(gameBase + 0x6E61180 + ((chunk + 2) * 8));
    auto entryOffset = poolChunk + (ULONG) (2 * name);
    auto nameEntry = read<INT16>(entryOffset);
    auto nameLength = nameEntry >> 6;
    char buff[1028];
    if ((DWORD) nameLength && nameLength > 0) {
        read_array(entryOffset + 2, buff, nameLength);
        buff[nameLength] = '\0';
        return std::string(buff);
    } else return "";
}

enum Bones {
    Root = 0,
    Bip001 = 1,
    pelvis = 2,
    spine_01 = 3,
    spine_02 = 4,
    spine_03 = 5,
    clavicle_l = 6,
    upperarm_l = 7,
    lowerarm_l = 8,
    hand_l = 9,
    thumb_01_l = 10,
    thumb_02_l = 11,
    thumb_03_l = 12,
    index_01_l = 13,
    index_02_l = 14,
    index_03_l = 15,
    middle_01_l = 16,
    middle_02_l = 17,
    middle_03_l = 18,
    ring_01_l = 19,
    ring_02_l = 20,
    ring_03_l = 21,
    pinky_01_l = 22,
    pinky_02_l = 23,
    pinky_03_l = 24,
    clavicle_r = 25,
    upperarm_r = 26,
    lowerarm_r = 27,
    hand_r = 28,
    thumb_01_r = 29,
    thumb_02_r = 30,
    thumb_03_r = 31,
    index_01_r = 32,
    index_02_r = 33,
    index_03_r = 34,
    middle_01_r = 35,
    middle_02_r = 36,
    middle_03_r = 37,
    ring_01_r = 38,
    ring_02_r = 39,
    ring_03_r = 40,
    pinky_01_r = 41,
    pinky_02_r = 42,
    pinky_03_r = 43,
    RightHandWeaponAttach = 44,
    neck_01 = 45,
    head = 46,
    BackpackAttach = 47,
    thigh_l = 48,
    calf_l = 49,
    foot_l = 50,
    ball_l = 51,
    thigh_r = 52,
    calf_r = 53,
    foot_r = 54,
    ball_r = 55,
    VB_spine_03_RightHandWeaponAttach = 56,
    VB_VB_spine_03_RightHandWeaponAttach_hand_r = 57,
    VB_VB_VB_spine_03_RightHandWeaponAttach_hand_r_lowerarm_r = 58,
};


class UObject {
protected:
    DWORD_PTR pointer;
    FName namePrivate;

public:
    explicit UObject(DWORD_PTR pointer) :
            pointer(pointer),
            namePrivate(read<FName>(pointer + 0x0018)) {}

    [[nodiscard]] DWORD_PTR getPointer() const {
        return pointer;
    }

    [[nodiscard]] const FName &getNamePrivate() const {
        return namePrivate;
    }

    [[nodiscard]] DWORD_PTR getOuterPrivate() const {
        return read<DWORD_PTR>(pointer + 0x0020);
    }

    [[nodiscard]] std::string getOuterPrivateName() const {
        return read<FName>(getOuterPrivate() + 0x0018).toString(baseId);
    }
};

class USceneComponent : public UObject {
public:
    FVector relativeLocation;
    FRotator relativeRotation;

    USceneComponent(DWORD_PTR pointer) :
            UObject(pointer),
            relativeLocation(read<FVector>(pointer +( 0x11c + 0x144))),
            relativeRotation(read<FRotator>(pointer + 0x0128)) {}
};

class UCapsuleComponent : public USceneComponent {
public:
    float capsuleHalfHeight;
    float capsuleRadius;

    explicit UCapsuleComponent(DWORD_PTR pointer) : USceneComponent(pointer), capsuleHalfHeight(read<float>(pointer + 0x0590)), capsuleRadius(read<float>(pointer + 0x0594)) {}
};


class UCharacterMovementComponent : public UObject {
public:
    float gravityScale;
    float maxStepHeight;
    float jumpZVelocity;
    float jumpOffJumpZFactor;
    float walkableFloorAngle;
    float walkableFloorZ;
    float GroundFriction;
    float maxWalkSpeed;
    float maxWalkSpeedCrouched;
    float maxSwimSpeed;
    float maxFlySpeed;
    float maxCustomMovementSpeed;
    float maxAcceleration;
    float minAnalogWalkSpeed;
    float brakingFrictionFactor;
    float brakingFriction;
    float brakingSubStepTime;
    float brakingDecelerationWalking;
    float brakingDecelerationFalling;
    float brakingDecelerationSwimming;
    float brakingDecelerationFlying;
    float airControl;
    float airControlBoostMultiplier;
    float airControlBoostVelocityThreshold;
    float fallingLateralFriction;
    float crouchedHalfHeight;
    float buoyancy;
    float perchRadiusThreshold;
    float perchAdditionalHeight;
    FVector acceleration;
    FQuat   lastUpdateRotation;
    FVector lastUpdateLocation;
    FVector lastUpdateVelocity;
    FVector pendingImpulseToApply;
    FVector pendingForceToApply;
    FVector requestedVelocity;

    explicit UCharacterMovementComponent(DWORD_PTR pointer) :
    UObject(pointer),
    gravityScale(read<float>(pointer + 0x0150)),
    maxStepHeight(read<float>(pointer + 0x0154)),
    jumpZVelocity(read<float>(pointer + 0x0158)),
    jumpOffJumpZFactor(read<float>(pointer + 0x015C)),
    walkableFloorAngle(read<float>(pointer + 0x0160)),
    walkableFloorZ(read<float>(pointer + 0x0164)),
    GroundFriction(read<float>(pointer + 0x016C)),
    maxWalkSpeed(read<float>(pointer + 0x018C)),
    maxWalkSpeedCrouched(read<float>(pointer + 0x0190)),
    maxSwimSpeed(read<float>(pointer + 0x0194)),
    maxFlySpeed(read<float>(pointer + 0x0198)),
    maxCustomMovementSpeed(read<float>(pointer + 0x019C)),
    maxAcceleration(read<float>(pointer + 0x01A0)),
    minAnalogWalkSpeed(read<float>(pointer + 0x01A4)),
    brakingFrictionFactor(read<float>(pointer + 0x01A8)),
    brakingFriction(read<float>(pointer + 0x01AC)),
    brakingSubStepTime(read<float>(pointer + 0x01B0)),
    brakingDecelerationWalking(read<float>(pointer + 0x01B4)),
    brakingDecelerationFalling(read<float>(pointer + 0x01B8)),
    brakingDecelerationSwimming(read<float>(pointer + 0x01BC)),
    brakingDecelerationFlying(read<float>(pointer + 0x01C0)),
    airControl(read<float>(pointer + 0x01C4)),
    airControlBoostMultiplier(read<float>(pointer + 0x01C8)),
    airControlBoostVelocityThreshold(read<float>(pointer + 0x01CC)),
    fallingLateralFriction(read<float>(pointer + 0x01D0)),
    crouchedHalfHeight(read<float>(pointer + 0x01D4)),
    buoyancy(read<float>(pointer + 0x01D8)),
    perchRadiusThreshold(read<float>(pointer + 0x01DC)),
    perchAdditionalHeight(read<float>(pointer + 0x01E0)),
    acceleration(read<FVector>(pointer + 0x022C)),
    lastUpdateRotation(read<FQuat>(pointer + 0x0240)),
    lastUpdateLocation(read<FVector>(pointer + 0x0250)),
    lastUpdateVelocity(read<FVector>(pointer + 0x025C)),
    pendingImpulseToApply(read<FVector>(pointer + 0x0274)),
    pendingForceToApply(read<FVector>(pointer + 0x0280)),
    requestedVelocity(read<FVector>(pointer + 0x03A4))
    {}
};


class UGameInstance : public UObject {
public:
    DWORD_PTR localPlayerPointer;
    explicit UGameInstance(DWORD_PTR pointer) : UObject(pointer), localPlayerPointer(read<DWORD_PTR>(pointer + 0x0038)) {}
};


class URig : public UObject {
public:
    TArray<FTransform> transformBases;
    TArray<FNode> nodes;

    explicit URig(DWORD_PTR pointer) : UObject(pointer), transformBases(read<TArray<FTransform>>(pointer + 0x0030)), nodes(read<TArray<FNode>>(pointer + 0x0040)) {}
};

class UStaticMesh : public UObject {
public:
    explicit UStaticMesh(DWORD_PTR pointer) : UObject(pointer) {}
};

class UStaticMeshComponent : public UObject {
public:
    UStaticMesh staticMesh;

    explicit UStaticMeshComponent(DWORD_PTR pointer) : UObject(pointer), staticMesh(UStaticMesh(pointer + 0x05C0)) {}
};

class USkinnedMeshComponent : public USceneComponent {
public:
    URig boneArray;

    explicit USkinnedMeshComponent(DWORD_PTR pointer) : USceneComponent(pointer), boneArray(URig(pointer + 0x05D0)) {}
};

class USkeletalMeshComponent : public USkinnedMeshComponent {
public:
    explicit USkeletalMeshComponent(DWORD_PTR pointer) : USkinnedMeshComponent(pointer) {}
};