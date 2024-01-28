#include <string>
#include <cstdint>
#include <cmath>
#include <d3d9.h>
#include <DirectXMath.h>
#include "../utils/driver.h"

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

    [[nodiscard]] std::string toString(const uintptr_t gameBase) const {
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
        return sqrtf((xV * xV) + (yV * yV) + (zV * zV)) * 0.0135f;
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

    [[nodiscard]] std::string debug() const {
        return "{x=" + std::to_string(static_cast<int>(x)) + ", y=" + std::to_string(static_cast<int>(y)) + ", z=" + std::to_string(static_cast<int>(z)) + "}";
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

int getHeadBoneIndex(int32_t characterID) {
    switch (characterID) {
        case 100006:
        case 100015:
        case 100004:
            return 51;
        case 100013:
            return 46;
        default:
            return 46;
    }
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


std::string nameFromId(const uintptr_t gameBase, int id) {
    auto chunk = (UINT) ((int) (id) >> 16);
    auto name = (USHORT) id;
    auto poolChunk = read<UINT64>(gameBase + offsets::gNames + ((chunk + 2) * 8));
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
    FVector Location; // 0x00(0x0c)
    FRotator Rotation; // 0x0c(0x0c)
    float FOV; // 0x18(0x04)
    float DesiredFOV; // 0x1c(0x04)
    float OrthoWidth; // 0x20(0x04)
    float OrthoNearClipPlane; // 0x24(0x04)
    float OrthoFarClipPlane; // 0x28(0x04)
    float AspectRatio; // 0x2c(0x04)
};


struct FCameraCacheEntry {
    float timestamp;                                                  // 0x0000   (0x0004)
    unsigned char UnknownData00_5[0xC];                                       // 0x0004   (0x000C)  MISSED
    FMinimalViewInfo POV;                                                        // 0x0010   (0x0650)
};


FVector projectWorldToScreen(DWORD_PTR localPlayerController, FVector worldLocation, FVector screenSize) {
    FVector screen = FVector(0, 0, 0);
    auto cameraCache = read<FCameraCacheEntry>(read<uintptr_t>(localPlayerController + offsets::pcPlayerCameraManager) + offsets::pcmCameraCachePrivate);
    D3DMATRIX tempMatrix = matrix(cameraCache.POV.Rotation, FVector(0, 0, 0));
    FVector vAxisX, vAxisY, vAxisZ;
    vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
    FVector vDelta = worldLocation - cameraCache.POV.Location;
    FVector vTransformed = FVector(vDelta.dot(vAxisY), vDelta.dot(vAxisZ), vDelta.dot(vAxisX));
    if (vTransformed.z < 1.f)
        vTransformed.z = 1.f;
    float fovAngle = cameraCache.POV.FOV;
    float screenCenterX = screenSize.x / 2.0f;
    float screenCenterY = screenSize.y / 2.0f;
    screen.x = screenCenterX + vTransformed.x * (screenCenterX / tanf(fovAngle * (float) M_PI / 360.f)) / vTransformed.z;
    screen.y = screenCenterY - vTransformed.y * (screenCenterX / tanf(fovAngle * (float) M_PI / 360.f)) / vTransformed.z;
    return screen;
}



struct TUObjectArray
{
    DWORD_PTR Objects;
    static int getMaxElements() ;
    static int getElementsSize() ;
    [[nodiscard]] DWORD_PTR getObjectPointer(int id) const;
};

int TUObjectArray::getMaxElements() {
    auto gObjects = read<DWORD_PTR>(baseId + offsets::gObject);
    if (gObjects == 0)
        return 0;

    return read<int>(gObjects + 0xC - sizeof(int));
}
int TUObjectArray::getElementsSize() {
    auto gObjects = read<DWORD_PTR>(baseId + offsets::gObject);
    if (gObjects == 0)
        return 0;
    return read<int>(gObjects + 0xC);
}

DWORD_PTR TUObjectArray::getObjectPointer(int id) const {
    if (id < 0 || id >= getElementsSize() || !Objects)
        return -1;
    const int elementsPerChunk = 64 * 1024;
    const int chunkIndex = id / elementsPerChunk;
    const int withinChunkIndex = id % elementsPerChunk;
    auto chunk = read<DWORD_PTR>(Objects + chunkIndex);
    if (!chunk)
        return -1;
    return read<DWORD_PTR>(chunk + (withinChunkIndex * 0x18));

    //auto objects = read<DWORD_PTR>(baseId + offsets::gObject);
    //auto chunk = read<DWORD_PTR>(objects + 8 * (id / 0x10000));
    //return read<DWORD_PTR>(chunk + 24 * (id % 0x10000));
}


struct TWeakObjectPtr {
    uint8_t ObjectIndex;
    uint8_t ObjectSerialNumber;
    [[nodiscard]] DWORD_PTR getPointer() const {
        auto gObjects = read<TUObjectArray>(baseId + offsets::gObject);
        return gObjects.getObjectPointer(ObjectIndex);
    }
};











//
//class ASolarWeapon {
//private:
//    DWORD_PTR address;
//public:
//    explicit ASolarWeapon(DWORD_PTR address) : address(address) {}
//
//    [[nodiscard]] ASolarWeaponProjectileSpline getProjSpline() const {
//        return read<ASolarWeaponProjectileSpline>(address + 0x0460);
//    }
//
//    [[nodiscard]] UGlobalWeaponConfig getGlobalConfigPrivate() const {
//        return read<UGlobalWeaponConfig>(address + 0x0488);
//    }
//
//    [[nodiscard]] USingleWeaponConfig getConfig() const {
//        return read<USingleWeaponConfig>(address + 0x0470);
//    }
//
//    [[nodiscard]] FHitResult getHitResult() const {
//        return read<FHitResult>(address + 0x0760);
//    }
//
//    [[nodiscard]] bool isTracingAimTarget() const {
//        return read<bool>(address + 0x066D);
//    }
//
//    [[nodiscard]] bool isDetectedEnemy() const {
//        return read<bool>(address + 0x066C);
//    }
//
//    [[nodiscard]] int32_t getClipRemainAmmoCount() const {
//        return read<int32_t>(address + 0x04F0);
//    }
//
//    [[nodiscard]] int32_t getExtraAmmo() const {
//        return read<int32_t>(address + 0x0498);
//    }
//
//    [[nodiscard]] float getFireChargingTime() const {
//        return read<float>(address + 0x03D8);
//    }
//
//    [[nodiscard]] float getFireChargingSumTime() const {
//        return read<float>(address + 0x03E0);
//    }
//};